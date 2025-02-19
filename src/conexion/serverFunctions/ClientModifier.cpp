#include "Server.hpp"


/*
###############################################################################
#								CLIENT INVOLVED								  #
###############################################################################
*/

void Server::ClientAuthentification(std::string cmd, int fd)
{
	Client		*client = GetClient(fd);
	size_t		position;

	// Verificar que el comando tenga suficientes caracteres
	if (cmd.length() <= 5) // "PASS " son al menos 5 caracteres
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}

	cmd = cmd.substr(5); // Eliminar "PASS "
	position = cmd.find_first_not_of("\t\v ");
	if (position == std::string::npos) // Si no hay más caracteres, es un error
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	
	cmd = cmd.substr(position); // Eliminar espacios iniciales

	// Verificación segura antes de acceder a cmd[0]
	if (!cmd.empty() && cmd[0] == ':')
		cmd.erase(cmd.begin());

	if (cmd.empty()) // Si después de todo, cmd está vacío, es un error
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}

	// Comprobar si el cliente ya está registrado
	if (!client->GetIsRegistered())
	{
		if (cmd == this->_password)
			client->SetIsRegistered(true);
		else
			SendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		SendResponse(ERR_ALREADYREGISTERED(client->GetNickname()), fd);
}

void Server::ClientNickname(std::string cmd, int fd)
{
	Client		*client = GetClient(fd);
	size_t		position;

	if (!(client->GetIsRegistered()))
	{
		SendResponse(ERR_NOTREGISTERED(cmd), fd);
		return ;
	}

	cmd = cmd.substr(4);
	position = cmd.find_first_not_of("\t\v ");
	if(position == std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	cmd = cmd.substr(position);
	position = cmd.find_first_not_of("\t\v ");
	if(cmd[0] == ':')
		cmd.erase(cmd.begin());	
	else if(position != std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}

	if (IsNickNameInUse(cmd) && client->GetNickname() != cmd)
	{
		if(client->GetNickname().empty())
			client->SetNickname("*");
	    SendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return;
	}
	if(!IsValidNickname(cmd))
	{
		SendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
	
	std::string oldnick = client->GetNickname();

	if (client->GetNickname().empty() || cmd == oldnick)
		return;
	else
	{
		client->SetNickname(cmd);
		SendResponse(RPL_NICKCHANGE(oldnick, cmd), fd);
	}	

	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickname()), fd);
	}
}

void	Server::ClientUsername(std::vector<std::string> &splited_cmd, int fd)
{
	Client *client = GetClient(fd); 

	if(client && splited_cmd.size() < 5)
	{
		SendResponse(ERR_NOTENOUGHPARAM(client->GetNickname()), fd);
		return; 
	}
	if(!(client) || !(client->GetIsRegistered()))
		SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (client && !(client->GetUsername().empty()))
	{
		SendResponse(ERR_ALREADYREGISTERED(client->GetNickname()), fd);
		return;
	}
	else
		client->SetUsername(splited_cmd[1]);

	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickname()), fd);
	}
}

