#include "Server.hpp"

/*
###############################################################################
#									KICK									  #
###############################################################################
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
				SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You're not on that channel\r\n");
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
					Channel->sentTo_all(ss.str());
					if (Channel->GetAdmin(Channel->IsClientInChannel(user)->GetFd()))
						Channel->RemoveAdmin(Channel->IsClientInChannel(user)->GetFd());
					else
						Channel->RemoveClient(Channel->IsClientInChannel(user)->GetFd());
					if (Channel->CountAllClients() == 0)
						_channels.erase(_channels.begin() + i);
				}
				else // if the client to kick is not in the channel
				{
					SendErrorV2(441, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], GetClient(fd)->GetFd(), " :They are not in the channel\r\n");
					continue ;
				}
			}
			else // if the client is not admin
			{
				SendErrorV2(482, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You are not channel operator\r\n");
				continue ;
			}
		}
		else // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");

	}
}
