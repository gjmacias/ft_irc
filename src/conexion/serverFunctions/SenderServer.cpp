#include "../../../includes/conexion/Server.hpp"

/*
###############################################################################
#									SENDERS									  #
###############################################################################
*/
void	Server::SendResponse(std::string response, int fd)
{
	//if(send(fd, response.c_str(), response.size(), 0) == -1)
	//	std::cerr << "Response send() failed" << std::endl;
	if (fd < 0)
    {
        std::cerr << "Response send() failed: Invalid socket fd" << std::endl;
        return;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLOUT; // Esperamos hasta que el socket esté listo para escribir

    int ret = poll(&pfd, 1, 1000); // Esperamos hasta 1 segundo para escribir

    if (ret == -1)
    {
        std::cerr << "Poll failed: " << strerror(errno) << std::endl;
        return;
    }
    else if (ret == 0)
    {
        std::cerr << "Timeout: No se pudo escribir en el socket" << std::endl;
        return;
    }

    ssize_t bytes_sent = send(fd, response.c_str(), response.size(), 0);

    if (bytes_sent == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            std::cerr << "Socket no está listo para escribir, reintentando más tarde..." << std::endl;
            return;
        }
        else if (errno == EPIPE || errno == ECONNRESET)
        {
            std::cerr << "Cliente desconectado, cerrando socket..." << std::endl;
            close(fd);
            RemoveClient(fd); // Asegúrate de limpiar al cliente si tienes esta función
            return;
        }
        else
        {
            std::cerr << "Response send() failed: " << strerror(errno) << std::endl;
        }
    }
}

	//	-----	Send Errors		-----
void	Server::SendError(int code, int fd, std::string clientname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":localhost " << code << " " << clientname << msg;
	response = ss.str();
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
}

void	Server::SendErrorV2(int code, int fd, std::string clientname, std::string channelname, std::string msg)
{
	std::stringstream	ss;
	std::string			response;

	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	response = ss.str();
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
}

