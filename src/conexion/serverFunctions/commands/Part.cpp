#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									PART									  #
###############################################################################
*/

void	Server::PartCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	std::string					reason;
	std::stringstream			ss;
	bool						flag;
	size_t						i;
	size_t						j;

	for (i = 0; i < splited_cmd.size(); i++)
	{
		flag = false;
		for (j = 0; i < splited_cmd.size(); j++)
		{
			if (this->_channels[j].GetName() == splited_cmd[i])
			{
				flag = true;
				if (!_channels[j].GetClient(fd) && !_channels[j].GetAdmin(fd))
				{
					SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + splited_cmd[i], " :You are not on that channel\r\n");
					continue ;
				}
				ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUsername() << "@" << "localhost" << " PART #" << splited_cmd[i];
				if (!reason.empty())
					ss << " :" << reason << "\r\n";
				else
					ss << "\r\n";
				_channels[j].SendEveryone(ss.str());
				if (_channels[j].GetAdmin(fd) && _channels[j].IsClientInChannel(GetClient(fd)->GetNickname()))
					_channels[j].RemoveAdmin(fd);
				else
					_channels[j].RemoveClient(fd);
				if (_channels[j].CountAllClients() == 0)
					_channels.erase(_channels.begin() + j);
			}
		}
		if (!flag) // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + splited_cmd[i], " :No such channel\r\n");
	}
}

