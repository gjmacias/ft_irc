#include "Server.hpp"

/*
###############################################################################
#									INVITE									  #
###############################################################################
Esta función Server::InviteCommand implementa la lógica para invitar
a un usuario a un canal en un servidor IRC. Maneja múltiples
errores potenciales y envía respuestas adecuadas al cliente 
en función de las condiciones detectadas.
*/

void	Server::InviteCommand(std::string &cmd, int &fd)
{
	std::vector<std::string> smcd = split_cmd(cmd);
	if (scmd.size() < 3)
		{
			senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
			return ;
		}
	std::channelname = smcd[2].substr(1);
	if (scmd[2][0] != '#' || !GetChannel(channelname))
	{
		senderror(403, channelname, fd, " :No such channel\r\n");
		return ;
	}
	if (!(GetChannel(channelname)->get_client(fd)) && !(GetChannel(channelname)->get_admin(fd)))
	{
		senderror(442, channelname, fd, " :You're not on that channel\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetClientInChannel(csmd[1]))
	{
		senderror(443, GetClient(fd)->GetNickName(), channelname, fd, " :is already on channel\r\n");
		return ;
	}
	Client	*clt = GetClientNick(scmd[1]);
	if (!clt)
	{
		senderror(401, scmd[1], fd, " :No such Nick\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetInvitOnly() && !GetChannel(channelname)->get_admin(fd))
	{
		senderror(482, GetChannel(channelname)->get_client(fd)->GetNickName(), fd, " :You're not channel operator\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetLimit() && GetChannel(channelname)->GetClientsNumber() >= GetChannel(channelname)->GetLimit())
	{
		senderror(473, GetChannel(channelname)->get_client(fd)->GetNickName(), channelname, fd, " :Cannot invite to channel (+i)\r\n");
		return ;
	}
	clt->AddChannelInvite(channelname);
	std::string rep1 = ": 341 " + GetClient(fd)->GetNickName() + " " + clt->GetNickName() + " " + scmd[2] + "\r\n";
	_sendResponse(rep1, fd);
	std::string rep2 = ":" + clt->getHostName() + " INVITE " + clt->GetNickName() + " " + scmd[2] + "\r\n";
	_sendResponse(rep2, clt->GetFd());
}
