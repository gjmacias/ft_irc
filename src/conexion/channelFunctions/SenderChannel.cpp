#include "Server.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/
void Channel::SendEveryone(std::string message)
{
	for(size_t i = 0; i < this->_admins.size(); i++)
	{
		if(send(this->_admins[i]->GetFd(), message.c_str(), message.size(), 0) == -1)
			std::cerr << "send() faild" << std::endl;
	}
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if(send(this->_clients[i]->GetFd(), message.c_str(), message.size(), 0) == -1)
			std::cerr << "send() faild" << std::endl;
	}
}

void Channel::SendMeToAll(int fd, std::string message)
{
	for(size_t i = 0; i < this->_admins.size(); i++)
	{
		if(this->_admins[i]->GetFd() != fd)
		{
			if(send(this->_admins[i]->GetFd(), message.c_str(), message.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
		}
	}
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if(this->_clients[i]->GetFd() != fd)
		{
			if(send(this->_clients[i]->GetFd(), message.c_str(), message.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
		}
	}
}

