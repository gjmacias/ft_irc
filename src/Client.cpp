#include "Client.hpp"

Client::Client(){}

int		Client::GetFd()
{
	return _Fd;
}

void	Client::SetFd(int fd)
{
	_Fd = fd;
}

void	Client::setIpAdd(std::string ipadd)
{
	_IPadd = ipadd;
}