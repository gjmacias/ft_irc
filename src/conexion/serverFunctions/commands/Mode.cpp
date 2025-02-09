#include "Server.hpp"

/*
###############################################################################
#									MODE									  #
###############################################################################
*/

void	Server::ModeCommand(std::string &cmd, int &fd)
{
	std::stringstream	modeChain;
	std::string			channelName;
	std::string			parameters;
	std::string			modeset;
	std::string			arguments;
	Channel				*channel;
	Client				*cli = GetClient(fd);
	char				opera = '\0';
	size_t				found = cmd.find_first_not_of("MODEmode \t\v");

	modeChain.clear();
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	else
	{
		SendResponse(ERR_NOTENOUGHPARAM(cli->GetNickname()), fd); 
		return ;
	}



	GetCmdArguments(cmd ,channelName, modeset ,parameters);
	std::vector<std::string> tokens = SplitParameters(parameters);



	if(channelName[0] != '#' || !(channel = GetChannel(channelName.substr(1))))
	{
		SendResponse(ERR_CHANNELNOTFOUND(cli->GetUsername(),channelName), fd);
		return ;
	}
	else if (!channel->GetClient(fd) && !channel->GetAdmin(fd))
	{
		SendErrorV2(442, fd, cli->GetNickname(), channel->GetName(), " :You're not on that channel\r\n");
		return ;
	}
	else if (modeset.empty()) // response with the channel modes (MODE #channel)
	{
		SendResponse(RPL_CHANNELMODES(cli->GetNickname(), channel->GetName(), channel->GetModes()) + \
		RPL_CREATIONTIME(cli->GetNickname(), channel->GetName(),channel->GetModesTimeCreation()),fd);
		return ;
	}
	else if (!channel->GetAdmin(fd)) // client is not channel operator
	{
		SendResponse(ERR_NOTOPERATOR(channel->GetName()), fd);
		return ;
	}
	else if(channel)
	{
		size_t pos = 0;
		for(size_t i = 0; i < modeset.size(); i++)
		{
			if(modeset[i] == '+' || modeset[i] == '-')
				opera = modeset[i];
			else
			{
				if(modeset[i] == 'i')//invite mode
					modeChain << ModeInviteOnly(channel , opera, modeChain.str());
				else if (modeset[i] == 't') //topic restriction mode
					modeChain << ModeTopicRestriction(channel, opera, modeChain.str());
				else if (modeset[i] == 'k') //password set/remove
					modeChain <<  ModeChannelKey(tokens, channel, pos, opera, fd, modeChain.str(), arguments);
				else if (modeset[i] == 'o') //set/remove user operator privilege
						modeChain << ModeOperatorPrivilege(tokens, channel, pos, fd, opera, modeChain.str(), arguments);
				else if (modeset[i] == 'l') //set/remove channel limits
					modeChain << ModeLimit(tokens, channel, pos, opera, fd, modeChain.str(), arguments);
				else
					SendResponse(ERR_UNKNOWNMODE(cli->GetNickname(), channel->GetName(),modeset[i]),fd);
			}
		}
	}
	std::string chain = modeChain.str();
	if(chain.empty())
		return ;
 	channel->SendEveryone(RPL_CHANGEMODE(cli->GetHostname(), channel->GetName(), modeChain.str(), arguments));
}

/*
###############################################################################
#									5 MODES									  #
###############################################################################
*/

std::string Server::ModeInviteOnly(Channel *channel, char opera, std::string chain)
{
	std::string param;
	if(opera == '+' && !channel->GetModesInvitOnly())
	{
		channel->SetModesInvitOnly(true);
		param =  ModeToAppend(chain, opera, 'i');
	}
	else if (opera == '-' && channel->GetModesInvitOnly())
	{
		channel->SetModesInvitOnly(false);
		param =  ModeToAppend(chain, opera, 'i');
	}
	return param;
}

std::string Server::ModeTopicRestriction(Channel *channel ,char opera, std::string chain)
{
	std::string param;
	if(opera == '+' && !channel->GetModesTopicRestriction())
	{
		channel->SetModesTopicRestriction(true);
		param =  ModeToAppend(chain, opera, 't');
	}
	else if (opera == '-' && channel->GetModesTopicRestriction())
	{
		channel->SetModesTopicRestriction(false);
		param =  ModeToAppend(chain, opera, 't');
	}	
	return param;
}

