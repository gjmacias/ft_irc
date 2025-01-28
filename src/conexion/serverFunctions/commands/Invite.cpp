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


void	Server::InviteCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	if (splited_cmd.size() < 3)
		{
			SendError(461, GetClient(fd)->GetNickname(), fd, " :Not enough parameters\r\n");
			return ;
		}
	std::string channelname = splited_cmd[2].substr(1);
	if (splited_cmd[2][0] != '#' || !GetChannel(channelname))
	{
		SendError(403, channelname, fd, " :No such channel\r\n");
		return ;
	}
	if (!(GetChannel(channelname)->GetClient(fd)) && !(GetChannel(channelname)->GetAdmin(fd)))
	{
		SendError(442, channelname, fd, " :You're not on that channel\r\n");
		return ;
	}
	if (GetChannel(channelname)->IsClientInChannel(splited_cmd[1]))
	{
		SendError(443, GetClient(fd)->GetNickname(), channelname, fd, " :is already on channel\r\n");
		return ;
	}
	Client	*clt = GetClient(splited_cmd[1]);
	if (!clt)
	{
		SendError(401, splited_cmd[1], fd, " :No such Nick\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetModesInvitOnly() && !GetChannel(channelname)->GetAdmin(fd))
	{
		SendError(482, GetChannel(channelname)->GetClient(fd)->GetNickname(), fd, " :You're not channel operator\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetLimit() && GetChannel(channelname)->GetClientsNumber() >= GetChannel(channelname)->GetLimit())
	{
		SendError(473, GetChannel(channelname)->GetClient(fd)->GetNickname(), channelname, fd, " :Cannot invite to channel (+i)\r\n");
		return ;
	}
	clt->ImInChannel(channelname);//Duda si esto va así
	std::string rep1 = ": 341 " + GetClient(fd)->GetNickname() + " " + clt->GetNickname() + " " + splited_cmd[2] + "\r\n";
	SendResponse(rep1, fd);
	std::string rep2 = ":" + clt->getHostName() + " INVITE " + clt->GetNickname() + " " + splited_cmd[2] + "\r\n";
	SendResponse(rep2, clt->GetFd());
}
