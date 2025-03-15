#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									PART									  #
###############################################################################
*/

void	Server::PartCommand(std::vector<std::string> &splited_cmd, std::string cmd_reason, int fd)
{
	bool						flag = false;
	int							it = 0;
	size_t						position = 0;
	std::stringstream			ss;
	std::vector<std::string>	list_channels;

	if (splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return ;
	}
	list_channels = split_delimeter(splited_cmd[1], ',');
	while (it < 2)
	{
		position = cmd_reason.find(splited_cmd[it]);
		cmd_reason = cmd_reason.substr(position + splited_cmd[it].size());
		position = cmd_reason.find_first_not_of("\t\v ");
		if (position != std::string::npos)
			cmd_reason = cmd_reason.substr(position);
		else
			cmd_reason.clear();
		it++;
	}
	for (size_t i = 0; i < list_channels.size(); i++)
	{
		flag = false;

		for (size_t j = 0; j < this->_channels.size(); j++)
		{
			if (this->_channels[j].GetName() == list_channels[i])
			{
				flag = true;
				if (!_channels[j].GetClient(fd) && !_channels[j].GetAdmin(fd))
				{
					SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), list_channels[i], " :You are not on that channel\r\n");
					continue ;
				}
				ss.str("");
				ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUsername() << "@" << "localhost" << " PART " << list_channels[i];
				if (!cmd_reason.empty())
				{
					ss << " ";
					if (cmd_reason[0] == ':')
						ss << cmd_reason << "\r\n";
					else
						ss << splited_cmd[2] << "\r\n";
				}
				else
					ss << "\r\n";
				_channels[j].SendEveryone(ss.str());
				if (_channels[j].GetAdmin(fd))
					_channels[j].RemoveAdmin(fd);
				else
					_channels[j].RemoveClient(fd);
				if (_channels[j].CountAllClients() == 0)
					_channels.erase(_channels.begin() + j);
				else if (_channels[j].CountAdmins() == 0)
					_channels[j].ChangeClientToAdmin((_channels[j].GetObligatedAdmin())->GetNickname());
			}
		}
		if (!flag) // if the channel doesn't exist
			SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), list_channels[i], " :No such channel\r\n");
	}
}

