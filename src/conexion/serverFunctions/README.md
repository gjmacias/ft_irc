# Guía de Comandos para Autenticación, Mensajería y Moderación en IRC

Este documento describe los comandos esenciales para la autenticación, gestión de canales y moderación en un servidor IRC, junto con ejemplos correctos e incorrectos de su uso.

### **Índice**  

1. [PASS - Autenticación con contraseña](#pass)  
2. [NICK - Asignar un apodo](#nick)  
3. [USER - Registro de usuario](#user)  
4. [JOIN - Unirse a un canal](#join)  
5. [PART - Salir de un canal](#part)  
6. [PRIVMSG - Enviar un mensaje privado](#privmsg)  
7. [KICK - Expulsar a un usuario](#kick)  
8. [INVITE - Invitar a un usuario a un canal](#invite)  
9. [TOPIC - Cambiar el tema de un canal](#topic)  
10. [MODE - Configurar modos en un canal](#mode)  

---

## **PASS**  

El comando `PASS` se utiliza para autenticar al usuario en el servidor IRC proporcionando una contraseña. Este comando es opcional en algunos servidores, pero si se requiere, debe enviarse **antes** de cualquier otro comando como `NICK` o `USER`.  

### **Uso y Sintaxis**  
La sintaxis correcta del comando es:  
```
PASS <contraseña>
```  
Donde `<contraseña>` es la clave establecida por el servidor.  
En IRC, los parámetros que comienzan con `:` se interpretan como un único argumento extendido hasta el final de la línea.
  
### **Detalles Importantes**  

1. **Debe incluir un parámetro válido**  
   - Si no se proporciona una contraseña, el servidor responde con un error.  
   - **Ejemplo incorrecto:**  
     ```
     PASS
     ```
     **Respuesta del servidor:**  
     ```
     461 * PASS :Not enough parameters
     ```
     *Falta la contraseña.*  

2. **Validaciones de caracteres**  
   - Generalmente, los servidores IRC permiten contraseñas alfanuméricas y algunos caracteres especiales, pero pueden restringir ciertos símbolos problemáticos.  
   - **Caracteres inválidos comunes:**  
     - Espacios (` `) → Muchas redes no permiten espacios en contraseñas.  
     - Saltos de línea (`\n`, `\r`) → No permitidos, pueden romper el protocolo.  
     - Caracteres de control (`\x00` a `\x1F`) → Pueden causar errores inesperados.  
   - **Ejemplo incorrecto:**  
     ```
     PASS clave con espacio
     ```
     **Posible respuesta del servidor:**  
     ```
     464 * :Password incorrect
     ```
     *El servidor podría rechazarla por formato inválido.*  

3. **Longitud mínima y máxima**  
   - Depende del servidor, pero generalmente:  
     - **Mínimo:** 3-5 caracteres  
     - **Máximo:** 32-64 caracteres  
   - **Ejemplo incorrecto:**  
     ```
     PASS 12
     ```
     **Posible respuesta del servidor:**  
     ```
     464 * :Password incorrect
     ```
     *Demasiado corta, puede ser rechazada.*  

### **Ejemplos Correctos**  

✅ **Correcto:**  
```
PASS miSuperSecretaClave
PASS :mi Super Secreta Clave
```
---

## **NICK**  

El comando `NICK` se utiliza para establecer o cambiar el apodo (nickname) de un usuario en un servidor IRC. Este apodo es único dentro del servidor y se usará para identificar al usuario en los canales y en las interacciones con otros usuarios.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
NICK <apodo>
```  
Donde `<apodo>` es el nombre que el usuario desea utilizar en el servidor.  

Ejemplo correcto:  
```
NICK Juan123
```  

### **Detalles Importantes**  

1. **El apodo es obligatorio**  
   - Si el usuario no proporciona un apodo, el servidor responderá con un error.  
   - **Ejemplo incorrecto:**  
     ```
     NICK
     ```
     **Respuesta del servidor:**  
     ```
     431 * :No nickname given
     ```
     *El servidor requiere un apodo para continuar.*  

2. **El apodo puede comenzar con `:`**  
   - A diferencia de otros comandos donde `:` indica un argumento extendido, en el caso de `NICK` no es necesario, pero tampoco genera un error.  
   - **Ejemplo válido:**  
     ```
     NICK :Juan123
     ```
     *El servidor interpretará el apodo correctamente.*  

3. **Restricciones de caracteres**  
   - Un apodo debe comenzar con una **letra** (`A-Z`, `a-z`) y puede contener:  
     - Letras (`A-Z`, `a-z`)  
     - Números (`0-9`)  
     - Guiones (`-`) y guiones bajos (`_`)  
   - **Caracteres prohibidos:**  
     - Espacios  
     - `@`, `#`, `:`, `,`, `!`, `?`, `*`, `/`, `.` (en algunos servidores)  
     - Caracteres de control (`\x00` a `\x1F`)  
   - **Ejemplo incorrecto:**  
     ```
     NICK Juan 123
     ```
     **Respuesta del servidor:**  
     ```
     432 * Juan 123 :Erroneous nickname
     ```
     *No se permiten espacios en el apodo.*  

     ```
     NICK @Usuario!
     ```
     **Respuesta del servidor:**  
     ```
     432 * @Usuario! :Erroneous nickname
     ```
     *El apodo contiene caracteres no permitidos.*  

4. **Longitud mínima y máxima**  
   - Depende del servidor, pero generalmente:  
     - **Mínimo:** 1 carácter  
     - **Máximo:** 9-30 caracteres (según la red IRC)  
   - **Ejemplo incorrecto (apodo demasiado largo):**  
     ```
     NICK EsteEsUnApodoDemasiadoLargo123
     ```
     **Respuesta del servidor:**  
     ```
     432 * EsteEsUnApodoDemasiadoLargo123 :Erroneous nickname
     ```
     *El apodo excede la longitud permitida.*  

5. **El apodo no debe estar en uso**  
   - Si otro usuario ya está utilizando el apodo, el servidor lo rechazará.  
   - **Ejemplo incorrecto:**  
     ```
     NICK Juan123
     ```
     **Respuesta del servidor:**  
     ```
     433 * Juan123 :Nickname is already in use
     ```
     *El apodo ya está registrado o en uso.*
     
6. **El apodo no puede comenzar con un número**  
   - El primer carácter del apodo debe ser una **letra** (`A-Z`, `a-z`).  
   - **Ejemplo incorrecto:**  
     ```
     NICK 123Juan
     ```
     **Respuesta del servidor:**  
     ```
     432 * 123Juan :Erroneous nickname
     ```
     *El apodo no puede empezar con un número.*  

### **Ejemplos Correctos**  

✅ **Asignar un apodo al conectar:**  
```
NICK Juan123
```  

✅ **Cambiar de apodo durante una sesión:**  
```
NICK NuevoApodo
```  

✅ **Usar `:` antes del apodo (opcional pero válido):**  
```
NICK :Carlos
```  
### **Resumen**  
- El comando `NICK` asigna o cambia el apodo del usuario.  
- Puede comenzar con `:`, pero **no puede contener espacios ni caracteres prohibidos**.  
- Si el apodo está en uso, el servidor devuelve `433 :Nickname is already in use`.  
- Si el apodo es inválido, se devuelve `432 :Erroneous nickname`.  

---
## **USER**  

El comando `USER` se utiliza para registrar un usuario en el servidor IRC. Este comando es obligatorio para completar la autenticación y establecer la identidad del usuario en la red.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
USER <username> <hostname> <servername> <realname>
```  
Donde:  
- `<username>` → Identificador del usuario en el servidor.  
- `<hostname>` → Nombre del host desde el que se conecta el usuario (generalmente ignorado por el servidor).  
- `<servername>` → Nombre del servidor al que el usuario se está conectando.  
- `<realname>` → Nombre real del usuario (puede contener espacios si se usa `:` antes).  

Ejemplo correcto:  
```
USER Juan localhost irc.example.com :Juan Perez
```  

### **Detalles Importantes**  

1. **Todos los parámetros son obligatorios**  
   - Si falta alguno, el servidor responderá con un error.  
   - **Ejemplo incorrecto:**  
     ```
     USER Juan localhost irc.example.com
     ```
     **Respuesta del servidor:**  
     ```
     461 USER :Not enough parameters
     ```
     *Falta el nombre real (`realname`).*  

2. **El campo `<realname>` puede contener espacios**  
   - Para incluir espacios en el nombre real, se debe anteponer `:`.  
   - **Ejemplo válido:**  
     ```
     USER Juan localhost irc.example.com :Juan Perez
     ```
   - Si no se usa `:`, el servidor tomará solo la primera palabra como nombre real.  

3. **Restricciones en `<username>`**  
   - Debe comenzar con una letra y solo puede contener letras (`A-Z`, `a-z`), números (`0-9`), guiones (`-`), y guiones bajos (`_`).  
   - **Ejemplo incorrecto:**  
     ```
     USER 123Juan localhost irc.example.com :Juan Perez
     ```
     **Respuesta del servidor:**  
     ```
     432 123Juan :Erroneous username
     ```
     *El `username` no puede empezar con un número.*  

4. **El `<hostname>` y `<servername>` son ignorados en la mayoría de los servidores**  
   - Se deben incluir, pero el servidor los sobrescribirá con la información real de la conexión.  

5. **El comando `USER` solo puede ejecutarse una vez por sesión**  
   - Si el usuario intenta enviarlo nuevamente después de registrarse, el servidor lo rechazará.  
   - **Ejemplo incorrecto:**  
     ```
     USER Juan localhost irc.example.com :Juan Perez
     ```
     **Respuesta del servidor:**  
     ```
     462 * :You may not reregister
     ```
     *El usuario ya está registrado y no puede ejecutar `USER` nuevamente.*  

### **Ejemplos Correctos**  

✅ **Registro de usuario con nombre real con espacios:**  
```
USER Juan localhost irc.example.com :Juan Perez
```  

✅ **Registro de usuario con nombre real sin espacios:**  
```
USER Juan localhost irc.example.com JuanPerez
```  

### **Resumen**  
- `USER` es obligatorio para completar la conexión en IRC.  
- Se debe proporcionar **todos** los parámetros: `<username>`, `<hostname>`, `<servername>` y `<realname>`.  
- `<username>` **no puede empezar con un número ni contener caracteres prohibidos**.  
- `<realname>` puede incluir espacios si se antepone `:`.  
- Solo se permite un `USER` por sesión; intentar enviarlo de nuevo dará error `462`.  

---

## **JOIN**  

El comando `JOIN` permite a un usuario unirse a uno o varios canales en un servidor IRC. Es necesario para participar en conversaciones y recibir mensajes dentro del canal.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
JOIN <#canal> (<contraseña>)
```  
Donde:  
- `<#canal>` → Es el nombre del canal al que se desea unir el usuario.  

Ejemplo correcto:  
```
JOIN #miCanal
```  
También es posible unirse a múltiples canales en un solo comando:  
```
JOIN #canal1,#canal2,#canal3
```  
Si alguno de los canales requiere clave, se pueden proporcionar en el mismo orden separadas por comas:  
```
JOIN #canal1,#canal2,#canal3 clave1,clave2
```  
- En este caso:  
  - `#canal1` requiere `clave1`.  
  - `#canal2` requiere `clave2`.
  - `#canal3` no necesita clave.   
- Si hay más canales que claves, los canales sin clave adicional se intentarán unir sin contraseña.  
- Si hay más claves que canales, las claves sobrantes son ignoradas.  

### **Detalles Importantes**  

1. **El nombre del canal debe comenzar con `#`**  
   - Los canales en IRC deben empezar con un prefijo (`#`, `&`, `+` o `!`, dependiendo del servidor).  
   - **Ejemplo incorrecto:**  
     ```
     JOIN miCanal
     ```
     **Respuesta del servidor:**  
     ```
     403 miCanal :No such channel
     ```
     *El canal no existe porque falta el `#`.*  

2. **Se puede unir a varios canales a la vez**  
   - **Ejemplo correcto:**  
     ```
     JOIN #canal1,#canal2,#canal3
     ```
   - Si alguno de los canales no existe, el servidor devolverá un error solo para ese canal.  

3. **Si un canal requiere clave y no se proporciona, el usuario no podrá unirse a él**  
   - **Ejemplo incorrecto:**  
     ```
     JOIN #canal1,#privado
     ```
     **Respuesta del servidor (si `#privado` requiere clave):**  
     ```
     475 #privado :Cannot join channel (+k)
     ```
     *El usuario se une a `#canal1`, pero no a `#privado` porque falta la clave.*  

4. **El usuario ya está en el canal**  
   - No es posible unirse a un canal en el que ya se encuentra.  
   - **Ejemplo incorrecto:**  
     ```
     JOIN #miCanal
     ```
     **Respuesta del servidor:**  
     ```
     443 Juan123 #miCanal :is already on channel
     ```
     *El usuario ya está en el canal.*  

### **Ejemplos Correctos**  

✅ **Unirse a un canal público:**  
```
JOIN #miCanal
```  

✅ **Unirse a varios canales a la vez:**  
```
JOIN #canal1,#canal2,#canal3
```  

✅ **Unirse a un canal con contraseña:**  
```
JOIN #seguro clave123
```  

✅ **Unirse a varios canales con claves correspondientes:**  
```
JOIN #privado,#vip,#canal1 clave1,clave2
```
- `#privado` requiere `clave1`.  
- `#vip` requiere `clave2`.  
- `#canal1` no necesita clave.  

✅ **Unirse a varios canales, ignorando claves adicionales:**  
```
JOIN #canal1,#canal2 clave1,clave2,claveExtra
```
- `claveExtra` es ignorada porque hay más claves que canales.  

### **Resumen**  
- `JOIN` permite unirse a uno o más canales en IRC.  
- **El nombre del canal debe empezar con `#`**.  
- **Se pueden unir varios canales a la vez, separándolos con comas (`JOIN #canal1,#canal2`)**.  
- **Si un canal requiere clave, esta debe proporcionarse en el mismo orden (`JOIN #canal1,#canal2 clave1,clave2`)**.  
- **Si el usuario ya está en el canal, recibirá el error `443`**.  

---

## **PART**  

El comando `PART` permite a un usuario salir de un canal en un servidor IRC. Es la forma estándar de abandonar un canal sin desconectarse del servidor.  
### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
PART <#canal> (<mensaje>)
```  
Donde:  
- `<#canal>` → Es el nombre del canal que el usuario desea abandonar.  
- `<mensaje>` (opcional) → Es un mensaje de despedida que se mostrará a los demás usuarios del canal.  

Ejemplo correcto:  
```
PART #miCanal
```  

Ejemplo con mensaje de salida:  
```
PART #miCanal Hasta luego, nos vemos!
```  

### **Detalles Importantes**  

1. **El nombre del canal debe comenzar con `#`**  
   - Los canales deben comenzar con `#`, `&`, `+` o `!`, dependiendo del servidor.  
   - **Ejemplo incorrecto:**  
     ```
     PART miCanal
     ```
     **Respuesta del servidor:**  
     ```
     403 miCanal :No such channel
     ```
     *El canal no existe porque falta el `#`.*  

2. **El usuario debe estar en el canal para abandonarlo**  
   - No se puede salir de un canal en el que no se encuentra.  
   - **Ejemplo incorrecto:**  
     ```
     PART #canalDondeNoEstoy
     ```
     **Respuesta del servidor:**  
     ```
     442 #canalDondeNoEstoy :You're not on that channel
     ```
     *El usuario no está en ese canal.*  

3. **Se puede abandonar varios canales a la vez**  
   - **Ejemplo correcto:**  
     ```
     PART #canal1,#canal2,#canal3
     ```
   - **Ejemplo con mensaje de salida:**  
     ```
     PART #canal1,#canal2 :Adiós a todos!
     ```
     *El mensaje de salida se aplica a todos los canales en la lista.*  

4. **Si el usuario no especifica un canal, el comando es inválido**  
   - **Ejemplo incorrecto:**  
     ```
     PART
     ```
     **Respuesta del servidor:**  
     ```
     461 PART :Not enough parameters
     ```
     *Falta el nombre del canal.*  

### **Ejemplos Correctos**  

✅ **Salir de un canal:**  
```
PART #miCanal
```  

✅ **Salir de un canal con un mensaje de despedida:**  
```
PART #miCanal :Hasta la próxima!
```  

✅ **Salir de varios canales al mismo tiempo:**  
```
PART #canal1,#canal2,#canal3
```  

✅ **Salir de varios canales con un mensaje de despedida:**  
```
PART #canal1,#canal2 :Adiós amigos!
```  

### **Resumen**  
- `PART` permite salir de un canal o varios al mismo tiempo.  
- **El nombre del canal debe empezar con `#`**.  
- **Si no se está en el canal, el servidor devuelve el error `442`**.  
- **Si no se especifica ningún canal, el servidor devuelve el error `461`**.  
- **Es posible añadir un mensaje de despedida opcional.**  

---

## **PRIVMSG**  

El comando `PRIVMSG` permite enviar un mensaje privado a uno o varios destinatarios en un servidor IRC. Puede dirigirse tanto a usuarios como a canales.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
PRIVMSG <destinatario>(,<destinatario2>,...) :<mensaje>
```  
Donde:  
- `<destinatario>` → Puede ser un **usuario** o un **canal** (`#canal`).  
- `,<destinatario2>,...` (opcional) → Se pueden especificar **múltiples destinatarios** separados por comas.  
- `:<mensaje>` → Es el contenido del mensaje. **Debe ir precedido por `:` si contiene espacios.**  

Ejemplo correcto enviando un mensaje a un canal:  
```
PRIVMSG #miCanal :Hola a todos
```  

Ejemplo correcto enviando un mensaje privado a un usuario:  
```
PRIVMSG Juan123 :Hola, ¿cómo estás?
```  

Ejemplo correcto enviando un mensaje a varios destinatarios (usuarios y canales a la vez):  
```
PRIVMSG #miCanal,Juan123 :¡Buenos días a todos!
```  
### **Detalles Importantes**  

1. **El destinatario debe ser válido (usuario o canal existente)**  
   - **Ejemplo incorrecto (usuario no existe):**  
     ```
     PRIVMSG UsuarioInexistente :Hola
     ```
     **Respuesta del servidor:**  
     ```
     401 UsuarioInexistente :No such nick/channel
     ```
     *El usuario o canal no existe.*  

   - **Ejemplo incorrecto (canal no existe):**  
     ```
     PRIVMSG #canalInexistente :Hola
     ```
     **Respuesta del servidor:**  
     ```
     403 #canalInexistente :No such channel
     ```
     *El canal no existe.*  

2. **El mensaje no puede estar vacío**  
   - **Ejemplo incorrecto:**  
     ```
     PRIVMSG Juan123
     ```
     **Respuesta del servidor:**  
     ```
     412 :No text to send
     ```
     *No se proporcionó un mensaje.*  

3. **Si el destinatario es un canal, el usuario debe estar en él (salvo excepciones del servidor)**  
   - **Ejemplo incorrecto:**  
     ```
     PRIVMSG #canalPrivado :Hola
     ```
     **Respuesta del servidor:**  
     ```
     404 #canalPrivado :Cannot send to channel
     ```
     *El usuario no tiene permiso para enviar mensajes a ese canal.*  

4. **El mensaje debe incluir `:` si contiene espacios**  
   - **Ejemplo correcto:**  
     ```
     PRIVMSG Juan123 :Hola, ¿cómo estás?
     ```
   - **Ejemplo incorrecto:**  
     ```
     PRIVMSG Juan123 Hola, ¿cómo estás?
     ```
     **Respuesta del servidor:**  
     ```
     461 PRIVMSG :Not enough parameters
     ```
     *El mensaje es inválido porque no está precedido por `:`.*  

5. **Se pueden enviar mensajes a múltiples destinatarios**  
   - **Ejemplo correcto:**  
     ```
     PRIVMSG #miCanal,Juan123 :¡Saludos a todos!
     ```
     *El mensaje se envía tanto al canal como al usuario.*  
   - **Ejemplo incorrecto (uno de los destinatarios no existe):**  
     ```
     PRIVMSG #miCanal,UsuarioInexistente :Hola
     ```
     **Respuesta del servidor:**  
     ```
     401 UsuarioInexistente :No such nick/channel
     ```
     *Uno de los destinatarios no existe, pero el mensaje aún se enviará a los destinatarios válidos.*  

### **Ejemplos Correctos**  

✅ **Enviar un mensaje a un usuario:**  
```
PRIVMSG Juan123 :Hola, ¿qué tal?
```  

✅ **Enviar un mensaje a un canal:**  
```
PRIVMSG #miCanal :Buenos días, equipo.
```  

✅ **Enviar un mensaje sin espacios (no requiere `:`):**  
```
PRIVMSG Juan123 Hola
```  

✅ **Enviar un mensaje con múltiples palabras (requiere `:`):**  
```
PRIVMSG Juan123 :Nos vemos más tarde en el canal.
```  

✅ **Enviar un mensaje a múltiples destinatarios:**  
```
PRIVMSG #miCanal,Juan123 :¡Hola a todos!
```  

### **Resumen**  
- `PRIVMSG` permite enviar mensajes a **usuarios**, **canales** o **múltiples destinatarios** a la vez.  
- **El mensaje debe ir precedido por `:` si contiene espacios.**  
- **Si el destinatario no existe, el servidor devuelve el error `401` o `403`.**  
- **Si el usuario no tiene permiso para escribir en un canal, recibirá el error `404`.**  
- **Si no se especifica mensaje, el error será `412`.**  
- **Se pueden incluir varios destinatarios separados por comas.**  

---

## **KICK**  

El comando `KICK` permite a un operador de canal **expulsar a un usuario** de un canal en IRC. Se utiliza para mantener el orden y hacer cumplir las normas del canal.  
### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
KICK <#canal> <usuario> (:<razón>)
```  
Donde:  
- `<#canal>` → Especifica el canal del que se expulsará al usuario.  
- `<usuario>` → Es el nombre del usuario que será expulsado.  
- `:<razón>` (opcional) → Explicación de por qué el usuario fue expulsado. Si contiene espacios, **debe ir precedida por `:`**.  

Ejemplo correcto expulsando a un usuario con razón:  
```
KICK #miCanal Juan123 :No respetó las reglas
```  

Ejemplo correcto sin razón:  
```
KICK #miCanal Juan123
```  
*(El servidor usará un mensaje por defecto, como "*Expulsado del canal*".)*  

### **Detalles Importantes**  

1. **Solo los operadores (`+o`) pueden usar `KICK`**  
   - **Ejemplo incorrecto (usuario sin permisos intenta expulsar a otro):**  
     ```
     KICK #miCanal Juan123 :Fuera de aquí
     ```
     **Respuesta del servidor:**  
     ```
     482 #miCanal :You're not channel operator
     ```
     *El usuario no tiene permisos para expulsar a otros del canal.*  

2. **El usuario debe estar en el canal**  
   - **Ejemplo incorrecto (intento de expulsar a un usuario que no está en el canal):**  
     ```
     KICK #miCanal UsuarioInexistente :Razón
     ```
     **Respuesta del servidor:**  
     ```
     441 UsuarioInexistente #miCanal :They aren't on that channel
     ```
     *El usuario no está en el canal.*  

3. **El canal debe existir y el usuario debe estar en él**  
   - **Ejemplo incorrecto (expulsión en un canal inexistente):**  
     ```
     KICK #canalFalso Juan123 :No respetó las normas
     ```
     **Respuesta del servidor:**  
     ```
     403 #canalFalso :No such channel
     ```
     *El canal no existe.*  

4. **El mensaje de razón es opcional, pero si hay espacios, debe ir precedido de `:`**  
   - **Ejemplo correcto:**  
     ```
     KICK #miCanal Juan123 :Incumplió las normas
     ```
   - **Ejemplo incorrecto:**  
     ```
     KICK #miCanal Juan123 Incumplió las normas
     ```
     **Respuesta del servidor:**  
     ```
     461 KICK :Not enough parameters
     ```
     *La razón contiene espacios pero no tiene `:` delante.*  

5. **Se pueden expulsar múltiples usuarios a la vez**  
   - **Ejemplo correcto:**  
     ```
     KICK #miCanal Juan123,Pedro456 :Reglas rotas
     ```
     *Ambos usuarios serán expulsados del canal con la misma razón.*  

### **Ejemplos Correctos**  

✅ **Expulsar a un usuario con una razón:**  
```
KICK #miCanal Juan123 :Lenguaje inapropiado
```  

✅ **Expulsar a un usuario sin razón específica:**  
```
KICK #miCanal Juan123
```  

✅ **Expulsar a múltiples usuarios a la vez:**  
```
KICK #miCanal Juan123,Pedro456 :Spam
```
### **Resumen**  
- **`KICK` permite expulsar a usuarios de un canal, pero solo operadores pueden usarlo.**  
- **Se pueden expulsar múltiples usuarios separándolos por comas.**  
- **Si se especifica una razón con espacios, debe ir precedida por `:`.**  
- **Si el usuario no está en el canal, se devuelve el error `441`.**  
- **Si el canal no existe, el error será `403`.**  
- **Si el usuario no es operador, el error será `482`.**

---

## **INVITE**  

El comando `INVITE` permite a un usuario con los permisos adecuados **invitar a otro usuario a un canal** en IRC. Es especialmente útil para canales privados (`+i`), donde los usuarios no pueden unirse sin una invitación previa.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
INVITE <usuario> <#canal>
```  
Donde:  
- `<usuario>` → Es el nombre del usuario que se quiere invitar.  
- `<#canal>` → Es el canal al que se enviará la invitación.  

Ejemplo correcto invitando a un usuario a un canal:  
```
INVITE Juan123 #miCanal
```  

### **Detalles Importantes**  

1. **El usuario que invita debe estar en el canal**  
   - **Ejemplo incorrecto (usuario intenta invitar a alguien sin estar en el canal):**  
     ```
     INVITE Juan123 #miCanal
     ```
     **Respuesta del servidor:**  
     ```
     442 #miCanal :You're not on that channel
     ```
     *El usuario no está en el canal y, por lo tanto, no puede invitar a otros.*  

2. **Si el canal tiene modo `+i` (invitación requerida), solo los operadores (`+o`) pueden invitar**  
   - **Ejemplo incorrecto (usuario sin privilegios intenta invitar a otro en un canal privado):**  
     ```
     INVITE Juan123 #canalPrivado
     ```
     **Respuesta del servidor:**  
     ```
     482 #canalPrivado :You're not channel operator
     ```
     *Solo los operadores pueden invitar a usuarios a canales con modo `+i`.*  

3. **El usuario invitado debe existir en el servidor**  
   - **Ejemplo incorrecto (intento de invitar a un usuario que no existe):**  
     ```
     INVITE UsuarioInexistente #miCanal
     ```
     **Respuesta del servidor:**  
     ```
     401 UsuarioInexistente :No such nick/channel
     ```
     *El usuario no existe.*  

4. **El usuario ya está en el canal**  
   - **Ejemplo incorrecto (intento de invitar a alguien que ya está en el canal):**  
     ```
     INVITE Juan123 #miCanal
     ```
     **Respuesta del servidor:**  
     ```
     443 Juan123 #miCanal :Is already on channel
     ```
     *El usuario ya forma parte del canal y no necesita invitación.*  

5. **Confirmación de invitación exitosa**  
   - Si la invitación es exitosa, el servidor responderá con:  
     ```
     341 <tuNick> <usuario> <#canal>
     ```
     *Confirmación de que la invitación fue enviada correctamente.*
     
### **Ejemplos Correctos**  

✅ **Invitar a un usuario a un canal público:**  
```
INVITE Juan123 #miCanal
```
🔹 **Respuesta del servidor:**  
```
341 TuNick Juan123 #miCanal
```

✅ **Invitar a un usuario a un canal privado (`+i`) cuando eres operador:**  
```
INVITE Juan123 #canalPrivado
```
🔹 **Respuesta del servidor:**  
```
341 TuNick Juan123 #canalPrivado
```
### **Resumen**  
- **`INVITE` se usa para invitar a un usuario a un canal.**  
- **El usuario que invita debe estar en el canal.**  
- **Si el canal es privado (`+i`), solo los operadores pueden invitar.**  
- **Si el usuario ya está en el canal, se recibe el error `443`.**  
- **Si el usuario no existe, se recibe el error `401`.**  
- **Si la invitación es exitosa, el servidor responde con `341`.**  
---

## **TOPIC**  

El comando `TOPIC` se utiliza para **cambiar el tema de un canal** en IRC. El tema de un canal suele ser una breve descripción o información que aparece en la parte superior del canal y puede ser visto por todos los usuarios que se encuentren en él.  

### **Uso y Sintaxis**  

La sintaxis correcta del comando es:  
```
TOPIC <#canal> :<tema>
```  
Donde:  
- `<#canal>` → Especifica el canal cuyo tema se desea cambiar.  
- `:<tema>` → Es el nuevo tema para el canal. **Debe ir precedido por `:`** si contiene espacios.  

Ejemplo correcto cambiando el tema de un canal:  
```
TOPIC #miCanal :Bienvenidos a todos
```  

### **Detalles Importantes**  

1. **Solo los operadores (`+o`) pueden cambiar el tema de un canal**  
   - **Ejemplo incorrecto (usuario sin privilegios intenta cambiar el tema):**  
     ```
     TOPIC #miCanal :Nuevo tema
     ```
     **Respuesta del servidor:**  
     ```
     482 #miCanal :You're not channel operator
     ```
     *Solo los operadores pueden cambiar el tema de un canal.*  

2. **El tema puede contener espacios, pero debe ir precedido por `:`**  
   - **Ejemplo incorrecto (falta el `:`):**  
     ```
     TOPIC #miCanal Nuevo tema para el canal
     ```
     **Respuesta del servidor:**  
     ```
     461 TOPIC :Not enough parameters
     ```
     *El tema debe ir precedido por `:` para indicar que contiene espacios.*  

3. **El canal debe existir**  
   - **Ejemplo incorrecto (intento de cambiar el tema de un canal que no existe):**  
     ```
     TOPIC #canalInexistente :Tema falso
     ```
     **Respuesta del servidor:**  
     ```
     403 #canalInexistente :No such channel
     ```
     *El canal no existe.*  

4. **El tema puede ser vacíado (dejar en blanco) para eliminar el tema del canal**  
   - **Ejemplo correcto (vaciar el tema de un canal):**  
     ```
     TOPIC #miCanal :
     ```
     *Este comando elimina el tema actual del canal.*  

5. **¿Qué pasa si no especifico un nuevo tema después de `TOPIC`?**  
   - Si un usuario ejecuta el comando `TOPIC <#canal>` sin especificar el tema (por ejemplo, solo escribiendo `TOPIC #miCanal`), el servidor devolverá el tema actual del canal, si es visible y accesible.  
   - **Ejemplo:**
     ```
     TOPIC #miCanal
     ```
     **Respuesta del servidor (si el canal tiene un tema):**  
     ```
     332 <tuNick> #miCanal :Bienvenidos al canal IRC de discusión
     ```

### **Ejemplos Correctos**  

✅ **Cambiar el tema de un canal a una descripción:**  
```
TOPIC #miCanal :Bienvenidos a todos
```  

✅ **Eliminar el tema de un canal:**  
```
TOPIC #miCanal :
```  

✅ **Cambiar el tema de un canal con espacios en el tema:**  
```
TOPIC #miCanal :Canal para discusión de IRC
```  

✅ **Ver el tema actual de un canal sin cambiarlo:**  
```
TOPIC #miCanal
```  
**Respuesta del servidor (si el canal tiene un tema):**  
```
332 <tuNick> #miCanal :Bienvenidos al canal IRC de discusión
```
### **Resumen**  
- **`TOPIC` permite cambiar o eliminar el tema de un canal.**  
- **El canal debe existir y el usuario debe ser operador (`+o`) para cambiar el tema.**  
- **El tema debe ir precedido de `:` si contiene espacios.**  
- **Si el canal no existe, el error será `403`.**  
- **Si el usuario no es operador, el error será `482`.**  
- **Si no se especifica un nuevo tema, el servidor devolverá el tema actual del canal.** 

---
## **MODE**  

El comando `MODE` se utiliza para **configurar modos en un canal** IRC. Los modos permiten modificar el comportamiento de un canal, como establecer restricciones de acceso o permisos adicionales para los usuarios. Los modos de canal son configurados por los operadores del canal (`+o`).  

En este caso, los modos que explicaremos son los siguientes:

- **`+l`**: Establece un límite de usuarios en el canal.
- **`+i`**: Convierte el canal en privado (requiere invitación para unirse).
- **`+k`**: Establece una contraseña para el canal (solo usuarios con la contraseña pueden unirse).
- **`+o`**: Asigna privilegios de operador a un usuario.
- **`+t`**: Restringe el cambio de tema solo a operadores.

### **Uso y Sintaxis**  

La sintaxis general del comando `MODE` es la siguiente:  
```
MODE <#canal> <modo> [parámetro]
```  
Donde:  
- `<#canal>` → Especifica el canal al que se aplicarán los modos.  
- `<modo>` → Es el modo que se desea establecer o quitar.  
- `[parámetro]` → Es un parámetro opcional, dependiendo del modo. Algunos modos requieren un parámetro, como una contraseña o un límite de usuarios.

### **Modos Explicados**  

#### **1. `+l <límite>` - Límite de usuarios**  

Este modo establece un límite de usuarios que pueden estar en el canal al mismo tiempo. Si el canal alcanza este límite, nuevos usuarios no podrán unirse hasta que haya espacio disponible.

**Ejemplo correcto:**
```
MODE #miCanal +l 50
```
Esto limita el canal a **50 usuarios**.

**Ejemplo incorrecto:**
```
MODE #miCanal +l
```
**Error:**  
```
461 MODE :Not enough parameters
```
*Falta especificar el límite de usuarios.*

#### **2. `+i` - Canal privado (invitación requerida)**  

El modo `+i` convierte un canal en privado, lo que significa que solo los usuarios que han sido invitados por un operador podrán unirse. Los usuarios deben ser invitados explícitamente para poder entrar en el canal.

**Ejemplo correcto:**
```
MODE #miCanal +i
```
Esto configura el canal para que sea privado y solo los usuarios invitados puedan unirse.

#### **3. `+k <contraseña>` - Establecer contraseña**  

El modo `+k` establece una contraseña en el canal. Los usuarios deben conocer esta contraseña para poder unirse al canal. La contraseña debe especificarse después del modo.

**Ejemplo correcto:**
```
MODE #miCanal +k Secreta123
```
Esto configura el canal con la contraseña `Secreta123`. Los usuarios deberán ingresar esta contraseña para poder unirse.

**Ejemplo incorrecto:**
```
MODE #miCanal +k
```
**Error:**  
```
461 MODE :Not enough parameters
```
*Falta la contraseña del canal.*

#### **4. `+o <usuario>` - Asignar privilegios de operador**  

El modo `+o` otorga privilegios de operador a un usuario. Los operadores tienen permisos especiales para cambiar los modos del canal, expulsar usuarios, y más.

**Ejemplo correcto:**
```
MODE #miCanal +o Juan123
```
Esto asigna privilegios de operador al usuario `Juan123`.
```
MODE #miCanal -o Juan123
```
Esto quita los privilegios de operador al usuario `Juan123`.


#### **5. `+t` - Solo operadores pueden cambiar el tema**  

El modo `+t` restringe el cambio de tema a solo los operadores del canal. Sin este modo, cualquier usuario en el canal puede cambiar el tema.

**Ejemplo correcto:**
```
MODE #miCanal +t
```
Esto configura el canal para que solo los operadores puedan cambiar el tema.

### **Resumen**  
- **`+l`**: Establece un límite de usuarios en el canal.
- **`+i`**: Hace que el canal sea privado (requiere invitación para unirse).
- **`+k`**: Establece una contraseña para el canal.
- **`+o`**: Otorga privilegios de operador a un usuario.
- **`+t`**: Restringe el cambio de tema a solo operadores.
