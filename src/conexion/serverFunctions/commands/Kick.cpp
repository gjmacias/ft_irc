#include "../../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									KICK									  #
###############################################################################
*/


void	Server::KickCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int& fd)
{
	size_t						it = 0;
	size_t						position = 0;
	std::stringstream			ss;
	std::string					channel;
	std::vector<std::string>	list_users;

	if (splited_cmd.size() < 3)
	{
		SendResponse(ERR_NOTENOUGHPARAM(GetClient(fd)->GetNickname()), fd);
		return;
	}
	channel = splited_cmd[1];
	list_users = split_delimeter(splited_cmd[2], ',');
	while (it < 3)
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
	if (GetChannel(channel))
	{
		Channel* Channel = GetChannel(channel);

		for (it = 0; this->_channels[it].GetName() != channel; it++)
			continue;
		if (!Channel->GetClient(fd) && !Channel->GetAdmin(fd)) // ISNOT in th channel
			SendErrorV2(442, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), channel, " :You're not on that channel\r\n");
		if (Channel->GetAdmin(fd))
		{
			for (position = 0; position < list_users.size(); position++)
			{
				if (Channel->IsClientInChannel(list_users[position])) // check if the client to kick is in the channel
				{
					ss << ":" << GetClient(fd)->GetNickname() << "!~" << GetClient(fd)->GetUsername() << "@" << "localhost" << " KICK " << channel << " " << list_users[position];
					if (!cmd_reason.empty())
					{
						ss << " ";
						if (cmd_reason[0] == ':')
							ss << cmd_reason << "\r\n";
						else
							ss << splited_cmd[3] << "\r\n";
					}
					else
						ss << "\r\n";
					Channel->SendEveryone(ss.str());
					if (Channel->IsClientInChannel(list_users[position]) && Channel->GetAdmin(Channel->GetClientByNickname(list_users[position])->GetFd()))
						Channel->RemoveAdmin(Channel->GetClientByNickname(list_users[position])->GetFd());
					else
						Channel->RemoveClient(Channel->GetClientByNickname(list_users[position])->GetFd());
					if (Channel->CountAllClients() == 0)
						_channels.erase(_channels.begin() + it);
					if (Channel->CountAdmins() == 0)
						Channel->ChangeClientToAdmin((Channel->GetObligatedAdmin())->GetNickname());
				}
				else // if the client to kick is not in the channel
				{
					SendErrorV2(441, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), channel, " :They are not in the channel\r\n");
					continue;
				}
			}
		}
		else // if the client is not admin
			SendErrorV2(482, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), channel, " :You are not channel operator\r\n");
	}
	else // if the channel doesn't exist
		SendErrorV2(403, GetClient(fd)->GetFd(), GetClient(fd)->GetNickname(), channel, " :No such channel\r\n");
}

