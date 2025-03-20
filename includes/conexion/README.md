# Estructura

El sistema está compuesto por varios archivos que implementan las funcionalidades del servidor, los clientes y los canales de chat. A continuación, se detalla la estructura del proyecto.

## Estructura del Server.hpp

![server](https://github.com/user-attachments/assets/c9861c76-ff0e-406f-8df9-a8efac9dc36d)

- **Señales**:El servidor está siempre escuchando para recibir mensajes. Si recibe señales especiales (como `ctlC`, `ctlD`, `ctl/`), debe cerrarse de forma segura.
- **Clientes**: El servidor guarda a los usuarios conectados en un **vector** de punteros a los objetos de los clientes, no los objetos completos.
- **Canales**: El servidor también guarda información sobre los canales en un **vector**.

## Estructura del Channels.hpp

![channel](https://github.com/user-attachments/assets/263a2539-a5b7-4d85-a61c-217c9e9e952e)

Cada canal tiene:
- Un **nombre**
- Un **tema** o tópico
- Un **límite de usuarios**
- Un **vector de administradores** (punteros a los clientes administradores)
- Un **vector de usuarios normales** (punteros a los clientes normales)

Si un cliente es administrador de un canal, estará en el vector de administradores, y si es solo un usuario normal, estará en el vector de usuarios normales. **JAMAS SE ENCONTRARÁ EN LOS DOS VECTORES**.

## Estructura del Client.hpp

![client](https://github.com/user-attachments/assets/127b0174-be2e-4a74-9861-ced450256083)

Cada cliente tiene:
- **fd**: Un identificador único usado para localizarlo rápidamente en los vectores.
- **Nickname**: Su nombre en el IRC.
- **Username**: Su nombre de usuario.
- **IP Address**: La dirección de su conexión.
- **Buffer de comandos**: Espacio donde se guardan los comandos del cliente.

### RESUMEN:
- El **servidor** organiza todo, conectando **clientes** y **canales**.
- **Clientes**: Son las personas conectadas, identificadas por su **fd** (un identificador único).
- **Canales**: Son grupos de chat. Cada canal tiene su propio **vector de usuarios** (normal y administrador).

---
  
### Comparación entre vector, list y deque en C++
- **`vector`**:
  - **Acceso rápido** a elementos (O(1)).
  - **Inserción/eliminación al final** eficiente.
  - **Memoria contigua** (mejor rendimiento en caché).
  - **Uso**: Ideal cuando necesitas acceso rápido y añadir elementos al final.

- **`list`**:
  - **Inserción/eliminación rápida** en cualquier lugar (O(1)).
  - **Acceso lento** a elementos (O(n)).
  - **Mayor uso de memoria** (por los punteros).
  - **Uso**: Ideal para operaciones frecuentes de inserción y eliminación en el medio.

- **`deque`**:
  - **Acceso rápido** a ambos extremos (O(1)).
  - **Inserción/eliminación rápida** en ambos extremos.
  - **Uso**: Ideal para cuando necesitas trabajar con ambos extremos de la colección.

**Conclusión:** El vector es más efectivo cuando necesitas acceso rápido a cualquier elemento por su contigüidad en memoria, lo que permite un acceso directo e inmediato (O(1)) mediante el índice. Además, su eficiencia de memoria y el rendimiento de caché lo hacen ideal cuando el acceso aleatorio es la operación más frecuente.

### **Uso de Punteros en Vectores vs. Almacenar Objetos Directamente**

Al almacenar objetos en un `vector`, cada vez que el contenedor necesita **redimensionarse** (por ejemplo, cuando se llena y se aumenta su capacidad), los elementos deben ser **copiados** a una nueva ubicación en memoria contigua. Este proceso puede ser costoso si los objetos son grandes o complejos.

En lugar de almacenar los objetos directamente en el `vector`, **trabajar con punteros** tiene varias ventajas:
1. **Reducción del costo de copia**: Al usar punteros, solo se copian las direcciones de memoria (punteros), en lugar de los propios objetos. Esto hace que la operación de redimensionamiento sea mucho más rápida y eficiente.
2. **Mayor control de la memoria**: Los objetos pueden mantenerse en su ubicación original y no necesitan ser movidos cada vez que el `vector` se redimensiona. Esto es particularmente útil cuando se manejan objetos grandes, como estructuras o clases que contienen muchos datos.
