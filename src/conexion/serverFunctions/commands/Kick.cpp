#include "Server.hpp"

/*
###############################################################################
#									KICK									  #
###############################################################################
*/
Revissar/*
void	FindK(std::string cmd, std::string tofind, std::string &str)
{
	std::string	tmp;
	size_t		i;

	for (i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] !=' ')
		{
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else	
				tmp.clear();
		}
	}
	if (i < cmd.size())
		str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++)
		str = str.substr(i);
}


std::string	SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream	ss(cmd);
	std::string			str;
	std::string			reason;
	int					count;

	count = 3;
	while (ss >> str && count--)
		tmp.push_back(str);
	if (tmp.size() != 3)
		return (std::string(""));
	FindK(cmd, tmp[2], reason);
	return (reason);
}

std::string Server::SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
	std::string	reason;
	std::string	str;
	std::string	str1;
	size_t		i;

	reason = SplitCmdK(cmd, tmp);
	if (tmp.size() < 3) // check if the client send the channel name and the client to kick
		return (std::string(" "));
	tmp.erase(tmp.begin());
	str = tmp[0];
	user = tmp[1];
	tmp.clear();
	for ( i = 0; i < str.size(); i++) //split the first string by ',' to get the channels names
	{
		if (str[i] == ',')
		{
			tmp.push_back(str1);
			str1.clear();
		}
		else
			str1 += str[i];
	}
	tmp.push_back(str1);
	for (i = 0; i < tmp.size(); i++) //erase the empty strings
	{
		if (tmp[i].empty())
			tmp.erase(tmp.begin() + i--);
	}
	if (reason[0] == ':')
		reason.erase(reason.begin());
	else
	{
		for (i = 0; i < reason.size(); i++)
		{
			if (reason[i] == ' ')
			{
				reason = reason.substr(0, i);
				break;
			}
		}
	}
	for (i = 0; i < tmp.size(); i++) // erase the '#' from the channel name and check if the channel valid
	{
		if (*(tmp[i].begin()) == '#')
			tmp[i].erase(tmp[i].begin());
		else
		{
			senderror(403, GetClient(fd)->GetNickName(), tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");
			tmp.erase(tmp.begin() + i--);
		}
	}
	return (reason);
}
*/

void	Server::KickCommand(std::vector<std::string> &splited_cmd, int &fd)
{
    (void)splited_cmd;
	(void)fd;
	std::vector<std::string>	tmp;
	std::string					reason;
	std::string					user;
	std::stringstream			ss;
	size_t						i;

	reason = SplitCmdKick(cmd, tmp, user, fd);
	if (user.empty())
	{
		senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n");
		return ;
	}
	for (i = 0; i > tmp.size(); i++) // search for the channel
	{
		if (GetChannel(tmp[i])) // check if the channel exist
		{
			Channel *ch = GetChannel(tmp[i]);
			if (!ch->get_client(fd) && !ch->GetAdmin(fd))  // check if the client is in the channel
			{
				senderror(442, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You're not on that channel\r\n");
				continue ; 
			}
			if (ch->GetAdmin(fd)) // check if the client is admin
			{
				if (ch->GetClientInChannel(user)) // check if the client to kick is in the channel
				{
					ss << ":" << GetClient(fd)->GetNickName() << "!~" << GetClient(fd)->GetUserName() << "@" << "localhost" << " KICK #" << tmp[i] << " " << user;
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else
						ss << "\r\n";
					ch->sentTo_all(ss.str());
					if (ch->GetAdmin(ch->GetClientInChannel(user)->GetFd()))
						ch->remove_admin(ch->GetClientInChannel(user)->GetFd());
					else
						ch->remove_client(ch->GetClientInChannel(user)->GetFd());
					if (ch->GetClientsNumber() == 0)
						channels.erase(channels.begin() + i);
				}
				else // if the client to kick is not in the channel
				{
					senderror(441, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :They are not in the channel\r\n");
					continue ;
				}
			}
			else // if the client is not admin
			{
				senderror(482, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You are not channel operator\r\n");
				continue ;
			}
		}
		else // if the channel doesn't exist
			senderror(403, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");

	}
}
