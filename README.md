# IRC - Oseivane && gmacias-

## **1. Introducción**
**Sumérgete en el núcleo de la comunicación en tiempo real**
Este proyecto te desafía a construir un servidor **IRC** funcional desde cero, utilizando C++ y conceptos clave de redes. Descubre cómo gestionar múltiples usuarios, canales dinámicos y mensajes en tiempo real, ¡todo mientras dominas un protocolo histórico de Internet!

### Imágenes de Ejemplo
Aquí tienes algunas imágenes del funcionamiento del server **IRC**:

| ![inicio](https://github.com/user-attachments/assets/0b2a4d5e-1314-422d-9a28-e63d24056d0b) | ![mensajes](https://github.com/user-attachments/assets/a3bcb2a8-3145-455d-b567-1e42f6e589ce) |
|:------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------:|
| ![clientes](https://github.com/user-attachments/assets/de24c2a0-f0d6-42ab-8b66-872af7a02f24) | ![chat](https://github.com/user-attachments/assets/0ff4c807-aac1-41f1-8fa4-66891edf8a7b) |

¡Estas imágenes muestran lo que puedes lograr con el proyecto y te dan una idea de cómo se verán los resultados finales!

### **¿Qué es un servidor IRC?**  

Un servidor IRC (**Internet Relay Chat**) es un sistema que permite la comunicación en tiempo real entre múltiples usuarios a través de canales de texto. Este proyecto te permite:  

- **Gestionar Conexiones Simultáneas:** Maneja docenas de usuarios conectados al mismo tiempo sin bloqueos.  
- **Crear Canales Dinámicos:** Los usuarios pueden unirse, crear y gestionar canales de chat públicos o privados.  
- **Implementar Comandos Básicos y Avanzados:** Desde autenticación (`PASS`, `NICK`, `USER`) hasta gestión de canales (`MODE`, `KICK`, `INVITE`).  

Continúa leyendo para descubrir cómo configurar y utilizar este servidor, incluyendo cómo gestionar usuarios, canales y mensajes en tiempo real.


Para tu proyecto de **servidor IRC**, el índice debe reflejar los aspectos clave del desarrollo y funcionamiento del servidor, así como los conceptos técnicos y las funcionalidades que implementaste. Aquí tienes una propuesta de índice adaptado a tu proyecto:

## **Índice**  

## Índice

1. [Introducción](#1-introduccion)
   - [Imágenes de Ejemplo](#imagenes-de-ejemplo)
   - [¿Qué es un servidor IRC?](#que-es-un-servidor-irc)
2. [Arquitectura del Servidor](#2-arquitectura-del-servidor)
   - [Estructuras y Funciones Clave](#estructuras-y-funciones-clave)
   - [Flujo de Trabajo del Servidor](#flujo-de-trabajo-del-servidor)
3. [Comandos Básicos y de Operadores](#3-comandos-basicos-y-de-operadores)
   - [README Comandos](#readme-comandos)
4. [Estructuras de Datos](#4-estructuras-de-datos)
   - [README Estructura](#readme-estructura)
5. [Manejo de Mensajes y Errores](#5-manejo-de-mensajes-y-errores)
   - [Envío de Respuestas (`SendResponse`)](#1-envio-de-respuestas-sendresponse)
   - [Respuestas Predefinidas](#2-respuestas-predefinidas)
6. [Compilación y Ejecución](#6-compilacion-y-ejecucion)
7. [Demostración](#7-demostracion)

## **2. Arquitectura del Servidor**  

El servidor IRC está diseñado para manejar múltiples conexiones de clientes de manera eficiente. Para lograrlo, utiliza mecanismos como `poll()`, `fcntl()` y estructuras como `sockaddr_in` y `pollfd`. Estas herramientas permiten que el servidor gestione múltiples clientes sin necesidad de crear un hilo por cada conexión, lo que optimiza el rendimiento y evita bloqueos innecesarios.

---

### **Estructuras y Funciones Clave**  

#### **`sockaddr_in` – Configuración de la dirección del servidor**  
Cuando un servidor necesita escuchar conexiones entrantes, debe especificar en qué dirección y puerto lo hará. Aquí es donde entra `sockaddr_in`, una estructura fundamental para configurar el socket.

**Ejemplo de uso:**  
```cpp
struct sockaddr_in socketAddress;
socketAddress.sin_family = AF_INET;          // IPv4
socketAddress.sin_port = htons(this->_port); // Puerto en formato de red
socketAddress.sin_addr.s_addr = INADDR_ANY;  // Escucha en todas las interfaces
```
**¿Por qué se usa?** Permite al servidor definir su punto de escucha en la red, asegurando que pueda recibir conexiones de los clientes.

#### **`pollfd` – Monitoreo de múltiples conexiones**  
El servidor IRC debe gestionar varios clientes al mismo tiempo sin bloquearse. `pollfd` permite hacerlo al monitorear múltiples descriptores de archivo.

**Ejemplo de uso:**  
```cpp
struct pollfd newPoll;
newPoll.fd = _mainSocket;    // Descriptor de archivo a monitorear
newPoll.events = POLLIN;     // Detecta datos disponibles para leer
newPoll.revents = 0;         // Se llena automáticamente con los eventos ocurridos
```
**¿Por qué se usa?** Permite al servidor reaccionar solo cuando hay actividad en los sockets, evitando ciclos innecesarios y mejorando la eficiencia.

#### **`poll()` – Espera de eventos en los sockets**  
En el bucle principal del servidor, `poll()` es clave para detectar actividad en los sockets de manera eficiente.

**Ejemplo de uso:**  
```cpp
poll(&_pollSocketFds[0], _pollSocketFds.size(), -1);
```
**Opciones clave:**  
- `POLLIN`: Detecta si hay datos disponibles para leer.  
- `POLLOUT`: Detecta si el socket está listo para escribir.  

**¿Por qué se usa?** Permite manejar múltiples conexiones en un solo hilo, evitando la sobrecarga de procesos adicionales.

#### **`fcntl()` – Configuración del socket en modo no bloqueante**  
Si el servidor esperara datos en un socket de manera bloqueante, se detendría hasta que llegara algo, lo que no es viable en un entorno con múltiples clientes.

 **Ejemplo de uso:**  
```cpp
fcntl(_mainSocketFd, F_SETFL, O_NONBLOCK);
```
**Opción clave:**  
- `O_NONBLOCK`: Permite que el socket funcione sin bloquear la ejecución del programa.

**¿Por qué se usa?** Evita que el servidor se quede atascado esperando datos, permitiendo que siga gestionando otros clientes sin interrupciones.

---

### **Flujo de Trabajo del Servidor**  

El servidor IRC sigue un proceso estructurado para manejar conexiones y datos de manera eficiente.

#### **1. Inicialización (`ServerInit`)**  
- Se crea un socket con `socket()`.  
- Se configura para reutilizar direcciones (`SO_REUSEADDR`) y operar sin bloqueos (`O_NONBLOCK`).  
- Se enlaza a una dirección y puerto con `bind()`.  
- Se pone en modo de escucha con `listen()`.  
- Se añade el socket principal a `pollfd` para monitorear nuevas conexiones.  

#### **2. Bucle Principal (`ServerLoop`)**  
- Se ejecuta `poll()` para detectar actividad en los sockets.  
- Si hay actividad en el socket principal, se acepta una nueva conexión con `accept()`.  
- Si hay actividad en un cliente, se leen sus datos con `recv()`.  

#### **3. Aceptación de Clientes (`AcceptNewClient`)**  
- `accept()` recibe la conexión y devuelve un nuevo descriptor de archivo para el cliente.  
- Se configura en modo no bloqueante y se añade a `pollfd`.  

#### **4. Recepción de Datos (`ReceiveNewData`)**  
- Se leen los datos con `recv()`.  
- Si el cliente se desconecta (`recv()` devuelve 0), se eliminan sus recursos.  
- Si hay datos, se procesan y ejecutan los comandos apropiados.  

Gracias a esta arquitectura, el servidor IRC puede manejar múltiples conexiones de manera eficiente, evitando bloqueos y optimizando el uso de los recursos del sistema.


## **3. Comandos Básicos y de Operadores** 
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
  - `port`: Puerto en el que escucha el servidor.  
  - `mainSocketFd`: Descriptor del socket principal.  
  - `clients`: Lista de clientes conectados.  
  - `channels`: Lista de canales creados.  
  - `pollSocketFds`: Descriptores de archivo monitoreados por `poll()`.  

#### **2. `Channel`**  
- **Propósito**: Representa un canal de chat.  
- **Datos clave**:  
  - `name`: Nombre del canal.  
  - `clients`: Lista de usuarios en el canal.  
  - `admins`: Lista de operadores del canal.  
  - `modes`: Modos del canal (invite-only, contraseña, límite de usuarios, etc.).  

#### **3. `Client`**  
- **Propósito**: Representa un usuario conectado.  
- **Datos clave**:  
  - `fd`: Descriptor de archivo del socket del cliente.  
  - `nickname` y `_username`: Identificadores del usuario.  
  - `buffer`: Almacena datos recibidos hasta que se procesan.  
  - `myInviteChannels`: Canales a los que el usuario ha sido invitado.  

📖 **Más detalles**: Consulta [ESTRUCTURAS.md](docs/ESTRUCTURAS.md) para una explicación completa de estas estructuras y su implementación. 


## **5. Manejo de Mensajes y Errores**  

Para que la comunicación en el servidor IRC sea clara y eficiente, es fundamental contar con un sistema robusto de envío de respuestas y manejo de errores. Esto no solo garantiza que los usuarios reciban retroalimentación inmediata, sino que también protege la estabilidad del servidor ante situaciones inesperadas.

---

### **1. Envío de Respuestas (`SendResponse`)**  
El servidor envía mensajes a los clientes a través de la función `SendResponse`, ubicada en `src/functions/SendResponse`. Esta función es clave para asegurar una comunicación fluida.

**¿Cómo funciona?**
- Antes de enviar datos, el servidor verifica si el socket del cliente está listo para escribir usando `poll()`.  
- Esto evita bloqueos y asegura que el servidor pueda seguir procesando otras conexiones sin interrupciones.

**Ejemplo de uso:**
```cpp
struct pollfd pfd;

pfd.fd = clientFd;
pfd.events = POLLOUT;
poll(&pfd, 1, TIMEOUT);

if (pfd.revents & POLLOUT)
    send(clientFd, message.c_str(), message.length(), 0);
```
**¿Por qué se usa?**
- **Evita bloqueos**: No se queda esperando indefinidamente a que el socket esté listo.  
- **Mejora el rendimiento**: Permite que el servidor gestione múltiples clientes simultáneamente.  

---

### **2. Respuestas Predefinidas**  
Para mantener un formato estandarizado en las respuestas, el servidor utiliza macros definidas en `includes/comunication/`. Estas macros facilitan la generación de mensajes según el protocolo IRC.

#### **Respuestas de éxito (`RPL_*`)** ✅
Mensajes que indican acciones exitosas:
- `RPL_CONNECTED`: Confirma la conexión del usuario al servidor.  
- `RPL_JOINMSG`: Notifica a los usuarios cuando alguien se une a un canal.  
- `RPL_TOPICIS`: Informa sobre el tema actual de un canal.  

#### **Manejo de Errores (`ERR_*`)** ❎
Los errores informan a los usuarios sobre problemas o restricciones:
- `ERR_NICKINUSE`: El apodo seleccionado ya está en uso.  
- `ERR_NOTOPERATOR`: Se intenta ejecutar un comando de operador sin los permisos necesarios.  
- `ERR_INCORPASS`: La contraseña ingresada es incorrecta.  

**Ejemplo de error en código:**
```cpp
SendResponse(ERR_NICKINUSE("Bob"), clientFd); // Informa que el apodo 'Bob' está en uso
```
**¿Por qué se usa?**
- **Código más claro**: Las macros hacen que el código sea más fácil de leer y mantener.  
- **Estandarización**: Todos los mensajes siguen el protocolo IRC, asegurando compatibilidad.  

---

El sistema de manejo de mensajes y errores del servidor IRC sigue este diseño por varias razones:

**Claridad**: Las macros predefinidas permiten escribir código más limpio y fácil de entender.  
**Eficiencia**: `poll()` optimiza el envío de datos evitando bloqueos innecesarios.  
**Robustez**: Un buen manejo de errores previene fallos inesperados y mejora la experiencia del usuario.  

## **6. Compilación y Ejecución**  

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


## 7. Demostración

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

◦ Email Oseivane: oseivane@student.42barcelona.com

◦ Email gmacias-: gmacias-@student.42barcelona.com
