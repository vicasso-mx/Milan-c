# MILAN C

Aplicación de consola educativa implementada exclusivamente en C11. El catálogo se lee desde `data/PRODUCTOS.csv`; el carrito vive en memoria durante la sesión.

## Requisitos

CMake 3.16 o posterior y un compilador C11, como GCC, Clang o MSVC.

## Construcción y ejecución

Desde la raíz del repositorio:

```sh
cmake -S . -B build
cmake --build build
```

Ejecute `build/milan` en Linux/macOS o `build\Debug\milan.exe` en una configuración Visual Studio de Windows. La aplicación busca `data/PRODUCTOS.csv` relativa al directorio actual; puede proporcionar una ruta explícita como primer argumento. CMake copia la carpeta `data` al directorio de compilación para permitir `cd build` y ejecutar allí.

## Pruebas

```sh
ctest --test-dir build --output-on-failure
```

El estado inicial siempre es un carrito vacío; no se escribe información persistente.
