#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#								VALIDATION									  #
###############################################################################
*/
bool	Server::IsRegisteredAndLoged(int fd)
{
	if (GetClient(fd) && GetClient(fd)->GetIsRegistered() && !(GetClient(fd)->GetNickname().empty()) 
		&& !(GetClient(fd)->GetUsername().empty()) && GetClient(fd)->GetIsLogedInServer())
		return true;
	return false;
}

bool	Server::IsOnlyRegistered(Client *client)
{
	if (client->GetIsRegistered() && !(client->GetUsername().empty()) && 
		!(client->GetNickname().empty()) && !(client->GetIsLogedInServer()))
		return true;
	return false;
}

bool Server::IsValidUsername(std::string& nickname)
{
	int	i = 0;

	if (nickname.empty() || std::isdigit(nickname[0]))
		return false;
	while (i < nickname.size())
	{
		if(!(std::isalnum(nickname[i])) && nickname[i] != ' ' && nickname[i] != '_' && nickname[i] != '-')
			return false;
		i++;
	}
	if (i > 20 || i < 3)
		return false;
	return true;
}

bool Server::IsValidNickname(std::string& nickname)
{
	int	i = 0;

	if (nickname.empty() || std::isdigit(nickname[0]))
		return false;
	while (i < nickname.size())
	{
		if(!(std::isalnum(nickname[i])) && nickname[i] != '_' && nickname[i] != '-')
			return false;
		i++;
	}
	if (i > 15 || i < 3)
		return false;
	return true;
}

bool Server::IsNickNameInUse(std::string& nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (UpperCase(this->_clients[i].GetNickname()) == UpperCase(nickname))
			return true;
	}
	return false;
}

