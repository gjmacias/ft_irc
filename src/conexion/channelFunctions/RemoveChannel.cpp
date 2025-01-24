#include "Channel.hpp"

/*
###############################################################################
#									REMOVE									  #
###############################################################################
*/

void Channel::RemoveClient(int fd)
{
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
		{
				this->_clients.erase(it); 
				break;
		}
	}
}
void Channel::RemoveAdmin(int fd)
{
	for (std::vector<Client *>::iterator it = this->_admins.begin(); it != this->_admins.end(); ++it)
	{
		if ((*it)->GetFd() == fd)
		{
			this->_admins.erase(it);
			break;
		}
	}
}