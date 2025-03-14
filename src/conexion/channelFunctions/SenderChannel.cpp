#include "../../../includes/conexion/Channel.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/
void Channel::SendEveryone(std::string message)
{
	for(size_t i = 0; i < this->_admins.size(); i++)
		SendResponse(message, this->_admins[i]->GetFd());
	for(size_t i = 0; i < this->_clients.size(); i++)
		SendResponse(message, this->_clients[i]->GetFd());
}

void Channel::SendMeToAll(int fd, std::string message)
{
	for(size_t i = 0; i < this->_admins.size(); i++)
	{
		if (this->_admins[i]->GetFd() != fd)
			SendResponse(message, this->_admins[i]->GetFd());
	}
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->GetFd() != fd)
			SendResponse(message, this->_clients[i]->GetFd());
	}
}

