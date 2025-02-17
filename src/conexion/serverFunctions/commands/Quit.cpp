#include "Server.hpp"

/*
###############################################################################
#									Quit									  #
###############################################################################
*/

void	Server::QuitCommand(std::vector<std::string> &splited_cmd, int &fd, std::string &name)
{
	std::string	rpl;
	size_t		i;

	std::string joined_cmd = "";
	for (size_t j = 0; j < splited_cmd.size(); j++)
	{
		joined_cmd += splited_cmd[j];
		if (j != splited_cmd.size() - 1) // Agregar espacios entre palabras
			joined_cmd += " ";
	}
	for (i = 0; i <_channels.size(); i++)
	{
		if (_channels[i].GetClient(fd))
		{
			_channels[i].RemoveClient(fd);
			if (_channels[i].CountAllClients() == 0)
				_channels.erase(_channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickname() + "!~" + GetClient(fd)->GetUsername() + "@localhost QUIT " + joined_cmd + "\r\n";
				_channels[i].SendEveryone(rpl);
			}
		}
		else if (_channels[i].GetAdmin(fd))
		{
			_channels[i].RemoveAdmin(fd);
			if (_channels[i].CountAllClients() == 0) //Buscar equivalente
				_channels.erase(_channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickname() + "!~" + GetClient(fd)->GetUsername() + "@localhost QUIT " + joined_cmd +"\r\n";
				_channels[i].SendEveryone(rpl);
			}
		}
	}
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	RemoveChannel(name);
	RemoveClient(fd);
	RemoveFd(fd);
	close(fd);
}

