# Estructura de Datos: `Server.hpp`

El archivo `Server.hpp` define la estructura principal del servidor IRC, gestionando la conexión con clientes, la administración de canales y la ejecución de comandos. A continuación, se detallan sus componentes clave.

---

## **1. Librerías Incluidas**

### **Librerías estándar**
- `<iostream>` y `<vector>`: Manejo de entrada/salida y contenedores dinámicos.

### **Librerías del sistema**
- `<sys/socket.h>`, `<netinet/in.h>` y `<arpa/inet.h>`: Manejo de sockets para la comunicación en red.
- `<fcntl.h>`, `<unistd.h>` y `<poll.h>`: Control de descriptores de archivo y multiplexación de conexiones.

### **Librerías de C**
- `<csignal>`, `<cstdlib>`, `<cstring>` y `<cerrno>`: Manejo de señales, memoria y errores.

### **Librerías del proyecto**
- `Client.hpp` y `Channel.hpp`: Definen las estructuras de clientes y canales.
- `Errors.hpp` y `Information.hpp`: Definen mensajes de error y respuestas del servidor.
- `AllFunctions.h` y `ColorsDefinitions.h`: Contienen funciones auxiliares y definiciones de colores.

---

## **2. Variables Principales**

```cpp
static bool _signal;
int _port;
int _mainSocketFd;
std::string _password;
std::vector<Client *> _clients;
std::vector<Channel> _channels;
std::vector<struct pollfd> _pollSocketFds;
```

- `_signal`: Bandera estática para manejar interrupciones del sistema.
- `_port`: Almacena el puerto en el que escucha el servidor.
- `_mainSocketFd`: Descriptor del socket principal del servidor.
- `_password`: Contraseña de acceso al servidor.
- `_clients`: Lista de clientes conectados al servidor.
- `_channels`: Lista de canales activos en el servidor.
- `_pollSocketFds`: Lista de descriptores de sockets utilizados en `poll()` para gestionar múltiples conexiones.

### **¿Por qué `std::vector` en lugar de `std::list`?**

Se usa `std::vector` porque:
1. **Acceso rápido**: `vector` permite acceso directo por índice, lo que mejora el rendimiento.
2. **Eficiencia en iteraciones**: Almacena elementos de manera contigua en memoria, optimizando la caché.
3. **Manejo de clientes mediante punteros**: Evita problemas con la reubicación de memoria en `vector`, asegurando que los punteros de `Channel` y `Server` siempre apunten a la misma dirección de `Client`.

---

## **3. Organización de Funciones**

### **Métodos Canónicos**
El `Server` sigue la forma ortodoxa canónica:

```cpp
Server();
~Server();
Server(Server const &src);
Server &operator=(Server const &src);
```

Estos aseguran que la clase sea correctamente copiable y asignable, evitando problemas de memoria y asegurando un comportamiento predecible.

### **Getters y Setters**

```cpp
int GetPort();
int GetMainFd();
std::string GetPassword();
Client *GetClient(int fd);
Client *GetClient_Nickname(std::string nickname);
Channel *GetChannel(std::string name);
```

Permiten acceder de manera controlada a las variables privadas del servidor, asegurando la encapsulación y evitando modificaciones accidentales.

### **Funciones Principales**

- **Inicialización y manejo del servidor**
  ```cpp
  void ServerInit(int port, std::string password);
  void ServerLoop();
  void AcceptNewClient();
  void ReceiveNewData(int fd);
  void CloseFds();
  ```

- **Gestión de clientes y canales**
  ```cpp
  void AddChannel(Channel& newChannel);
  void AddClient(int clientFd, std::string IPAddress);
  void RemoveClient(int fd);
  void RemoveChannel(std::string name);
  ```

- **Validaciones**
  ```cpp
  bool IsRegisteredAndLoged(int fd);
  bool IsValidNickname(std::string &nickname);
  bool IsNickNameInUse(std::string &nickname);
  ```

Estas funciones garantizan un manejo robusto del servidor, permitiendo agregar y eliminar clientes/canales de manera segura, así como validar los datos antes de realizar operaciones.

---

## **4. Comandos del Servidor**

Los comandos implementados permiten la interacción entre los usuarios y el servidor.

- **Comandos generales**
  ```cpp
  void CapCommand(std::vector<std::string> &splited_cmd, int fd);
  void PingCommand(std::vector<std::string> &splited_cmd, int fd);
  void WhoisCommand(std::vector<std::string> &splited_cmd, int fd);
  ```

