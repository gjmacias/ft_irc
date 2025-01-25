#include "Server.hpp"

/*
###############################################################################
#								PRIVATE_MESSAGE								  #
###############################################################################
*/

void	Server::CheckForChannels_Clients(std::vector<std::string> &tmp, int fd)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			if (!GetChannel(tmp[i]))//ERR_NOSUCHNICK (401) // if the channel doesn't exist
				{
					senderror(401, "#" + tmp[i], GetClient(fd)->GetFd(), " :No such nick/channel\r\n"); 
					tmp.erase(tmp.begin() + i); 
					i--;
				}
			else if (!GetChannel(tmp[i])->GetClientInChannel(GetClient(fd)->GetNickName())) //ERR_CANNOTSENDTOCHAN (404) // if the client is not in the channel
				{
					senderror(404, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :Cannot send to channel\r\n");
					tmp.erase(tmp.begin() + i);
					i--;
				}
			else tmp[i] = "#" + tmp[i];
		}
		else
		{
			if (!GetClientNick(tmp[i]))//ERR_NOSUCHNICK (401) // if the client doesn't exist
				{
					senderror(401, tmp[i], GetClient(fd)->GetFd(), " :No such nick/channel\r\n");
					tmp.erase(tmp.begin() + i);
					i--;
				}
		}
	}
}

void	Server::PrivateMessageCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	(void)splited_cmd;
	(void)fd;
	std::vector<std::string> tmp = splited_cmd;

	if (!tmp.size())//ERR_NORECIPIENT (411) // if the client doesn't specify the recipient
		{
			senderror(411, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :No recipient given (PRIVMSG)\r\n");
			return;
		}
	if (message.empty())//ERR_NOTEXTTOSEND (412) // if the client doesn't specify the message
		{
			senderror(412, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :No text to send\r\n");
			return;
		}
	if (tmp.size() > 10) //ERR_TOOMANYTARGETS (407) // if the client send the message to more than 10 clients
		{
			senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many recipients\r\n");
			return;
		}
	CheckForChannels_Clients(tmp, fd); // check if the channels and clients exist
	for (size_t i = 0; i < tmp.size(); i++)
	{// send the message to the clients and channels
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost PRIVMSG #" + tmp[i] + " :" + message + "\r\n";
			GetChannel(tmp[i])->sendTo_all(resp, fd);
		}
		else
		{
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost PRIVMSG " + tmp[i] + " :" + message + "\r\n";
			_sendResponse(resp, GetClientNick(tmp[i])->GetFd());
		}
	}
}
