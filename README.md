# MiniRT - Oseivane && gmacias-

## 1. Introducción

**Sumérgete en el núcleo de la comunicación en tiempo real**
Este proyecto te desafía a construir un servidor **IRC** funcional desde cero, utilizando C++ y conceptos clave de redes. Descubre cómo gestionar múltiples usuarios, canales dinámicos y mensajes en tiempo real, ¡todo mientras dominas un protocolo histórico de Internet!

### Imágenes de Ejemplo

Aquí tienes algunas imágenes renderizadas con **IRC**:

| ![Esfera y cilindro](https://github.com/gjmacias/miniRT/blob/master/README-images/examples/imagen.png) | ![5Objetos](https://github.com/gjmacias/miniRT/blob/master/README-images/examples/5obj.PNG) |
|:------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------:|
| ![Escenario complejo](https://github.com/gjmacias/miniRT/blob/master/README-images/examples/Brightness_Complex.PNG) | ![3Ejes](https://github.com/gjmacias/miniRT/blob/master/README-images/examples/Multi%20objetos.png) |

¡Estas imágenes muestran lo que puedes lograr con el proyecto y te dan una idea de cómo se verán los resultados finales!

### **¿Qué es un servidor IRC?**  

Un servidor IRC (**Internet Relay Chat**) es un sistema que permite la comunicación en tiempo real entre múltiples usuarios a través de canales de texto. Este proyecto te permite:  

- **Gestionar Conexiones Simultáneas:** Maneja docenas de usuarios conectados al mismo tiempo sin bloqueos.  
- **Crear Canales Dinámicos:** Los usuarios pueden unirse, crear y gestionar canales de chat públicos o privados.  
- **Implementar Comandos Básicos y Avanzados:** Desde autenticación (`PASS`, `NICK`, `USER`) hasta gestión de canales (`MODE`, `KICK`, `INVITE`).  

Continúa leyendo para descubrir cómo configurar y utilizar este servidor, incluyendo cómo gestionar usuarios, canales y mensajes en tiempo real.


Para tu proyecto de **servidor IRC**, el índice debe reflejar los aspectos clave del desarrollo y funcionamiento del servidor, así como los conceptos técnicos y las funcionalidades que implementaste. Aquí tienes una propuesta de índice adaptado a tu proyecto:

## **Índice**  

1. [Introducción](#introducción)  
2. [Arquitectura del Servidor](#arquitectura-del-servidor)  
3. [Comandos Básicos y de Operadores](#Comandos Básicos y de Operadores)  
4. [Estructuras de Datos](#estructuras-de-datos)  
5. [Manejo de mensajes y Errores](#manejo-de-mensajes-y-errores)  
6. [Compilación y Ejecución](#compilación-y-ejecución)  
7. [Pruebas y Demostración](#pruebas-y-demostración)  

## **2.Arquitectura del Servidor**  

El servidor IRC está diseñado para manejar múltiples conexiones de clientes de manera eficiente, utilizando mecanismos como `poll()`, `fcntl()`, y estructuras como `sockaddr_in` y `pollfd`. A continuación, se explica en detalle cómo funcionan estas herramientas y por qué son esenciales para el servidor.

### **1. Estructuras y Funciones Clave**  

##### **`sockaddr_in`**  
- **Qué es**: Una estructura que almacena la información de una dirección de socket (familia, dirección IP y puerto).  
- **Uso en el código**:  
  ```cpp
  struct sockaddr_in socketAddress;
  socketAddress.sin_family = AF_INET;          // Familia de direcciones (IPv4)
  socketAddress.sin_port = htons(this->_port); // Puerto en formato de red
  socketAddress.sin_addr.s_addr = INADDR_ANY;  // Escucha en todas las interfaces
  ```  
- **Por qué se usa**: Para configurar el socket del servidor y especificar en qué dirección y puerto debe escuchar.  

##### **`pollfd`**  
- **Qué es**: Una estructura que representa un descriptor de archivo y los eventos que se deben monitorear.  
- **Uso en el código**:  
  ```cpp
  struct pollfd newPoll;
  newPoll.fd = _mainSocket;    // Descriptor de archivo a monitorear
  newPoll.events = POLLIN;     // Evento: datos disponibles para leer
  newPoll.revents = 0;         // Eventos que ocurrieron (se llena automáticamente)
  ```  
- **Por qué se usa**: Para monitorear múltiples descriptores de archivo (sockets) en un solo hilo, evitando bloqueos.  
##### **`poll()`**  
- **Qué es**: Una función que monitorea múltiples descriptores de archivo para detectar eventos (como datos disponibles para leer).  
- **Uso en el código**:  
  ```cpp
  poll(&_pollSocketFds[0], _pollSocketFds.size(), -1)
  ```  
- **Opciones clave**:  
  - `POLLIN`: Detecta si hay datos disponibles para leer.  
  - `POLLOUT`: Detecta si el socket está listo para escribir.  
- **Por qué se usa**: Para manejar múltiples conexiones de clientes sin necesidad de crear un hilo por cada una.  

##### **`fcntl()`**  
- **Qué es**: Una función que permite configurar opciones en un descriptor de archivo.  
- **Uso en el código**:  
  ```cpp
  fcntl(_mainSocketFd, F_SETFL, O_NONBLOCK)
  ```  
- **Opciones clave**:  
  - `O_NONBLOCK`: Establece el socket en modo no bloqueante.  
- **Por qué se usa**: Para evitar que el servidor se bloquee mientras espera datos en un socket.  

### **2. Flujo de Trabajo del Servidor**  

1. **Inicialización (`ServerInit`)**:
   - Se crea un socket con `socket()`.  
   - Se configura el socket para reutilizar la dirección (`SO_REUSEADDR`) y evitar bloqueos (`O_NONBLOCK`).  
   - Se enlaza el socket a una dirección y puerto con `bind()`.  
   - Se pone el socket en modo de escucha con `listen()`.  
   - Se añade el socket a la lista de `pollfd` para ser monitoreado.  

2. **Bucle Principal (`ServerLoop`)**:
   - El servidor entra en un bucle donde llama a `poll()` para detectar actividad en los sockets.  
   - Si hay actividad en el socket principal, se acepta una nueva conexión con `accept()`.  
   - Si hay actividad en un socket de cliente, se leen los datos con `recv()`.  

3. **Aceptación de Clientes (`AcceptNewClient`)**:
   - Se acepta la conexión con `accept()`, que devuelve un nuevo descriptor de archivo para el cliente.  
   - El nuevo socket se configura en modo no bloqueante y se añade a la lista de `pollfd`.  

4. **Recepción de Datos (`ReceiveNewData`)**:
   - Se leen los datos del cliente con `recv()`.  
   - Si el cliente se desconecta (`recv()` devuelve 0), se limpian sus recursos.  
   - Si hay datos, se procesan y se ejecutan los comandos correspondientes.

## **3.Comandos Básicos y de Operadores**  

Este servidor IRC soporta una variedad de comandos para la autenticación, gestión de usuarios y canales, y privilegios especiales para operadores.  

#### **Autenticación y Registro de Usuarios**  
- **`PASS <contraseña>`**: Autentica al usuario con la contraseña del servidor.  
- **`NICK <apodo>`**: Asigna un apodo (nickname) al usuario.  
- **`USER <username> <hostname> <servername> <realname>`**: Registra al usuario en el servidor.  
#### **Gestión de Canales y Mensajería**  
- **`JOIN <canal>`**: Une al usuario a un canal.  
- **`PART <canal>`**: Abandona un canal.  
- **`PRIVMSG <destinatario> <mensaje>`**: Envía un mensaje a un canal o usuario.  
#### **Comandos de Operadores**  
- **`KICK <canal> <usuario>`**: Expulsa a un usuario de un canal.  
- **`INVITE <usuario> <canal>`**: Invita a un usuario a un canal.  
- **`TOPIC <canal> <tema>`**: Cambia el tema de un canal.  
- **`MODE <canal> <modo>`**: Configura modos como contraseñas, límites de usuarios o permisos.  

📖 **Más detalles**: Consulta [COMANDOS.md](docs/COMANDOS.md) para una lista completa de modos y ejemplos.  


## **4. Estructuras de Datos**  

El servidor IRC utiliza tres estructuras principales para gestionar usuarios, canales y conexiones:  

#### **1. `Server`**  
- **Propósito**: Gestiona el estado global del servidor.  
- **Datos clave**:  
  - `_port`: Puerto en el que escucha el servidor.  
  - `_mainSocketFd`: Descriptor del socket principal.  
  - `_clients`: Lista de clientes conectados.  
  - `_channels`: Lista de canales creados.  
  - `_pollSocketFds`: Descriptores de archivo monitoreados por `poll()`.  

#### **2. `Channel`**  
- **Propósito**: Representa un canal de chat.  
- **Datos clave**:  
  - `_name`: Nombre del canal.  
  - `_clients`: Lista de usuarios en el canal.  
  - `_admins`: Lista de operadores del canal.  
  - `_modes`: Modos del canal (invite-only, contraseña, límite de usuarios, etc.).  

#### **3. `Client`**  
- **Propósito**: Representa un usuario conectado.  
- **Datos clave**:  
  - `_fd`: Descriptor de archivo del socket del cliente.  
  - `_nickname` y `_username`: Identificadores del usuario.  
  - `_buffer`: Almacena datos recibidos hasta que se procesan.  
  - `_myInviteChannels`: Canales a los que el usuario ha sido invitado.  

📖 **Más detalles**: Consulta [ESTRUCTURAS.md](docs/ESTRUCTURAS.md) para una explicación completa de estas estructuras y su implementación. 


## **5. Manejo de Mensajes y Errores**  

El servidor IRC utiliza un sistema robusto para enviar respuestas a los clientes y manejar errores de manera eficiente. Esto garantiza que los usuarios reciban retroalimentación clara y que el servidor pueda gestionar situaciones inesperadas sin interrupciones.  

#### **1. Envío de Respuestas (`SendResponse`)**  
La función `src/functions/SendResponse` se encarga de enviar mensajes desde el servidor a los clientes. Aquí está cómo funciona:  

- **Uso de `poll()`**:  
  - Antes de enviar datos, el servidor verifica si el socket del cliente está listo para escribir usando `poll()`.  
  - Esto evita bloqueos y asegura que el servidor no se quede esperando indefinidamente.  

#### **2. Respuestas Predefinidas**  
El servidor utiliza macros en `includes/comunication/` para generar respuestas estandarizadas según el protocolo IRC. Estas respuestas incluyen:  

- **Respuestas de éxito (`RPL_*`)**:  
  - `RPL_CONNECTED`: Mensaje de bienvenida al usuario.  
  - `RPL_JOINMSG`: Notificación cuando un usuario se une a un canal.  
  - `RPL_TOPICIS`: Envía el tema actual de un canal.  

- **Errores (`ERR_*`)**:  
  - `ERR_NICKINUSE`: Indica que el apodo ya está en uso.  
  - `ERR_NOTOPERATOR`: Notifica que el usuario no tiene privilegios de operador.  
  - `ERR_INCORPASS`: Indica que la contraseña es incorrecta.  

- **Ejemplo de error**:  
  ```cpp
  SendResponse(ERR_NICKINUSE("Bob"), clientFd); // Envía un error de apodo en uso
  ```  

#### **3. ¿Por Qué Este Enfoque?**  

- **Claridad**: Las macros predefinidas hacen que el código sea más legible y fácil de mantener.  
- **Eficiencia**: El uso de `poll()` asegura que el servidor no se bloquee al enviar datos.  
- **Robustez**: El manejo de errores evita que el servidor falle ante situaciones inesperadas.  

## **6.Compilación y Ejecución**  

Sigue estos pasos para poner en marcha el servidor IRC:  

#### **1. Compilar el Proyecto**  
1. Abre una terminal en el directorio del proyecto.  
2. Ejecuta:  
   ```bash  
   make  
   ```  
   Esto compilará el código y generará el ejecutable `ircserv`.  

#### **2. Ejecutar el Servidor**  
1. Usa el siguiente comando para iniciar el servidor:  
   ```bash  
   ./ircserv <puerto> <contraseña>  
   ```  
   - **`<puerto>`**: Puerto en el que el servidor escuchará (por ejemplo, `6667`).  
   - **`<contraseña>`**: Contraseña que los clientes deben usar para conectarse.  

   **Ejemplo**:  
   ```bash  
   ./ircserv 6667 mySecurePassword  
   ```  

#### **3. Conectar un Cliente**  
1. Abre un cliente IRC como [HexChat](https://hexchat.github.io/) o [Irssi](https://irssi.org/).  
2. Conéctate al servidor usando la dirección `localhost` y el puerto especificado.  
3. Autentícate con la contraseña proporcionada.  


## Demostración

Para ilustrar cómo funciona el proyecto en la práctica, hemos preparado una serie de videos que muestran diferentes aspectos y funcionalidades del programa en acción. Estos videos cubren:


Estos clips te proporcionarán una visión clara de cómo utilizar el programa y lo que puedes esperar en términos de rendimiento y visualización.

# Cómo Contribuir

Si deseas contribuir al proyecto:

1. **Fork** el repositorio.
2. **Crea una rama** para tus cambios.
3. **Realiza los cambios** necesarios.
4. **Envía un Pull Request** describiendo los cambios realizados.

# Quizás pueda interesarte!

### - Perfil de GitHub Oseivane
[AQUÍ](https://github.com/faltrum)

### - Perfil de GitHub gmacias-
[AQUÍ](https://github.com/gjmacias)

# Contactos 📥

◦ Email ffornes-: ffornes-@student.42barcelona.com

◦ Email gmacias-: gmacias-@student.42barcelona.com
