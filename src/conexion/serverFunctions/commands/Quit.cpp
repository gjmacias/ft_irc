#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									Quit									  #
###############################################################################
*/

void	Server::QuitCommand(std::vector<std::string> &splited_cmd, std::string cmd_reason, int fd)
{
	size_t		i;
	std::string response;

	i = cmd_reason.find(splited_cmd[0]);
	cmd_reason = cmd_reason.substr(i + splited_cmd[0].size());
	i = cmd_reason.find_first_not_of("\t\v ");
	if (i != std::string::npos)
		cmd_reason = cmd_reason.substr(i);
	else
		cmd_reason.clear();
	if (splited_cmd.size() > 1 && cmd_reason[0] != ':')
			cmd_reason = splited_cmd[2];
	for (i = 0; i <_channels.size(); i++)
	{
		if (_channels[i].GetClient(fd))
		{
			_channels[i].RemoveClient(fd);
			if (_channels[i].CountAllClients() == 0)
				_channels.erase(_channels.begin() + i);
			else
			{
				response = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@" + GetClient(fd)->GetIPaddress() + " QUIT " + cmd_reason + "\r\n";
				_channels[i].SendEveryone(response);
			}
		}
		else if (_channels[i].GetAdmin(fd))
		{
			_channels[i].RemoveAdmin(fd);
			if (_channels[i].CountAllClients() == 0) //Buscar equivalente
				_channels.erase(_channels.begin() + i);
			else
			{
				response = ":" + GetClient(fd)->GetNickname() + "!" + GetClient(fd)->GetUsername() + "@" + GetClient(fd)->GetIPaddress() + " QUIT " + cmd_reason +"\r\n";
				_channels[i].SendEveryone(response);
			}
		}
	}
	std::cout << RED << "Client <" << fd - 3 << "> Disconnected" << WHITE << std::endl;
	RemoveClient(fd);
	RemoveFd(fd);
	close(fd);
}