- **Gestión de canales**
  ```cpp
  void JoinCommand(std::vector<std::string> &splited_cmd, int fd);
  void PartCommand(std::vector<std::string> &splited_cmd, std::string cmd_reason, int fd);
  void TopicCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);
  ```

- **Moderación**
  ```cpp
  void KickCommand(std::vector<std::string>& splited_cmd, std::string cmd_reason, int fd);
  void ModeCommand(std::vector<std::string>& splited_cmd, int fd);
  ```

Estos comandos garantizan un control adecuado del servidor y la interacción fluida entre usuarios, permitiendo gestionar roles, restricciones y comunicación de manera eficiente.

---

# Estructura de Datos: `Channel.hpp`

El archivo `Channel.hpp` define la estructura y funcionalidades clave de un canal en el servidor IRC. Los canales son esenciales para organizar la comunicación entre clientes, permitiendo control de acceso, gestión de usuarios y administración de permisos.

---

## **1. Librerías Incluidas**

### **Librerías estándar**
- `<ctime>`: Manejo de marcas de tiempo para la creación y modificación del canal.

### **Librerías del proyecto**
- `Client.hpp`: Define la estructura y operaciones de los clientes.
- `Server.hpp`: Permite la comunicación con el servidor.

---

## **2. Variables Principales**

```cpp
std::string _time_creation;
std::vector<std::pair<char, bool>> _modes;
std::string _name;
std::string _key_password;
std::string _topic_name;
std::string _topic_time;
std::string _topic_editor;
size_t _limit_numberOfClients;
std::vector<Client *> _clients;
std::vector<Client *> _admins;
```

- `_time_creation`: Guarda el tiempo de creación del canal.
- `_modes`: Lista de modos activos en el canal (ej. restricciones y permisos).
- `_name`: Nombre del canal.
- `_key_password`: Clave de acceso al canal.
- `_topic_name`, `_topic_time`, `_topic_editor`: Información sobre el tema del canal.
- `_limit_numberOfClients`: Límite de usuarios permitidos en el canal.
- `_clients`: Lista de clientes en el canal.
- `_admins`: Lista de administradores del canal.

### **Uso de `std::vector` en lugar de `std::list`**

Se elige `std::vector` debido a:
1. **Acceso eficiente**: Permite acceso rápido a clientes y administradores.
2. **Mejor uso de caché**: `vector` almacena elementos de manera contigua en memoria.
3. **Compatibilidad con `Client`**: Se evita el movimiento de memoria que ocurre en `vector`, asegurando que los punteros a `Client` sean estables dentro del `Server` y `Channel`.

---

## **3. Organización de Funciones**

### **Métodos Canónicos**
El `Channel` sigue la forma ortodoxa canónica:

```cpp
Channel();
~Channel();
Channel(Channel const &src);
Channel &operator=(Channel const &src);
```

Estos métodos aseguran una correcta administración de memoria y permiten copiar o asignar instancias de `Channel` sin problemas.

### **Getters y Setters**

```cpp
std::string GetName();
std::string GetPassword();
std::string GetTopicName();
size_t GetLimitNumber();
void SetName(std::string name);
void SetPassword(std::string password);
void SetTopicName(std::string topic_name);
void SetLimitNumber(unsigned int number_limit);
```

Estas funciones proporcionan una interfaz segura para acceder y modificar atributos del canal, manteniendo encapsulación y evitando modificaciones directas.

### **Funciones Principales**

- **Gestión de clientes y administradores**
  ```cpp
  void AddClient(Client *newClient);
  void AddAdmin(Client *newClient);
  void RemoveClient(int fd);
  void RemoveAdmin(int fd);
  ```
  Permiten agregar y eliminar clientes o administradores de manera segura.

- **Manejo de roles**
  ```cpp
  bool ChangeClientToAdmin(std::string nickname);
  bool ChangeAdminToClient(std::string nickname);
  ```
  Cambian los permisos de un usuario dentro del canal.

- **Funciones de validación**
  ```cpp
  bool IsModeCharActive(char character);
  bool IsClientInChannel(std::string nickname);
  ```
  Validan si un cliente pertenece a un canal y si un modo específico está activado.

- **Envío de mensajes**
  ```cpp
  void SendEveryone(std::string message);
  void SendMeToAll(int fd, std::string message);
  ```
  Gestionan la difusión de mensajes dentro del canal.

