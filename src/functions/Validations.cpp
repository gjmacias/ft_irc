#include "AllFunctions.h"

bool isPortValid(std::string port)
{
	return (port.find_first_not_of("0123456789") == std::string::npos && std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

