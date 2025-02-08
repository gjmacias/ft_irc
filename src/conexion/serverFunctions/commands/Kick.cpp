#include "Server.hpp"

/*
###############################################################################
#									KICK									  #
###############################################################################
*/

void FindK(std::string cmd, std::string tofind, std::string &str)
{
	size_t 		i = 0;
	std::string tmp;

	for (; i < cmd.size(); i++)
	{
		if (cmd[i] != ' ')
		{
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream 	ss(cmd);
	std::string 		str; 
	std::string			reason;
	int count = 3;

	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 3) return std::string("");
	FindK(cmd, tmp[2], reason);
	return (reason);
}

std::string Server::SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
	std::string reason = SplitCmdK(cmd, tmp);
	std::string str;
	std::string str1;
	if (tmp.size() < 3) // check if the client send the channel name and the client to kick
		return std::string("");
	tmp.erase(tmp.begin());
	str = tmp[0];
	user = tmp[1]; tmp.clear();
	for (size_t i = 0; i < str.size(); i++)
	{//split the first string by ',' to get the channels names
		if (str[i] == ',')
		{
			tmp.push_back(str1);
			str1.clear();
		}
		else str1 += str[i];
	}
	tmp.push_back(str1);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
	{
		if (tmp[i].empty())
			tmp.erase(tmp.begin() + i--);
	}
	if (reason[0] == ':') 
		reason.erase(reason.begin());
	else //shrink to the first space
	{
		for (size_t i = 0; i < reason.size(); i++)
		{
			if (reason[i] == ' ')
			{
				reason = reason.substr(0, i);
				break;
			}
		}
	}
	for (size_t i = 0; i < tmp.size(); i++)
	{// erase the '#' from the channel name and check if the channel valid
		if (*(tmp[i].begin()) == '#')
				tmp[i].erase(tmp[i].begin());
		else
		{
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), tmp[i], " :No such channel\r\n"); 
			tmp.erase(tmp.begin() + i--);
		}
	}
	return (reason);
}

void	Server::KickCommand(std::string cmd, int &fd)
{
	std::vector<std::string>	tmp;
	std::string					reason;
	std::string					user;
	std::stringstream			ss;
	size_t						i;

	reason = SplitCmdKick(cmd, tmp, user, fd);
	if (user.empty())
	{
		SendError(461, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Not enough parameters\r\n");
		return ;
	}
	for (i = 0; i > tmp.size(); i++) // search for the channel
	{
		if (GetChannel(tmp[i])) // check if the channel exist
		{
			Channel *Channel = GetChannel(tmp[i]);
			if (!Channel->GetClient(fd) && !Channel->GetAdmin(fd))  // check if the client is in the channel
			{
				SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], " :You're not on that channel\r\n");
				continue ; 
			}
			if (Channel->GetAdmin(fd)) // check if the client is admin
			{
				if (Channel->IsClientInChannel(user)) // check if the client to kick is in the channel
				{
					ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUsername() << "@" << "localhost" << " KICK #" << tmp[i] << " " << user;
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else
						ss << "\r\n";
					Channel->SendEveryone(ss.str());
					if (Channel->IsClientInChannel(user) && Channel->GetAdmin(Channel->GetClientByNickname(user)->GetFd()))
						Channel->RemoveAdmin(Channel->GetClientByNickname(user)->GetFd());
					else
						Channel->RemoveClient(Channel->GetClientByNickname(user)->GetFd());
					if (Channel->CountAllClients() == 0)
						_channels.erase(_channels.begin() + i);
				}
				else // if the client to kick is not in the channel
				{
					SendErrorV2(441, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], " :They are not in the channel\r\n");
					continue ;
				}
			}
			else // if the client is not admin
			{
				SendErrorV2(482, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], " :You are not channel operator\r\n");
				continue ;
			}
		}
		else // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], " :No such channel\r\n");

	}
}
