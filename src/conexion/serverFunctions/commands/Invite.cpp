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


void	Server::InviteCommand(std::vector<std::string> &splited_cmd, int &fd)
{
	if (splited_cmd.size() < 3)
		{
			SendError(461, fd, GetClient(fd)->GetNickname(), " :Not enough parameters\r\n");
			return ;
		}
	std::string channelname = splited_cmd[2].substr(1);
	if (splited_cmd[2][0] != '#' || !GetChannel(channelname))
	{
		SendError(403, fd, channelname, " :No such channel\r\n");
		return ;
	}
	if (!(GetChannel(channelname)->GetClient(fd)) && !(GetChannel(channelname)->GetAdmin(fd)))
	{
		SendError(442, fd, channelname, " :You're not on that channel\r\n");
		return ;
	}
	if (GetChannel(channelname)->IsClientInChannel(splited_cmd[1]))
	{
		SendErrorV2(443, fd, GetClient(fd)->GetNickname(), channelname, " :is already on channel\r\n");
		return ;
	}
	Client	*Client = GetClient(fd);
	if (!Client)
	{
		SendError(401, fd, splited_cmd[1], " :No such Nick\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetModesInvitOnly() && !GetChannel(channelname)->GetAdmin(fd))
	{
		SendError(482, fd, GetChannel(channelname)->GetClient(fd)->GetNickname(), " :You're not channel operator\r\n");
		return ;
	}
	if (GetChannel(channelname)->GetModesLimit() && GetChannel(channelname)->CountAllClients() >= GetChannel(channelname)->GetModesLimit())
	{
		SendErrorV2(473, fd, GetChannel(channelname)->GetClient(fd)->GetNickname(), channelname, " :Cannot invite to channel (+i)\r\n");
		return ;
	}
	Client->ImInChannel(channelname);//Duda si esto va así
	std::string rep1 = ": 341 " + GetClient(fd)->GetNickname() + " " + Client->GetNickname() + " " + splited_cmd[2] + "\r\n";
	SendResponse(rep1, fd);
	std::string rep2 = ":" + Client->GetIPaddress() + " INVITE " + Client->GetNickname() + " " + splited_cmd[2] + "\r\n";
	SendResponse(rep2, Client->GetFd());
}

