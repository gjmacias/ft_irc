#include "Server.hpp"

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
	for (i = 0; i < tjois->_channels.size(); i++)
	{
		if (this->_channels[i].GetClientInChannel(nickname))
			count++;
	}
	return (count);
}

bool IsInvited(Client *cli, std::string ChName, int flag)
{
	if (cli->GetInviteChannel(ChName))
	{
		if (flag == 1)
			cli->RmChannelInvite(ChName);
		return (true);
	}
	return (false);
}

void	Server::ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->_channels[j].GetClientInChannel(GetClient(fd)->GetNickname()))
		return ;
	if (SearchForClients(GetClient(fd)->GetNickname()) >= 10)
	{
		SendError(405, GetClient(fd)->GetNickname(), GetClient(fd)->GetFd(), " :You have joined too many _channels\r\n");
		return ;
	}
	if (!this->_channels[j].GetPassword().empty() && this->_channels[j].GetPassword() != token[i].second)
	{
		if (!IsInvited(GetClient(fd), token[i].first, 0))
		{
			SendError(475, GetClient(fd)->GetNickname(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetInviteOnly())
	{
		if (!IsInvited(GetClient(fd), token[i].first, 1))
		{
			SendError(473, GetClient(fd)->GetNickname(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n");
			return ;
		}
	}
	if (this->_channels[j].GetLimit() && this->_channels[j].GetClientsNumber() >= this->_channels[j].GetLimit())
	{
		SendError(471, GetClient(fd)->GetNickname(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+1)\r\n");
		return ;
	}
	//add Client to the channel
	Client	*cli = GetClient(fd);
	this->_channels[j].add_client(*cli);
	if(_channels[j].GetTopicName().empty())
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),token[i].first) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	else
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),token[i].first) + \
			RPL_TOPICIS(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].GetTopicName()) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickname(),_channels[j].GetName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),_channels[j].GetName()),fd);
	_channels[j].sendTo_all(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first), fd);
}

void	Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(GetClient(fd)->GetNickname()) >= 10)
	{
		SendError(405, GetClient(fd)->GetNickname(), GetClient(fd)->GetFd(), " :You have joined too many _channels\r\n");
		return ;
	}
	Channel	newChannel;
	newChannel.SetName(token[i].first);
	newChannel.add_admin(*GetClient(fd));
	newChannel.set_createiontime();
	this->_channels.push_back(newChannel);
	//notify taht the client join the channel
	_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAdd(),newChannel.GetName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickname(),newChannel.GetName(),newChannel.clientChannel_list()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickname(),newChannel.GetName()),fd);
}

void	Server::JoinCommand(std::vector<std::string> &splited_cmd, int &fd)
{
    (void)splited_cmd;
	(void)fd;
	std::vector < std::pair < std::string, std::string > > token;
    if (token.size() > 10)
    {
        SendError(407, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), " :Too many _channels\r\n");
        return ;
    }
    for (size_t i = 0; i < token.size(); i++)
    {
        if (this->_channels[j].GetName() == token[i].first)
        {
            ExistCh(token, i, j, fd);
            flag = true;
            break;
        }
    }
    if (!flag)
        NotExistCh(token, i, fd);
}
