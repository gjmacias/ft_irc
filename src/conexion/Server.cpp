#include "Colors.h"
#include "Server.hpp"
#include "Client.hpp"

bool Server::_signal = false;
	
Server::Server()
{
	_mainSocketFd = -1;
}

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "_signal Received!" << std::endl;
	Server::_signal = true;
}

void Server::ServerInit()
{
	struct sockaddr_in	socketAddress;
	struct pollfd		newPoll;

	this->Port = 4444;

	socketAddress.sin_family = AF_INET; 
	socketAddress.sin_port = htons(this->Port);
	socketAddress.sin_addr.s_addr = INADDR_ANY;

	_mainSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(_mainSocketFd == -1)
		throw(std::runtime_error("faild to create socket"));

	int enabled = 1;
	if(setsockopt(_mainSocketFd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) == -1) 
			throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_mainSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_mainSocketFd, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_mainSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));

	newPoll.fd = _mainSocketFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	_pollSocketFds.push_back(newPoll);

	std::cout << GREEN << "Server <" << _mainSocketFd << "> Connected" << WHITE << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	ServerLoop()
}

void Server::ServerLoop()
{
	while (Server::_signal == false)
	{
		if((poll(&_pollSocketFds[0], _pollSocketFds.size(), -1) == -1) && Server::_signal == false)
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < _pollSocketFds.size(); i++)
		{
			if (_pollSocketFds[i].revents & POLLIN)
			{
				if (_pollSocketFds[i].fd == _mainSocketFd)
					AcceptNewClient();
				else
					ReceiveNewData(_pollSocketFds[i].fd);
			}
		}
	}
	CloseFds();
}

void Server::AcceptNewClient()
{
	Client 				newClient;
	struct sockaddr_in	clientAddress;
	struct pollfd		NewPoll;


	int clientFd = accept(_mainSocketFd, (struct sockaddr *)&(clientAddress), sizeof(clientAddress));
	if (clientFd == -1)
		std::cout << "accept() failed" << std::endl; return;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		std::cout << "fcntl() failed" << std::endl; return;

	NewPoll.fd = clientFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	newClient.SetFd(clientFd);
	newClient.setIpAdd(inet_ntoa(clientAddress.sin_addr));
	_clients.push_back(newClient);
	_pollSocketFds.push_back(NewPoll);

	std::cout << GREEN << "Client <" << clientFd << "> Connected" << WHITE << std::endl;
}

void Server::ReceiveNewData(int fd)
{
	char	buffer[1024];
	ssize_t	bytes;

	memset(buffer, 0, sizeof(buffer));
	bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);

	if(bytes <= 0)
	{
		if (bytes == 0) 
			std::cout << RED << "Client <" << fd << "> Disconnected" << WHITE << std::endl;
		else 
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
			std::cout << RED << "Error receiving data from client <" << fd << ">: " << strerror(errno) << WHITE << std::endl;
		}
		ClearClients(fd); 
	}
	else
	{
		buffer[bytes] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> Data: " << WHITE << buffer;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}

void Server::CloseFds()
{
	for(size_t i = 0; i < _clients.size(); i++)
	{ 
		std::cout << RED << "Client <" << _clients[i].GetFd() << "> Disconnected" << WHITE << std::endl;
		close(_clients[i].GetFd());
	}
	if (_mainSocketFd != -1)
	{
		std::cout << RED << "Server <" << _mainSocketFd << "> Disconnected" << WHITE << std::endl;
		close(_mainSocketFd);
	}
}