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
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
		{
			delete* it;
			_clients.erase(it);
			return;
		}
	}
}

void Server::RemoveAllClients()
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete* it; 
	_clients.clear();
}

void	Server::RemoveClientFromChannels(int fd)
{
	std::string	response;
	int			flag = 0;
	Client *client = GetClient(fd);

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
		if (flag && client)
		{
			response = ":" + client->GetNickname() + "!" + client->GetUsername() + "@" + client->GetIPaddress() + " QUIT Quit\r\n";
			this->_channels[i].SendEveryone(response);
		} 
		else if (flag)
			std::cerr << "Warning: Client fd " << fd << " was removed before sending QUIT message." << std::endl;
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

