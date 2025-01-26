#include "Server.hpp"

/*
###############################################################################
#									REMOVERS								  #
###############################################################################
*/

void Server::RemoveFd(int fd)
{
	for (size_t i = 0; i < this->_pollSocketFds.size(); i++)
	{
		if (this->_pollSocketFds[i].fd == fd)
		{
			this->_pollSocketFds.erase(this->_pollSocketFds.begin() + i); 
			return;
		}
	}
}

void Server::RemoveClient(int fd)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].GetFd() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			return;
		}
	}
}

void	Server::RemoveClientFromChannels(int fd)
{
	std::string	response;

	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		int flag = 0;
		if (this->_channels[i].GetClient(fd))
		{
			this->_channels[i].RemoveClient(fd);
			flag = 1;
		}
		else if (this->_channels[i].GetAdmin(fd))
		{
			this->_channels[i].RemoveAdmin(fd);
			flag = 1;
		}
		if (this->_channels[i].CountAllClients() == 0)
		{
			this->_channels.erase(this->_channels.begin() + i);
			i--;
			continue;
		}
		if (flag)
		{
			response = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT Quit\r\n";
			this->_channels[i].SendEveryone(response);
		}
	}
}

void Server::RemoveChannel(std::string name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].GetName() == name)
		{
			this->_channels.erase(this->_channels.begin() + i); 
			return;
		}
	}
}
