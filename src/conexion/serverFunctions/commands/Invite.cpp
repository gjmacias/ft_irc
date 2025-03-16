#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									INVITE									  #
###############################################################################
Esta función Server::InviteCommand implementa la lógica para invitar
a un usuario a un canal en un servidor IRC. Maneja múltiples
errores potenciales y envía respuestas adecuadas al cliente 
en función de las condiciones detectadas.
*/


void	Server::InviteCommand(std::vector<std::string> &splited_cmd, int fd)
{
	Client		*user;
	Channel 	*channel;

	if (splited_cmd.size() != 3)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return;
	}
	if ( !(user = GetClient_Nickname(splited_cmd[1])))
	{
		SendError(401, fd, splited_cmd[1], " :No such Nick\r\n");
		return;
	}
	if (!(channel = GetChannel(splited_cmd[2])))
	{
		SendResponse(ERR_CHANNELNOTFOUND(GetClient(fd)->GetNickname(), splited_cmd[2]), fd);
		return;
	}
	if (!(channel->GetClient(GetClient(fd)->GetFd())) && !(channel->GetAdmin(GetClient(fd)->GetFd())))
	{
		SendError(442, fd, channel->GetName(), " :You're not on that channel\r\n");
		return ;
	}
	if (channel->GetModesInvitOnly() && !(channel->GetAdmin(fd)))
	{
		SendError(482, fd, GetClient(fd)->GetNickname(), " :You're not channel operator\r\n");
		return;
	}
	if (channel->IsClientInChannel(user->GetNickname()))
	{
		SendErrorV2(443, fd, GetClient(fd)->GetNickname(), channel->GetName(), " :is already on channel\r\n");
		return ;
	}
	if (channel->GetModesLimit() && channel->CountAllClients() >= channel->GetLimitNumber())
	{
		SendErrorV2(473, fd, GetClient(fd)->GetNickname(), channel->GetName(), " :Cannot invite to channel (+l)\r\n");
		return ;
	}
	user->AddToMyInvitedChannels(channel->GetName());
	SendResponse(":irserv 341 " + GetClient(fd)->GetNickname() + " " + user->GetNickname() + " " + channel->GetName() + " :Invited to channel\r\n", user->GetFd());
	channel->SendEveryone(RPL_INVITED(GetClient(fd)->GetHostname(), GetClient(fd)->GetIPaddress(), user->GetNickname(), channel->GetName()));
}

