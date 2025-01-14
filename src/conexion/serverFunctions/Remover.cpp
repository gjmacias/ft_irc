#include "Server.hpp"

/*
###############################################################################
#									REMOVERS								  #
###############################################################################
*/

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
void Server::RemoveFd(int fd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == fd)
		{
			this->_fds.erase(this->_fds.begin() + i); 
			return;
		}
	}
}
void	Server::RemoveClientsFromChannels(int fd)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		int flag = 0;
		if (this->_channels[i].get_client(fd))
		{
			this->_channels[i].remove_client(fd);
			flag = 1;
		}
		else if (this->_channels[i].get_admin(fd))
		{
			this->_channels[i].remove_admin(fd);
			flag = 1;
		}
		if (this->_channels[i].GetClientsNumber() == 0)
		{
			this->_channels.erase(this->_channels.begin() + i);
			i--;
			continue;
		}
		if (flag)
		{
			// std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@localhost QUIT Quit\r\n";
			// this->_channels[i].sendTo_all(rpl);
		}
	}
}