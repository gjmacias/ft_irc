#include "Server.hpp"

/*
###############################################################################
#									PARSE									  #
###############################################################################
*/
void	Server::ParseAndExcecute(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;

	std::vector<std::string>	splited_cmd = split_cmd(cmd);
	size_t						found = cmd.find_first_not_of(" \t\v");

	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
	if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
		ClientAuthentification(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		ClientNickname(cmd, fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		ClientUsername(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QuitCommand(cmd, fd);
	else if(isRegistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			InviteCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JoinCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KickCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			ModeCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			PartCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PrivateMessageCommand(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			TopicCommand(cmd, fd);
		else if (splited_cmd.size())
			SendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(), splited_cmd[0]), fd);
	}
	else if (!isRegistered(fd))
		SendResponse(ERR_NOTREGISTERED(std::string("*")),fd);
}
