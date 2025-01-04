#include "Colors.h"
#include "Server.hpp"

int	main(void)
{
	Server	myServer;

	std::cout << CYAN << "---- SERVER ----" << WHITE << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler); 
		myServer.ServerInit();
		myServer.ServerLoop();
	}
	catch(const std::exception& e)
	{
		myServer.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << CYAN << "The Server Closed!" << WHITE << std::endl;
}