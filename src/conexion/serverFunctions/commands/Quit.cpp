#include "Server.hpp"

/*
###############################################################################
#									Quit									  #
###############################################################################
*/

void	Server::QuitCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	std::string	rpl;
	size_t		i;

	for (i = 0; i <channels.size(); i++)
	{
		if (channels[i].GetClient(fd))
		{
			channels[i].RemoveClient(fd);
			if (channels[i].CountAllClients() == 0)
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + splited_cmd + "\r\n";
				channels[i].SendEveryone(rpl);
			}
		}
		else if (channels[i].GetAdmin(fd))
		{
			channels[i].RemoveAdmin(fd);
			if (channels[i].CountAllClients() == 0) //Buscar equivalente
				channels.erase(channels.begin() + i);
			else
			{
				rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT " + splited_cmd +"\r\n";
				channels[i].SendEveryone(rpl);
			}
		}
	}
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
	RemoveChannel(fd);
	RemoveClient(fd);
	RemoveFd(fd);
	close(fd);
}
