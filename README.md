# Proyecto Tetris C++ / SFML

## Librería Gráfica
Este proyecto utiliza **SFML 3 (Simple and Fast Multimedia Library)** para la interfaz gráfica, manejo de ventana y renderizado de texto/eventos.

## Requisitos de Compilación e Instalación
- **Lenguaje:** C++17 o superior.
- **Compilador:** GCC / MinGW (compatible con C++17).
- **IDE sugerido:** ZinjaI / VS Code.
- **Librería externa:** SFML 3.x (archivos de cabecera `#include ` y bibliotecas enlazadas `-lsfml-graphics -lsfml-window -lsfml-system`).

## Instrucciones para Compilar y Ejecutar

### Opción A: Desde ZinjaI
1. Abrir el proyecto ejecutando el archivo `.zpr`.
2. Asegurarse de que el entorno tenga configuradas las rutas de inclusión y librerías de SFML 3 en la configuración del proyecto.
3. Presionar `F9` (Compilar y Ejecutar) o usar el menú **Ejecutar -> Compilar y Ejecutar**.

### Opción B: Compilación Manual (Consola G++)
1. Abrir la terminal en la carpeta raíz del proyecto.
2. Asegurarse de que el archivo de fuente `ariblk.ttf` esté en la misma carpeta del ejecutable/proyecto.
3. Ejecutar el comando de compilación:
   ```bash
   g++ -std=c++17 src/*.cpp -I/ruta/sfml/include -L/ruta/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -o tetris
