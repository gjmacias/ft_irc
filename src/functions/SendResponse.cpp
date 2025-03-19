#include "../../includes/conexion/Server.hpp"

void    SendResponse(std::string response, int fd)
{
    struct pollfd   pfd;
    int             ret;
    ssize_t         bytes_sent;

    if (fd < 0)
    {
        std::cerr << "Response send() failed: Invalid socket fd" << std::endl;
        return;
    }
    pfd.fd = fd;
    pfd.events = POLLOUT; // Esperamos hasta que el socket esté listo para escribir
    ret = poll(&pfd, 1, 1000); // Esperamos hasta 1 segundo para escribir
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
    bytes_sent = send(fd, response.c_str(), response.size(), 0);
    if (bytes_sent == -1)
    {
         std::cerr << "Response send() failed: " << strerror(errno) << std::endl;
    }
}