---

## **4. Modos y Administración del Canal**

Los canales pueden tener modos que afectan su funcionamiento:

- **Restricción de invitaciones** (`+i`): Solo usuarios invitados pueden unirse.
- **Protección de temas** (`+t`): Solo administradores pueden cambiar el tema.
- **Clave de acceso** (`+k`): Requiere una contraseña para ingresar.
- **Límite de usuarios** (`+l`): Restringe la cantidad máxima de clientes en el canal.
- **Privilegios de operador** (`+o`): Define administradores dentro del canal.

Estos modos permiten que un canal sea configurable y adecuado para distintos tipos de comunidades dentro del servidor IRC.

---
# Estructura de Datos: `Client.hpp`

El archivo `Client.hpp` define la estructura y el comportamiento de los clientes dentro del servidor IRC. Un cliente representa a un usuario conectado al servidor y maneja su información, autenticación y comunicación con otros clientes y canales.

---

## **1. Librerías Incluidas**

### **Librerías del proyecto**
- `Server.hpp`: Permite la interacción del cliente con el servidor.
- `Channel.hpp`: Permite la gestión de los canales a los que pertenece el cliente.

---

## **2. Variables Principales**

```cpp
int _fd;
bool _isRegistered;
bool _isLogedInServer;
std::string _IPaddress;
std::string _nickname;
std::string _username;
std::string _buffer;
std::vector<std::string> _myInviteChannels;
```

- `_fd`: Descriptor de archivo asociado al socket del cliente.
- `_isRegistered`: Indica si el cliente ha ingresado la contraseña correctamente.
- `_isLogedInServer`: Determina si el cliente ha completado el proceso de autenticación.
- `_IPaddress`: Dirección IP del cliente.
- `_nickname`, `_username`: Identidad del cliente dentro del servidor.
- `_buffer`: Almacena temporalmente los datos recibidos del cliente.
- `_myInviteChannels`: Lista de canales a los que ha sido invitado el cliente.

Se utiliza `std::vector` en `_myInviteChannels` porque permite un acceso rápido y eficiente a las invitaciones del cliente, evitando la sobrecarga de `std::list` cuando las búsquedas no son tan frecuentes.

---

## **3. Organización de Funciones**

### **Métodos Canónicos**
El `Client` sigue la forma ortodoxa canónica:

```cpp
Client();
Client(std::string nickname, std::string username, int fd);
~Client();
Client(Client const &src);
Client &operator=(Client const &src);
```

Estos métodos permiten la creación, copia y destrucción segura de objetos `Client`, asegurando la correcta administración de memoria.

### **Getters y Setters**

```cpp
int GetFd();
bool GetIsRegistered();
bool GetIsLogedInServer();
std::string GetIPaddress();
std::string GetNickname();
std::string GetUsername();
std::string GetBuffer();
```

Estos métodos permiten acceder a la información del cliente sin modificar directamente las variables internas, garantizando encapsulación y seguridad.

Los **setters** cumplen una función similar pero permitiendo modificar las propiedades de manera controlada:

```cpp
void SetFd(int fd);
void SetIsRegistered(bool selector);
void SetIsLogedInServer(bool selector);
void SetIPaddress(std::string ipadd);
void SetNickname(std::string nickname);
void SetUsername(std::string username);
void SetBuffer(std::string received);
```

Estos métodos aseguran que los cambios en los atributos se realicen de manera estructurada y controlada.

### **Funciones Principales**

- **Manejo del buffer:**
  ```cpp
  void ClearBuffer();
  void ClearUsedBuffer();
  ```
  Permiten limpiar los datos almacenados en el buffer del cliente.

- **Gestión de invitaciones a canales:**
  ```cpp
  void AddToMyInvitedChannels(std::string channel_name);
  void RmFromMyInvitedChannels(std::string channel_name);
  bool ImInvitedInChannel(std::string channel_name);
  ```
  Estas funciones manejan la lista de canales a los que el cliente ha sido invitado, permitiendo agregar, eliminar y verificar invitaciones.

---

## **4. Relación con `Server` y `Channel`**

El `Client` está diseñado para interactuar con `Server` y `Channel` de manera eficiente. Debido a la gestión dinámica de clientes dentro del servidor y los canales, se utiliza un puntero a `Client` dentro de `Channel`, asegurando que cada cliente mantenga una referencia estable sin ser movido en memoria.

---
