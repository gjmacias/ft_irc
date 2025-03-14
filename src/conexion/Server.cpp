#include "../../includes/conexion/Server.hpp"

/*
###############################################################################
#									CONSTRUCTOR								  #
###############################################################################
*/
Server::Server()
{
	_mainSocketFd = -1;
}

Server::~Server(){}

Server::Server(Server const &src){*this = src;}

Server &Server::operator=(Server const &src)
{
	if (this != &src)
	{
		this->_port = src._port;
		this->_mainSocketFd = src._mainSocketFd;
		this->_password = src._password;
		this->_clients = src._clients;
		this->_pollSocketFds = src._pollSocketFds;
	}
	return *this;
}
/*
###############################################################################
#									SETTERS									  #
###############################################################################
*/
/*
###############################################################################
#									GETTERS									  #
###############################################################################
*/
int			Server::GetPort(){return this->_port;}
int			Server::GetMainFd(){return this->_mainSocketFd;}
std::string	Server::GetPassword(){return this->_password;}

Client	*Server::GetClient(int fd)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].GetFd() == fd)
			return &this->_clients[i];
	}
	return NULL;
}

Client	*Server::GetClient_Nickname(std::string nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].GetNickname() == nickname)
			return &this->_clients[i];
	}
	return NULL;
}

Channel *Server::GetChannel(std::string name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].GetName() == name)
			return &_channels[i];
	}
	return NULL;
}

/*
###############################################################################
#								FUNCTIONS									  #
###############################################################################
*/
bool Server::_signal = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_signal = true;
}

void Server::ServerInit(int port, std::string password)
{
	struct sockaddr_in	socketAddress;
	struct pollfd		newPoll;

	this->_port = port;
	this->_password = password;

	socketAddress.sin_family = AF_INET; 
	socketAddress.sin_port = htons(this->_port);
	socketAddress.sin_addr.s_addr = INADDR_ANY;

	_mainSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(_mainSocketFd == -1)
		throw(std::runtime_error("failed to create socket"));

	int enabled = 1;
	if(setsockopt(_mainSocketFd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) == -1) 
			throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_mainSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(_mainSocketFd, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) == -1)
		throw(std::runtime_error("failed to bind socket"));
	if (listen(_mainSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));

	newPoll.fd = _mainSocketFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	_pollSocketFds.push_back(newPoll);

	std::cout << GREEN << "Server <" << _mainSocketFd - 3 << "> Connected" << WHITE << std::endl;
	std::cout << "Waiting to accept a connection...\n";
}

void Server::ServerLoop()
{
	while (Server::_signal == false)
	{
		if((poll(&_pollSocketFds[0], _pollSocketFds.size(), -1) == -1) && Server::_signal == false)
			throw(std::runtime_error("poll() failed"));

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
	socklen_t			clientAddressLenght;
	int					clientFd;

	clientAddressLenght = sizeof(clientAddress);
	clientFd = accept(_mainSocketFd, (struct sockaddr *)&(clientAddress), &clientAddressLenght);
	if (clientFd == -1)
	{
		std::cerr << "accept() failed" << std::endl;
		return;
	}
	// Asegurarse de que el descriptor de archivo es válido
    if (clientFd <= 0)
    {
        std::cerr << "Invalid clientFd received: " << clientFd << std::endl;
        return;
    }

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl() failed to set non-blocking mode" << clientFd << std::endl;
		close(clientFd);
		return;
	}
	NewPoll.fd = clientFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	newClient.SetFd(clientFd);
	newClient.SetIPaddress(inet_ntoa(clientAddress.sin_addr));

	_clients.push_back(newClient);
	_pollSocketFds.push_back(NewPoll);

	std::cout << GREEN << "Client <" << clientFd - 3 << "> Connected" << WHITE << std::endl;

	std::string welcomeMsg = ":ircserv 001 " + newClient.GetIPaddress() + " :Welcome to MyIRC\r\n";
	SendResponse(welcomeMsg, clientFd);
}

void Server::ReceiveNewData(int fd)
{
	char						buffer[1024];
	std::vector<std::string>	cmd;
	ssize_t						bytes;
	Client						*client = GetClient(fd);

	if (client == NULL) 
	{
        std::cerr << "Client with fd " << fd - 3 << " not found!" << std::endl;
        return;  // O maneja el error de alguna forma
    }

	memset(buffer, 0, sizeof(buffer));
	bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);
	if(bytes == 0)
	{
		std::cout << RED << "Client <" << fd - 3 << "> Disconnected" << WHITE << std::endl;
		RemoveClientFromChannels(fd);
		RemoveClient(fd);
		RemoveFd(fd);
		close(fd);
		return;
	}
	else if (bytes < 0)
	{
		std::cout << RED << "Error receiving data from: <" << fd - 3 << ">" << WHITE << std::endl;
		return ;
	}	
	// Añadimos el contenido al buffer del cliente
	client->SetBuffer(buffer);
	if (client->GetBuffer().find_first_of("\r\n") == std::string::npos)
		return;
	cmd = split_recivedBuffer(client->GetBuffer());
	for(size_t i = 0; i < cmd.size(); i++)
		ParseAndExecute(cmd[i], fd);
	if (GetClient(fd)) 
		GetClient(fd)->ClearUsedBuffer(); 	
}

void Server::CloseFds()
{
	for(size_t i = 0; i < _clients.size(); i++)
	{ 
		std::cout << RED << "Client <" << _clients[i].GetFd() - 3 << "> Disconnected" << WHITE << std::endl;
		close(_clients[i].GetFd());
	}
	if (_mainSocketFd != -1)
	{
		std::cout << RED << "Server <" << _mainSocketFd - 3 << "> Disconnected" << WHITE << std::endl;
		close(_mainSocketFd);
	}
}

