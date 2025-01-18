#include "Channel.hpp"

/*
###############################################################################
#									REMOVE									  #
###############################################################################
*/

void Channel::RemoveClient(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->GetFd() == fd)
		{
				clients.erase(it); 
				break;
		}
	}
}
void Channel::RemoveAdmin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if (it->GetFd() == fd)
		{
			admins.erase(it);
			break;
		}
	}
}