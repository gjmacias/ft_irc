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
	std::cout << RED << "Client <" << fd - 3 << "> Disconnected" << WHITE << std::endl;
	RemoveClientFromChannels(fd, cmd_reason);
	RemoveClient(fd);
	RemoveFd(fd);
	close(fd);
}

