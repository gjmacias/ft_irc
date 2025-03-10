#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									PART									  #
###############################################################################
*/

void	Server::PartCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	bool						flag;
	std::stringstream			ss;
	std::string					reason;
	std::vector<std::string>	list_channels;

	if (splited_cmd.size() < 2)
	{
		ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname());
		return ;
	}
	list_channels = split_delimeter(splited_cmd[1], ',');
	for (size_t i = 0; i < splited_cmd.size(); i++)
	{
		flag = false;

		if (!splited_cmd.empty() && splited_cmd[1 + i] == '#')
			splited_cmd.erase(splited_cmd.begin());
		for (size_t j = 0; i < splited_cmd.size(); j++)
		{
			if (this->_channels[j].GetName() == splited_cmd[1 + i])
			{
				flag = true;
				if (!_channels[j].GetClient(fd) && !_channels[j].GetAdmin(fd))
				{
					SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + splited_cmd[1 + i], " :You are not on that channel\r\n");
					continue ;
				}
				ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUsername() << "@" << "localhost" << " PART #" << splited_cmd[1 + i];
				if (!reason.empty())
					ss << " :" << reason << "\r\n";
				else
					ss << "\r\n";
				_channels[j].SendEveryone(ss.str());
				if (_channels[j].GetAdmin(fd))
					_channels[j].RemoveAdmin(fd);
				else
					_channels[j].RemoveClient(fd);
				if (_channels[j].CountAllClients() == 0)
					_channels.erase(_channels.begin() + j);
			}
		}
		if (!flag) // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), "#" + splited_cmd[1 + i], " :No such channel\r\n");
	}
}

