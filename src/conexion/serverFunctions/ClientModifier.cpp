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

/*void Server::ClientNickname(std::string cmd, int fd)
{
	Client		*client = GetClient(fd);
	size_t		position;

	// Verificar que el comando tenga suficientes caracteres
	if (cmd.length() <= 5) // "NICK " son al menos 5 caracteres
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	cmd = cmd.substr(5);
	if (!(client->GetIsRegistered()))
	{
		SendResponse(ERR_NOTREGISTERED(cmd), fd);
		return ;
	}
	position = cmd.find_first_not_of("\t\v ");
	if(position == std::string::npos)
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	cmd = cmd.substr(position);
	//position = cmd.find_first_not_of("\t\v ");
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
}*/
void Server::ClientNickname(std::string cmd, int fd)
{
    Client *client = GetClient(fd);
    
    // Usar un stringstream para procesar el comando correctamente
    std::istringstream iss(cmd);
    std::string command, nickname;
    iss >> command >> nickname;
    
    // Verificar si falta el parámetro del nickname
    if (nickname.empty())
    {
        SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
        return;
    }
    
    if (!(client->GetIsRegistered()))
    {
        SendResponse(ERR_NOTREGISTERED(cmd), fd);
        return;
    }
    
    // Si el nickname empieza con ':', eliminarlo
    if (nickname[0] == ':')
        nickname.erase(nickname.begin());
    
    // Verificar si el nickname ya está en uso
    if (IsNickNameInUse(nickname) && client->GetNickname() != nickname)
    {
        if (client->GetNickname().empty())
            client->SetNickname("*");
        SendResponse(ERR_NICKINUSE(nickname), fd);
        return;
    }
    
    // Verificar si el nickname es válido
    if (!IsValidNickname(nickname))
    {
        SendResponse(ERR_ERRONEUSNICK(nickname), fd);
        return;
    }
    
    std::string oldnick = client->GetNickname();
    
    // Si el nick es el mismo o está vacío, no hacer nada
    if (oldnick.empty() || nickname == oldnick)
        return;
    
    // Asignar el nuevo nickname y enviar la respuesta correspondiente
    client->SetNickname(nickname);
    SendResponse(RPL_NICKCHANGE(oldnick, nickname), fd);
    
    if (IsOnlyRegistered(client))
    {
        client->SetIsLogedInServer(true);
        SendResponse(RPL_CONNECTED(client->GetNickname()), fd);
    }
}

void	Server::ClientUsername(std::vector<std::string> &splited_cmd, int fd)
{
	Client *client = GetClient(fd); 
	
	// Verificar si el cliente existe
    if (!client)
    {
        SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
        return;
    }
	// Verificar si el comando tiene suficientes parámetros (mínimo 2: "USER <username>")
	if(splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(client->GetNickname()), fd);
		return; 
	}
	// Verificar si el cliente ya tiene un nombre de usuario asignado
	if (!client->GetUsername().empty())
	{
		SendResponse(ERR_ALREADYREGISTERED(client->GetNickname()), fd);
		return;
	}
	// Asignar el nuevo nombre de usuario
	client->SetUsername(splited_cmd[1]);

	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickname()), fd);
	}
}

