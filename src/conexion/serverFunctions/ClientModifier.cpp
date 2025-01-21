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
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if(!client->GetRegistered())
	{
		password = cmd;
		if(password == this->_password)
			client->SetRegistered(true);
		else
			SendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		SendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->GetNickName()), fd);
}

void Server::ClientNickname(std::string cmd, int fd)
{
	std::string inuse;
	cmd = cmd.substr(4);
	size_t pos = cmd.find_first_not_of("\t\v ");
	if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	Client *cli = GetClient(fd);
	if(pos == std::string::npos || cmd.empty())
	{
		SendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	if (IsNickNameInUse(cmd) && cli->GetNickName() != cmd)
	{
		inuse = "*";
		if(cli->GetNickName().empty())
			cli->SetNickname(inuse);
	    SendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return;
	}
	if(!IsValidNickname(cmd))
	{
		SendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
	else
	{
		if(cli && cli->GetRegistered())
		{
			std::string oldnick = cli->GetNickName();
			cli->SetNickname(cmd);
			for(size_t i = 0; i < this->_channels.size(); i++){
				Client *cl = this->_channels[i].GetClientByNickname(oldnick);
				if(cl)
					cl->SetNickname(cmd);
			}
			if(!oldnick.empty() && oldnick != cmd)
			{
				if(oldnick == "*" && !cli->GetUserName().empty())
				{
					cli->setLogedin(true);
					SendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
					SendResponse(RPL_NICKCHANGE(cli->GetNickName(), cmd), fd);
				}
				else
					SendResponse(RPL_NICKCHANGE(oldnick,cmd), fd);
				return;
			}
			
		}
		else if (cli && !cli->getRegistered())
			SendResponse(ERR_NOTREGISTERED(cmd), fd);
	}
	if(cli && cli->getRegistered() && !cli->GetUserName().empty() && !cli->GetNickName().empty() && cli->GetNickName() != "*" && !cli->GetLogedIn())
	{
		cli->setLogedin(true);
		SendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
	}
}

void	Server::ClientUsername(std::string cmd, int fd)
{
	std::vector<std::string> splited_cmd = split_cmd(cmd);

	Client *cli = GetClient(fd); 
	if((cli && splited_cmd.size() < 5))
		{SendResponse(ERR_NOTENOUGHPARAM(cli->GetNickName()), fd); return; }
	if(!cli  || !cli->getRegistered())
		SendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (cli && !cli->GetUserName().empty())
		{SendResponse(ERR_ALREADYREGISTERED(cli->GetNickName()), fd); return;}
	else
		cli->SetUsername(splited_cmd[1]);
	if(cli && cli->getRegistered() && !cli->GetUserName().empty() && !cli->GetNickName().empty() && cli->GetNickName() != "*"  && !cli->GetLogedIn())
	{
		cli->setLogedin(true);
		SendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
	}
}