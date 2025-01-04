#include "Client.hpp"

Client::Client(){}

int		Client::GetFd()
{
	return _fd;
}

void	Client::SetFd(int fd)
{
	_fd = fd;
}

void	Client::setIpAdd(std::string ipadd)
{
	_IPadd = ipadd;
}