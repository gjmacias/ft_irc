# Guía de Comandos para Autenticación, Mensajería y Moderación en IRC

Este documento describe los comandos esenciales para la autenticación, gestión de canales y moderación en un servidor IRC, junto con ejemplos correctos e incorrectos de su uso.

---

## **1. Autenticación y Registro de Usuarios**

### **PASS <contraseña>**
Autentica al usuario con la contraseña del servidor.

**Ejemplo Correcto:**
```
PASS miSuperSecretaClave
```

**Ejemplo Incorrecto:**
```
PASS
```
*Falta la contraseña.*

---

### **NICK <apodo>**
Asigna un apodo (nickname) al usuario.

**Ejemplo Correcto:**
```
NICK Juan123
```

**Ejemplo Incorrecto:**
```
NICK
```
*No se proporciona un apodo.*

---

### **USER <username> <hostname> <servername> <realname>**
Registra al usuario en el servidor.

**Ejemplo Correcto:**
```
USER Juan localhost irc.example.com "Juan Perez"
```

**Ejemplo Incorrecto:**
```
USER Juan localhost irc.example.com
```
*Falta el nombre real.*

---

## **2. Gestión de Canales y Mensajería**

### **JOIN <canal>**
Une al usuario a un canal.

**Ejemplo Correcto:**
```
JOIN #miCanal
```

**Ejemplo Incorrecto:**
```
JOIN miCanal
```
*Falta el `#` antes del nombre del canal.*

---

### **PART <canal>**
Abandona un canal.

**Ejemplo Correcto:**
```
PART #miCanal
```

**Ejemplo Incorrecto:**
```
PART
```
*No se especifica un canal.*

---

### **PRIVMSG <destinatario> <mensaje>**
Envía un mensaje privado a un canal o usuario.

**Ejemplo Correcto:**
```
PRIVMSG #miCanal :Hola a todos
PRIVMSG Juan123 :Hola, ¿cómo estás?
```

**Ejemplo Incorrecto:**
```
PRIVMSG Juan123
```
*Falta el mensaje a enviar.*

---

## **3. Comandos de Operadores**

### **KICK <canal> <usuario>**
Expulsa a un usuario de un canal.

**Ejemplo Correcto:**
```
KICK #miCanal Juan123 :Razón del kick
```

**Ejemplo Incorrecto:**
```
KICK #miCanal
```
*Falta el usuario a expulsar.*

---

### **INVITE <usuario> <canal>**
Invita a un usuario a un canal.

**Ejemplo Correcto:**
```
INVITE Juan123 #miCanal
```

**Ejemplo Incorrecto:**
```
INVITE #miCanal Juan123
```
*El orden de los argumentos es incorrecto.*

---

### **TOPIC <canal> <tema>**
Cambia el tema de un canal.

**Ejemplo Correcto:**
```
TOPIC #miCanal :Bienvenidos a nuestro canal
```

**Ejemplo Incorrecto:**
```
TOPIC #miCanal
```
*Falta el nuevo tema.*

---

### **MODE <canal> <modo>**
Configura modos en un canal. Solo se incluyen los modos: `l`, `k`, `o`, `t`, `i`.

- `l <límite>`: Establece un límite de usuarios.
- `k <clave>`: Asigna una contraseña al canal.
- `o <usuario>`: Otorga privilegios de operador.
- `t`: Solo operadores pueden cambiar el tema.
- `i`: Convierte el canal en privado (invitación requerida para unirse).

**Ejemplos Correctos:**
```
MODE #miCanal +l 25  
MODE #miCanal +k secreto123  
MODE #miCanal +o Juan123  
MODE #miCanal +t  
MODE #miCanal +i  
```

**Ejemplos Incorrectos:**
```
MODE #miCanal +l
```
*Falta el límite de usuarios.*
```
MODE #miCanal +k
```
*Falta la contraseña del canal.*

---

## **Conclusión**
Esta guía proporciona una referencia rápida para el uso correcto de comandos básicos en IRC. Es importante seguir el formato correcto para evitar errores en la comunicación y gestión de canales.

