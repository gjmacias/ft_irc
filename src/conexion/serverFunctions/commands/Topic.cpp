#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									TOPIC									  #
###############################################################################
*/

std::string Server::TopicTime()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}

void	Server::TopicCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd)
{
	std::string			channelname;

	if (splited_cmd.size() < 2)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return;
	}
	channelname = splited_cmd[1];
	if (!GetChannel(channelname))
	{
		SendError(403, fd, channelname, " :No such channel \r\n");
		return;
	}
	if (!(GetChannel(channelname)->GetClient(fd)) && !(GetChannel(channelname)->GetAdmin(fd)))
	{
		SendError(442, fd, channelname, " :You're not on that channel\r\n");
		return;
	}
	if (splited_cmd.size() == 2) // response with the channelname modes (MODE #channel)
	{
		if (GetChannel(channelname)->GetTopicName() == "")
			SendResponse(": 331 " + GetClient(fd)->GetNickname() + " " + channelname + " : No topic is set\r\n", fd);
		else
		{
			SendResponse(RPL_TOPICIS(GetClient(fd)->GetNickname(), channelname, GetChannel(channelname)->GetTopicName()), fd);
			SendResponse(RPL_TOPICTIME(GetClient(fd)->GetNickname(), channelname, GetChannel(channelname)->GetTopicEditor(), GetChannel(channelname)->GetTopicTime()), fd);
		}
		return;
	}
	if (!(GetChannel(channelname)->GetAdmin(fd)) && GetChannel(channelname)->GetModesTopicRestriction())
	{
		SendResponse(ERR_NOTOPERATOR(channelname), fd);
		return;
	}
	TopicExecute(channelname, splited_cmd, cmd_reason, fd);
}

void	Server::TopicExecute(std::string channelname, std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd)
{
	size_t				it = 0;
	size_t				position = 0;

	while (it < 2)
	{
		position = cmd_reason.find(splited_cmd[it]);
		cmd_reason = cmd_reason.substr(position + splited_cmd[it].size());
		position = cmd_reason.find_first_not_of("\t\v ");
		if (position != std::string::npos)
			cmd_reason = cmd_reason.substr(position);
		else
			cmd_reason.clear();
		it++;
	}
	if ( cmd_reason[0] != ':')
		cmd_reason = splited_cmd[2];
	else
	{
		cmd_reason = cmd_reason.substr(1);
		position = cmd_reason.find_first_not_of("\t\v ");
		if (position != std::string::npos)
			cmd_reason = cmd_reason.substr(position);
		else
			cmd_reason.clear();
	}
	if (cmd_reason.empty())
	{
		(GetChannel(channelname))->SetTopicName("");
		(GetChannel(channelname))->SetTopicEditor("");
		(GetChannel(channelname))->SetTopicTime("");
		SendResponse(": 331 " + GetClient(fd)->GetNickname() + " " + channelname + " : No topic is set\r\n", fd);
	}
	else
	{
		(GetChannel(channelname))->SetTopicName(cmd_reason);
		(GetChannel(channelname))->SetTopicEditor(GetClient(fd)->GetNickname());
		(GetChannel(channelname))->SetTopicTime(TopicTime());
		(GetChannel(channelname))->SendEveryone(RPL_TOPICIS(GetClient(fd)->GetNickname(), channelname, GetChannel(channelname)->GetTopicName()));
	}
}
