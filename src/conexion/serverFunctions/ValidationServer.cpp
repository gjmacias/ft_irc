#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/
bool	Server::IsRegisteredAndLoged(int fd)
{
	if (GetClient(fd) && GetClient(fd)->GetIsRegistered() && !(GetClient(fd)->GetNickname().empty()) 
		&& !(GetClient(fd)->GetUsername().empty()) && GetClient(fd)->GetNickname() != "*"
		&& GetClient(fd)->GetIsLogedInServer())
		return true;
	return false;
}

bool	Server::IsOnlyRegistered(Client *client)
{
	if (client && client->GetIsRegistered() && !(client->GetUsername().empty()) && 
		!(client->GetNickname().empty()) && client->GetNickname() != "*" && !(client->GetIsLogedInServer()))
		return true;
	return false;
}

bool Server::IsValidNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!(std::isalnum(nickname[i])) && nickname[i] != '_')
			return false;
	}
	return true;
}

bool Server::IsNickNameInUse(std::string& nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].GetNickname() == nickname)
			return true;
	}
	return false;
}