std::string Server::ModeChannelKey(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
	std::string param;
	std::string pass;

	if(tokens.size() > pos)
		pass = tokens[pos++];
	else
	{
		SendResponse(ERR_NEEDMODEPARM(channel->GetName(),std::string("(k)")),fd);
		return param;
	}
	if(!IsPasswordValid(pass))
	{
		SendResponse(ERR_INVALIDMODEPARM(channel->GetName(),std::string("(k)")),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->SetModesChannelKey(true);
		channel->SetPassword(pass);
		if(!arguments.empty())
			arguments += " ";
		arguments += pass;
		param = ModeToAppend(chain,opera, 'k');
	}
	else if (opera == '-' && channel->GetModesChannelKey())
	{
		if(pass == channel->GetPassword())
		{		
			channel->SetModesChannelKey(false);
			channel->SetPassword("");
			param = ModeToAppend(chain,opera, 'k');
		}
		else
			SendResponse(ERR_KEYSET(channel->GetName()),fd);
	}
	return param;
}

std::string Server::ModeOperatorPrivilege(std::vector<std::string> tokens, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments)
{
	std::string user;
	std::string param;

	if(tokens.size() > pos)
		user = tokens[pos++];
	else
	{
		SendResponse(ERR_NEEDMODEPARM(channel->GetName(),"(o)"),fd);
		return param;
	}
	if(!channel->IsClientInChannel(user))
	{
		SendResponse(ERR_NOSUCHNICK(channel->GetName(), user),fd);
		return param;
	}
	if(opera == '+')
	{
		channel->SetModesOperatorPrivilege(true);
		if(channel->ChangeClientToAdmin(user))
		{
			param = ModeToAppend(chain, opera, 'o');
			if(!arguments.empty())
				arguments += " ";
			arguments += user;
		}
	}
	else if (opera == '-')
	{
		channel->SetModesOperatorPrivilege(false);
		if(channel->ChangeAdminToClient(user))
		{
			param = ModeToAppend(chain, opera, 'o');
				if(!arguments.empty())
					arguments += " ";
			arguments += user;
		}
	}
	return param;
}

std::string Server::ModeLimit(std::vector<std::string> tokens,  Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments)
{
	std::string limit;
	std::string param;

	if(opera == '+')
	{
		if(tokens.size() > pos)
		{
			limit = tokens[pos++];
			if(!IsLimitValid(limit))
			{
				SendResponse(ERR_INVALIDMODEPARM(channel->GetName(),"(l)"), fd);
			}
			else
			{
				channel->SetModesLimit(true);
				channel->SetModesLimitNumber(std::atoi(limit.c_str()));
				if(!arguments.empty())
					arguments += " ";
				arguments += limit;
				param =  ModeToAppend(chain, opera, 'l');
			}
		}
		else
			SendResponse(ERR_NEEDMODEPARM(channel->GetName(),"(l)"),fd);
	}
	else if (opera == '-' && channel->GetModesLimit())
	{
		channel->SetModesLimit(false);
		channel->SetModesLimitNumber(0);
		param  = ModeToAppend(chain, opera, 'l');
	}
	return param;
}

/*
###############################################################################
#									FUNCTIONS								  #
###############################################################################
*/

std::string Server::ModeToAppend(std::string chain, char opera, char mode)
{
	std::stringstream ss;
	char last = '\0';

	for(size_t i = 0; i < chain.size(); i++)
	{
		if(chain[i] == '+' || chain[i] == '-')
			last = chain[i];
	}
	if(last != opera)
		ss << opera << mode;
	else
		ss << mode;
	return ss.str();
}

void Server::GetCmdArguments(std::string cmd,std::string& name, std::string& modeset ,std::string &params)
{
	std::istringstream stm(cmd);
	stm >> name;
	stm >> modeset;
	size_t found = cmd.find_first_not_of(name + modeset + " \t\v");
	if(found != std::string::npos)
		params = cmd.substr(found);
}


std::vector<std::string> Server::SplitParameters(std::string parameters)
{
	std::vector<std::string>	tokens;
	std::string					argument;
	std::istringstream			stm(parameters);

	if(!parameters.empty() && parameters[0] == ':')
		parameters.erase(parameters.begin());
	while (std::getline(stm, argument, ','))
	{
		tokens.push_back(argument);
		argument.clear();
	}
	return tokens;
}

bool Server::IsPasswordValid(std::string password)
{
	if(password.empty())
		return false;
	for(size_t i = 0; i < password.size(); i++)
	{
		if(!std::isalnum(password[i]) && password[i] != '_')
			return false;
	}
	return true;
}

bool Server::IsLimitValid(std::string& limit)
{
	return (!(limit.find_first_not_of("0123456789")!= std::string::npos) && std::atoi(limit.c_str()) > 0);
}
