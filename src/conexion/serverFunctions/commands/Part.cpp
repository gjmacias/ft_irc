#include "Server.hpp"

/*
###############################################################################
#									PART									  #
###############################################################################
*/

void	Server::PartCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	(void)splited_cmd;
	(void)fd;
	std::vector<std::string>	tmp;
	std::string					reason;
	std::stringstream			ss;
	bool						flag;
	size_t						i;
	size_t						j;

	if (!SplitCmdPart(cmd, tmp, reason, fd))
	{
		senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough  parameters\r\n");
		return ;
	}
	for (i = 0, i < tmp.size(); i++)
	{
		flag = false;
		for (j = 0; i < tmp.size(); j++)
		{
			if (this->channels[j].GetName() == tmp[i])
			{
				flag = true;
				if (!channels[j].get_client(fd) && !channels[j].get_admin(fd))
				{
					senderror(442, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :You are not on that channel\r\n");
					continue ;
				}
				ss << ":" << GetClient(fd)->GetNickName() << "!~" << GetClient(fd)->GetUserName() << "@" << "localhost" << " PART #" << tmp[i];
				if (!reason.empty())
					ss << " :" << reason << "\r\n";
				else
					ss << "\r\n";
				channels[j].sendTo_all(ss.str());
				if (channels[j].get_admin(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd()))
					channels[j].remove_admin(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd());
				else
					channels[j].remove_client(channels[j].GetClientInChannel(GetClient(fd)->GetNickName())->GetFd());
				if (channels[j].GetClientsNumber() == 0)
					channels.erase(channels.begin() + j);
			}
		}
		if (!flag) // if the channel doesn't exist
			senderror(403, GetClient(fd)->GetNickName(), "#" + tmp[i], GetClient(fd)->GetFd(), " :No such channel\r\n");
	}
}
