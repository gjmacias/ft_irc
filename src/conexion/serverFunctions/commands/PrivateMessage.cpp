#include "Server.hpp"

/*
###############################################################################
#								PRIVATE_MESSAGE								  #
###############################################################################
*/

void FindPM(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++)
	{
		if (cmd[i] != ' ')
		{
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) 
		str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdPM(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str;
	std::string msg;
	int count = 2;

	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) 
		return (std::string(""));
	FindPM(cmd, tmp[1], msg);
	return (msg);
}

std::string SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp)
{
	std::string str = SplitCmdPM(cmd, tmp);
	if (tmp.size() != 2) 
	{
		tmp.clear(); 
		return std::string("");
	}
	tmp.erase(tmp.begin());
	std::string str1 = tmp[0]; 
	std::string str2; 
	tmp.clear();
	for (size_t i = 0; i < str1.size(); i++)
	{//split the first string by ',' to get the channels names
		if (str1[i] == ',')
			{
				tmp.push_back(str2); 
				str2.clear();
			}
		else str2 += str1[i];
	}
	tmp.push_back(str2);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
	{
		if (tmp[i].empty())
			tmp.erase(tmp.begin() + i--);
	}
	if (str[0] == ':') 
		str.erase(str.begin());
	else //shrink to the first space
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] == ' ')
			{
				str = str.substr(0, i);
				break;
			}
		}
	}
	return  (str);
	
}

void	Server::CheckForChannels_Clients(std::vector<std::string> &tmp, int fd)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			if (!GetChannel(tmp[i]))//ERR_NOSUCHNICK (401) // if the channel doesn't exist
				{
					SendError(401, GetClient(fd)->GetFd(), "#" + tmp[i], " :No such nick/channel\r\n"); 
					tmp.erase(tmp.begin() + i); 
					i--;
				}
			else if (!GetChannel(tmp[i])->IsClientInChannel(GetClient(fd)->GetNickname())) //ERR_CANNOTSENDTOCHAN (404) // if the client is not in the channel
				{
					SendErrorV2(404, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], " :Cannot send to channel\r\n");
					tmp.erase(tmp.begin() + i);
					i--;
				}
			else tmp[i] = "#" + tmp[i];
		}
		else
		{
			if (!GetClient_Nickname(tmp[i]))//ERR_NOSUCHNICK (401) // if the client doesn't exist
				{
					SendError(401, GetClient(fd)->GetFd(), tmp[i], " :No such nick/channel\r\n");
					tmp.erase(tmp.begin() + i);
					i--;
				}
		}
	}
}

void	Server::PrivateMessageCommand(std::string cmd, int &fd)
{
	std::vector<std::string> tmp;
	std::string message = SplitCmdPrivmsg(cmd, tmp);
	if (!tmp.size())//ERR_NORECIPIENT (411) // if the client doesn't specify the recipient
		{
			SendError(411, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :No recipient given (PRIVMSG)\r\n");
			return;
		}
	if (message.empty())//ERR_NOTEXTTOSEND (412) // if the client doesn't specify the message
		{
			SendError(412, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :No text to send\r\n");
			return;
		}
	if (tmp.size() > 10) //ERR_TOOMANYTARGETS (407) // if the client send the message to more than 10 clients
		{
			SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Too many recipients\r\n");
			return;
		}
	CheckForChannels_Clients(tmp, fd); // check if the channels and clients exist
	for (size_t i = 0; i < tmp.size(); i++)
	{// send the message to the clients and channels
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			std::string resp = ":" + GetClient(fd)->GetNickname() + "!~" + GetClient(fd)->GetUsername() + "@localhost PRIVMSG #" + tmp[i] + " :" + message + "\r\n";
			GetChannel(tmp[i])->SendMeToAll(fd, resp);
		}
		else
		{
			std::string resp = ":" + GetClient(fd)->GetNickname() + "!~" + GetClient(fd)->GetUsername() + "@localhost PRIVMSG " + tmp[i] + " :" + message + "\r\n";
			SendResponse(resp, GetClient_Nickname(tmp[i])->GetFd());
		}
	}
}
