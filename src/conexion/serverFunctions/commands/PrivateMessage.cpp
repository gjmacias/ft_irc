#include "Server.hpp"

/*
###############################################################################
#								PRIVATE_MESSAGE								  #
###############################################################################
*/

void	Server::CheckForChannels_Clients(std::vector<std::string> &splited_cmd, int fd)
{
	for (size_t i = 0; i < splited_cmd.size(); i++)
	{
		if (splited_cmd[i][0] == '#')
		{
			splited_cmd[i].erase(splited_cmd[i].begin());
			if (!GetChannel(splited_cmd[i]))//ERR_NOSUCHNICK (401) // if the channel doesn't exist
				{
					SendError(401, GetClient(fd)->GetFd(), "#" + splited_cmd[i], " :No such nick/channel\r\n"); 
					splited_cmd.erase(splited_cmd.begin() + i); 
					i--;
				}
			else if (!GetChannel(splited_cmd[i])->GetClientInChannel(GetClient(fd)->GetNickName())) //ERR_CANNOTSENDTOCHAN (404) // if the client is not in the channel
				{
					SendErrorV2(404, GetClient(fd)->GetFd(), GetClient(fd)->GetNickName(), "#" + splited_cmd[i], " :Cannot send to channel\r\n");
					splited_cmd.erase(splited_cmd.begin() + i);
					i--;
				}
			else splited_cmd[i] = "#" + splited_cmd[i];
		}
		else
		{
			if (!GetClient_Nickname(splited_cmd[i]))//ERR_NOSUCHNICK (401) // if the client doesn't exist
				{
					SendError(401, GetClient(fd)->GetFd(), splited_cmd[i], " :No such nick/channel\r\n");
					splited_cmd.erase(splited_cmd.begin() + i);
					i--;
				}
		}
	}
}

void	Server::PrivateMessageCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	if (!splited_cmd.size())//ERR_NORECIPIENT (411) // if the client doesn't specify the recipient
		{
			SendError(411, GetClient(fd)->GetFd(), GetClient(fd)->GetNickName(), " :No recipient given (PRIVMSG)\r\n");
			return;
		}
	if (message.empty())//ERR_NOTEXTTOSEND (412) // if the client doesn't specify the message
		{
			SendError(412, GetClient(fd)->GetFd(), GetClient(fd)->GetNickName(), " :No text to send\r\n");
			return;
		}
	if (splited_cmd.size() > 10) //ERR_TOOMANYTARGETS (407) // if the client send the message to more than 10 clients
		{
			SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickName(), " :Too many recipients\r\n");
			return;
		}
	CheckForChannels_Clients(splited_cmd, fd); // check if the channels and clients exist
	for (size_t i = 0; i < splited_cmd.size(); i++)
	{// send the message to the clients and channels
		if (splited_cmd[i][0] == '#')
		{
			splited_cmd[i].erase(splited_cmd[i].begin());
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUsername() + "@localhost PRIVMSG #" + splited_cmd[i] + " :" + message + "\r\n";
			GetChannel(splited_cmd[i])->SendMeToAll(resp, fd);
		}
		else
		{
			std::string resp = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUsername() + "@localhost PRIVMSG " + splited_cmd[i] + " :" + message + "\r\n";
			SendResponse(resp, GetClient_Nickname(splited_cmd[i])->GetFd());
		}
	}
}
