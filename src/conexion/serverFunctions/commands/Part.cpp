#include "Server.hpp"

/*
###############################################################################
#									PART									  #
###############################################################################
*/

void	Server::PartCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	std::vector<std::string>	reason;
	std::stringstream			ss;
	bool						flag;
	size_t						i;
	size_t						j;

	for (i = 0, i < splited_cmd.size(); i++)
	{
		flag = false;
		for (j = 0; i < splited_cmd.size(); j++)
		{
			if (this->channels[j].GetName() == splited_cmd[i])
			{
				flag = true;
				if (!channels[j].GelClient(fd) && !channels[j].GetAdmin(fd))
				{
					SendErrorV2(442, GetClient(fd)->GetNickname(), "#" + splited_cmd[i], GetClient(fd)->GetFd(), " :You are not on that channel\r\n");
					continue ;
				}
				ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUserName() << "@" << "localhost" << " PART #" << splited_cmd[i];
				if (!reason.empty())
					ss << " :" << reason << "\r\n";
				else
					ss << "\r\n";
				channels[j].sendTo_all(ss.str());
				if (channels[j].GetAdmin(channels[j].GetClientInChannel(GetClient(fd)->GetNickname())->GetFd()))
					channels[j].RemoveAdmin(channels[j].GetClientInChannel(GetClient(fd)->GetNickname())->GetFd());
				else
					channels[j].RemoveClient(channels[j].GetClientInChannel(GetClient(fd)->GetNickname())->GetFd());
				if (channels[j].CountAllClients() == 0)
					channels.erase(channels.begin() + j);
			}
		}
		if (!flag) // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + splited_cmd[i], " :No such channel\r\n");
	}
}
