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

bool Server::IsValidUsername(std::string &nickname)
{
    long unsigned int	i = 0;
	std::string			vsc = "áéíóúÁÉÍÓÚàèìòùÀÈÌÒÙâêîôûÂÊÎÔÛäëïöüÄËÏÖÜñÑçÇ";
	char				c;

    if (nickname.empty() || std::isdigit(nickname[0]))
        return false;
    while (i < nickname.size())
    {
        c = nickname[i];

        if (!(std::isalnum(c)) && c != ' ' && c != '_' && c != '-' && vsc.find(c) == std::string::npos)
            return false;
        i++;
    }
    if (i > 30 || i < 3)
        return false;
    return true;
}

bool Server::IsValidChannelname(std::string &channelname)
{
	long unsigned int	i = 0;

	if (channelname.empty() || channelname[i] != '#')
		return false;
	i++;
	while (i < channelname.size())
	{
		if(!(std::isalnum(channelname[i])) && channelname[i] != ' ' && channelname[i] != '_'
				&& channelname[i] != '-' && channelname[i] != '.')
			return false;
		i++;
	}
	if (i > 200 || i < 3)
		return false;
	return true;
}

bool Server::IsValidNickname(std::string &nickname)
{
	long unsigned int	i = 0;

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

bool Server::IsValidPassword(std::string password)
{
	std::string allowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*-_+=.?";

	if (password.empty() || password[0] == ':')
		return false;
	for (size_t i = 0; i < password.size(); i++)
	{
		if (allowedCharacters.find(password[i]) == std::string::npos)
			return false;
	}
	return true;
}

bool Server::IsLimitValid(std::string &limit)
{
	return (!(limit.find_first_not_of("0123456789") != std::string::npos) && std::atoi(limit.c_str()) > 0);
}

bool Server::IsNickNameInUse(std::string &nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (UpperCase(this->_clients[i]->GetNickname()) == UpperCase(nickname))
			return true;
	}
	return false;
}

