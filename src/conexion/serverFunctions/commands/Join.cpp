#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									Join									  #
###############################################################################
*/

int	Server::SearchForClients(std::string nickname)
{
	int		count;
	size_t	i;

	count = 0;
	for (i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i].IsClientInChannel(nickname))
			count++;
	}
	return (count);
}

bool Server::IsInvited(Client *client, std::string channelname, int flag)
{
	if (client->ImInChannel(channelname))
	{
		if (flag == 1)
			client->RmFromMyChannels(channelname);
		return (true);
	}
	return (false);
}

void	Server::ExistCh(std::string &channelname, std::string &password, int j, int fd)
{
	if (this->_channels[j].IsClientInChannel(GetClient(fd)->GetNickname()))
		return ;
	if (this->_channels[j].GetModesChannelKey() && this->_channels[j].GetPassword() != password)
	{
		if (!IsInvited(GetClient(fd), channelname, 0))
		{
			SendErrorV2(475, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), '#' + channelname, " :Cannot join channel (+k) - bad key\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetModesInvitOnly())
	{
		if (!IsInvited(GetClient(fd), channelname, 1))
		{
			SendErrorV2(473, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), '#' + channelname, " :Cannot join channel (+i)\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetModesLimit() && this->_channels[j].CountAllClients() >= this->_channels[j].GetModesLimitNumber())
	{
		SendErrorV2(471, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), '#' + channelname, " :Cannot join channel (+1)\r\n");
		return ;
	}
	//add Client to the channel
	Client	*Client = GetClient(fd);
	this->_channels[j].AddClient(Client);
	if(_channels[j].GetTopicName().empty())
		SendResponse(RPL_JOINMSG(GetClient(fd)->GetHostname(), GetClient(fd)->GetIPaddress(),channelname) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	else
		SendResponse(RPL_JOINMSG(GetClient(fd)->GetHostname(),GetClient(fd)->GetIPaddress(),channelname) + \
			RPL_TOPICIS(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].GetTopicName()) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].ListOfClients()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	_channels[j].SendMeToAll(fd, RPL_JOINMSG(GetClient(fd)->GetHostname(), GetClient(fd)->GetIPaddress(), channelname));
}

void	Server::NotExistCh(std::string channelname, std::string password, int fd)
{

	Channel	newChannel;

	if (!channelname.empty() && channelname[0] == '#')
		channelname.erase(channelname.begin());
	if (!IsValidChannelname(channelname))
		return ;
	newChannel.SetName(channelname);
	newChannel.AddAdmin(GetClient(fd));
	if (!password.empty())
	{
		newChannel.SetModesChannelKey(true);
		newChannel.SetPassword(password);
	}
	this->_channels.push_back(newChannel);
	//notify taht the clientent join the channel
	SendResponse(RPL_JOINMSG(GetClient(fd)->GetHostname(),GetClient(fd)->GetIPaddress(),newChannel.GetName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickname(),newChannel.GetName(),newChannel.ListOfClients()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),newChannel.GetName()),fd);
}

void	Server::JoinCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	bool						flag = false;
	std::vector<std::string>	list_channels;
	std::vector<std::string>	list_passwords;
	std::string					tmp = "";

	if (splited_cmd.size() < 2)
	{
		ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname());
		return ;
	}
	list_channels = split_delimeter(splited_cmd[1], ',');
	if (splited_cmd.size() >= 3)
		list_passwords = split_delimeter(splited_cmd[2], ',');
    if (list_channels.size() > 10)
    {
        SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Too many _channels\r\n");
        return ;
    }
    for (size_t i = 0; i < list_channels.size(); i++)
    {
		if (i < list_passwords.size())
			tmp = list_passwords[i];
		else
			tmp = "";
		for (size_t j = 0; j < this->_channels.size(); j++)
		{
			if (this->_channels[j].GetName() == list_channels[i])
			{
				ExistCh(list_channels[i], tmp, j, fd);
				flag = true;
				break;
			}
		}
		if (!flag)
			NotExistCh(list_channels[i], tmp, fd);
    }
}

