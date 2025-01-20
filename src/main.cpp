#include "ColorsDefinitions.h"
#include "AllFunctions.h"
#include "Server.hpp"

int	main(int words, char **arguments)
{
	Server	myServer;

	if (words != 3)
	{
		std::cerr << "Usage: " << arguments[0] << " <port number> <password>" << std::endl;
		return (1);
	}
	std::cout << CYAN << "---- SERVER ----" << WHITE << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		signal(SIGPIPE, SIG_IGN);
		if(!isPortValid(arguments[1]) || !*arguments[2] || std::strlen(arguments[2]) > 20)
		{
			std::cerr << "invalid Port number / Password!" << std::endl;
			return (1);
		}
		myServer.ServerInit(atoi(arguments[1]), arguments[2]);
		myServer.ServerLoop();
	}
	catch(const std::exception &e)
	{
		myServer.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << CYAN << "The Server Closed!" << WHITE << std::endl;
}