# Estructura de Datos

El sistema está compuesto por varios archivos que implementan las funcionalidades del servidor, los clientes y los canales de chat. A continuación, se detalla la estructura del proyecto.

### Estructura del Server.hpp

**1. ¿Cómo funciona el servidor IRC?**

![server](https://github.com/user-attachments/assets/c9861c76-ff0e-406f-8df9-a8efac9dc36d)

- **Señales**:El servidor está siempre escuchando para recibir mensajes. Si recibe señales especiales (como `ctlC`, `ctlD`, `ctl/`), debe cerrarse de forma segura.
- **Clientes**: El servidor guarda a los usuarios conectados en un **vector** de punteros. Un **vector** es como una lista que contiene direcciones (punteros) a los objetos de los clientes, no los objetos completos.
- **Canales**: El servidor también guarda información sobre los canales en un **vector**.
**4. Canales**
![channel](https://github.com/user-attachments/assets/263a2539-a5b7-4d85-a61c-217c9e9e952e)
Cada canal tiene:
- Un **nombre**
- Un **tema** o tópico
- Un **límite de usuarios**
- Un **vector de administradores** (punteros a los clientes administradores)
- Un **vector de usuarios normales** (punteros a los clientes normales)

Si un cliente es administrador de un canal, estará en el **vector de administradores**, y si es solo un usuario normal, estará en el **vector de usuarios normales**.

**5. Cliente**
![client](https://github.com/user-attachments/assets/d40f90cf-5ca0-4c91-942a-7ae883add3b1)
Cada cliente tiene:
- **fd**: Un identificador único usado para localizarlo rápidamente en los vectores.
- **Nickname**: Su nombre en el IRC.
- **Username**: Su nombre de usuario.
- **IP Address**: La dirección de su conexión.
- **Buffer de comandos**: Espacio donde se guardan los comandos del cliente.

### Explicación:
- El **servidor** organiza todo, conectando **clientes** y **canales**.
- **Clientes**: Son las personas conectadas, identificadas por su **fd** (un identificador único).
- **Canales**: Son grupos de chat. Cada canal tiene su propio **vector de usuarios** (normal y administrador).
  
### ¿Por qué usar punteros?

Al usar **punteros**, no necesitamos mover los objetos completos cuando cambiamos su ubicación en memoria. Solo movemos la **dirección** a la que apunta el puntero. Esto hace que el sistema sea más eficiente.

Esta es una descripción técnica, sencilla y clara de cómo funciona la estructura del servidor IRC.
