#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									ADDERS									  #
###############################################################################
*/
void Server::AddClient(int clientFd, std::string IPAddress)
{
	Client* newClient = new Client();

	newClient->SetFd(clientFd);
	newClient->SetIPaddress(IPAddress);
	_clients.push_back(newClient);
}

void Server::AddChannel(Channel& newChannel) { this->_channels.push_back(newChannel); }

