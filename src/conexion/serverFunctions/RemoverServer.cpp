#include "../../../includes/conexion/Server.hpp"

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
	std::cerr << "Warning: Tried to remove fd " << fd << " but it was not found in _pollSocketFds." << std::endl;
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
	int			flag = 0;

	for (size_t i = 0; i < this->_channels.size(); i++)
	{
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
			Client *client = GetClient(fd);
    		if (client)
			{
        		response = ":" + client->GetNickname() + "!~" + client->GetUsername() + "@localhost QUIT Quit\r\n";
        		this->_channels[i].SendEveryone(response);
    		} 
			else
        		std::cerr << "Warning: Client fd " << fd << " was removed before sending QUIT message." << std::endl;
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

