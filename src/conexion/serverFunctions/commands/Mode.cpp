#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									MODE									  #
###############################################################################
*/

void	Server::ModeCommand(std::vector<std::string> &splited_cmd, int fd)
{
	std::vector<std::pair<char, bool> >	list_modes;
	std::vector<std::string>			list_parameters;
	Channel								*channel;
	size_t								i = 0;

	if (splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return;
	}
	if (!(channel = GetChannel(splited_cmd[1])))
	{
		SendResponse(ERR_CHANNELNOTFOUND((this->GetClient(fd))->GetNickname(), splited_cmd[1]), fd);
		return ;
	}
	else if (!channel->GetAdmin(fd) && !channel->GetClient(fd))
	{
		SendErrorV2(442, fd, (this->GetClient(fd))->GetNickname(), channel->GetName(), " :You're not on that channel\r\n");
		return ;
	}
	else if (splited_cmd.size() == 2) // response with the channel modes (MODE #channel)
	{
		SendResponse(RPL_CHANNELMODES((this->GetClient(fd))->GetNickname(), channel->GetName(), channel->GetModes()) + \
		RPL_CREATIONTIME((this->GetClient(fd))->GetNickname(), channel->GetName(),channel->GetModesTimeCreation()),fd);
		return ;
	}
	else if (!channel->GetAdmin(fd)) // client is not channel operator
	{
		SendResponse(ERR_NOTOPERATOR(channel->GetName()), fd);
		return ;
	}
	for (i = 2; i < splited_cmd.size(); i++)
	{
		if (splited_cmd[i][0] == '-' || splited_cmd[i][0] == '+')
			OrderModes(list_modes, splited_cmd[i]);
		else
			list_parameters.push_back(splited_cmd[i]);
	}
	ModeExecute(channel, list_modes, list_parameters, fd);
}

void	Server::ModeExecute(Channel* channel, std::vector<std::pair<char, bool> > list_modes, std::vector<std::string> list_parameters, int fd)
{
	size_t						i = 0;
	size_t						j = 0;
	std::vector<std::string>	response;
	std::string					parameter;
	bool						flag_response = true;

	for (i = 0; i < list_modes.size(); i++)
	{
		if (j < list_parameters.size())
			parameter = list_parameters[j];
		else
			parameter = "";
		if (list_modes[i].first == 'i')//invite mode
			flag_response = ModeInviteOnly(channel, list_modes[i].second);
		else if (list_modes[i].first == 't') //topic restriction mode
			flag_response = ModeTopicRestriction(channel, list_modes[i].second);
		else if (list_modes[i].first == 'k') //password set/remove
		{
			flag_response = ModeChannelKey(channel, list_modes[i].second, parameter, fd);
			j++;
		}
		else if (list_modes[i].first == 'o') //set/remove user operator privilege
		{
			flag_response = ModeOperatorPrivilege(channel, list_modes[i].second, parameter, fd);
			j++;
		}
		else if (list_modes[i].first == 'l') //set/remove channel limits
		{
			flag_response = ModeLimit(channel, list_modes[i].second, parameter, fd);
			j++;
		}
		else
		{
			SendResponse(ERR_UNKNOWNMODE((this->GetClient(fd))->GetNickname(), channel->GetName(), list_modes[i].first), fd);
			flag_response = false;
		}
		if (flag_response)
			PrepareModeResponse(response, list_modes[i], parameter);
	}
	SendModeResponse(response, channel, fd);
}


/*
###############################################################################
#									5 MODES									  #
###############################################################################
*/

bool	Server::ModeInviteOnly(Channel *channel, bool flag)
{
	channel->SetModesInvitOnly(flag);
	return true;
}

bool	Server::ModeTopicRestriction(Channel *channel, bool flag)
{
	channel->SetModesTopicRestriction(flag);
	return true;
}

bool	Server::ModeChannelKey(Channel* channel, bool flag, std::string & password, int fd)
{

	if(flag)
	{
		if (!IsPasswordValid(password))
		{
			SendResponse(ERR_INVALIDMODEPARM(channel->GetName(), std::string("(k)")), fd);
			return false;
		}
		channel->SetPassword(password);
	}
	else 
		channel->SetPassword("");
	channel->SetModesChannelKey(flag);
	return true;
}

bool	Server::ModeOperatorPrivilege(Channel* channel, bool flag, std::string& user, int fd)
{
	if(!channel->IsClientInChannel(user))
	{
		SendResponse(ERR_NOSUCHNICK(channel->GetName(), user), fd);
		return false;
	}
	channel->SetModesOperatorPrivilege(flag);
	if (flag)
		return channel->ChangeClientToAdmin(user);
	else
		return channel->ChangeAdminToClient(user);
}

bool	Server::ModeLimit(Channel* channel, bool flag, std::string& limit, int fd)
{
	if (flag)
	{
		if (!IsLimitValid(limit))
		{
			SendResponse(ERR_INVALIDMODEPARM(channel->GetName(), "(l)"), fd);
			return false;
		}
		channel->SetLimitNumber(std::atoi(limit.c_str()));
	}
	else
		channel->SetLimitNumber(0);
	channel->SetModesLimit(flag);
	return true;
}

/*
###############################################################################
#									FUNCTIONS								  #
###############################################################################
*/

void	Server::PrepareModeResponse(std::vector<std::string> &response, std::pair<char, bool> mode, std::string parameter)
{
	std::string	result;

	if (mode.second)
		result.push_back('+');
	else
		result.push_back('-');
	result.push_back(mode.first);
	response.push_back(result);
	if (!(parameter.empty()))
		response.push_back(parameter);
}
void	Server::SendModeResponse(std::vector<std::string> response, Channel* channel, int fd)
{
	std::string	result;
	size_t		i = 0;

	if (response.size() == 0)
		return;
	while (i < response.size())
	{
		result += response[i];
		if (++i < response.size())
			result += " ";
	}
	channel->SendEveryone(RPL_CHANGEMODE((this->GetClient(fd))->GetHostname(), channel->GetName(), result));
}

void	Server::OrderModes(std::vector<std::pair<char, bool> > &list_modes, std::string arraymodes)
{
	size_t	i = 0;
	bool	flag = false;

	while (i < arraymodes.size())
	{
		if (arraymodes[i] == '+')
			flag = true;
		else if (arraymodes[i] == '-')
			flag = false;
		else if (arraymodes[i] == 'i' || arraymodes[i] == 't' || arraymodes[i] == 'k' ||
					arraymodes[i] == 'o' || arraymodes[i] == 'l')
			list_modes.push_back(std::make_pair(arraymodes[i], flag));
		else
			list_modes.push_back(std::make_pair('!', false));
		i++;
	}
}
