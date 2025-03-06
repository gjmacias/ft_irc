#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#								CLIENT INVOLVED								  #
###############################################################################
*/

void Server::ClientAuthentification(std::string cmd, int fd)
{
	Client						*client = GetClient(fd);
	size_t						position;
	std::vector<std::string>	splited_cmd;


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
	else
	{
		splited_cmd = split_cmd(cmd);
		if (splited_cmd.size() != 0)
			cmd = splited_cmd[0];
	}

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

void	Server::ClientUsername(std::vector<std::string> &splited_cmd, std::string cmd, int fd)
{
    Client		*client = GetClient(fd);
	std::string	response = client->GetNickname();
	size_t		position = 0;
	size_t		i = 0;

	if (response.empty())
		response = "*";
	if (!(client->GetIsRegistered()))
    {
        SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
        return;
    }
	if (!client->GetUsername().empty())
	{
		SendResponse(ERR_ALREADYREGISTERED(response), fd);
		return;
	}

	if(splited_cmd.size() < 5)
	{
		SendResponse(ERR_NOTENOUGHPARAM(response), fd);
		return; 
	}

    // Si el username empieza con ':', eliminarlo
    if (splited_cmd[4][0] != ':')
		cmd = splited_cmd[4];
	else
	{
		while (i < 4)
		{
			position = cmd.find(splited_cmd[i]);
			cmd = cmd.substr(position + splited_cmd[i].size());
			position = cmd.find_first_not_of("\t\v ");
			cmd = cmd.substr(position);
			i++;
		}
		cmd.erase(cmd.begin());
	}
	if (!IsValidUsername(cmd))
    {
        SendResponse(ERR_ERRONEUSUSERNAME(response), fd);
        return;
    }
    client->SetUsername(cmd);  

    if (IsOnlyRegistered(client))
    {
        client->SetIsLogedInServer(true);
        SendResponse(RPL_CONNECTED(client->GetUsername()), fd);
    }
}

void	Server::ClientNickname(std::vector<std::string> &splited_cmd, int fd)
{
	Client		*client = GetClient(fd); 
	std::string	oldnick = client->GetNickname();
	std::string	response = client->GetNickname();

	if (response.empty())
		response = "*";
    if (!(client->GetIsRegistered()))
    {
        SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
        return;
    }
	// Verificar si el comando tiene suficientes parámetros (mínimo 2: "NICK <Nickname>")
	if(splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(response), fd);
		return; 
	}
	if (IsNickNameInUse(splited_cmd[1]))
    {
        SendResponse(ERR_NICKINUSE(response), fd);
        return;
    }
	if (!IsValidNickname(splited_cmd[1]))
    {
        SendResponse(ERR_ERRONEUSNICK(response), fd);
        return;
    }
	if (splited_cmd[1] == oldnick)
		return ;
	// Asignar el nuevo nombre de usuario
	client->SetNickname(splited_cmd[1]);

	if (!(oldnick.empty()))
    	SendResponse(RPL_NICKCHANGE(oldnick, splited_cmd[1]), fd);

	if(IsOnlyRegistered(client))
	{
		client->SetIsLogedInServer(true);
		SendResponse(RPL_CONNECTED(client->GetNickname()), fd);
	}
}

