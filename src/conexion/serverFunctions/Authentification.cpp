#include "Server.hpp"

/*
###############################################################################
#							AUTHENTIFICATION								  #
###############################################################################
*/

void Server::ClientAuthentification(int fd, std::string cmd)
{
	Client		*client = GetClient(fd);
	size_t		position;
	std::string	password;

	cmd = cmd.substr(4);
	position = cmd.find_first_not_of("\t\v ");
	if(position < cmd.size())
	{
		cmd = cmd.substr(position);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	if(cmd.empty() || position == std::string::npos) 
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if(!client->getRegistered())
	{
		password = cmd;
		if(password == this->_password)
			client->setRegistered(true);
		else
			_sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		_sendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->GetNickName()), fd);
}