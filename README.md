# Anima tu Estructura de Datos — Sqrt Decomposition

**Curso:** CS2023 - Algoritmos y Estructuras de Datos (UTEC, 2026-2)


**Estructura asignada:** Sqrt Decomposition


**Integrantes:** 
- Carlos Condor
- Maydelith Zuñiga
- Luis Sanchez

## Descripción

Este proyecto implementa la estructura de datos **Sqrt Decomposition** en C++ y genera un video
educativo animado (estilo 3Blue1Brown / Manim) que explica su funcionamiento, sus operaciones
principales y casos borde relevantes.

La estructura divide un arreglo de `N` elementos en bloques de tamaño `O(√N)`, precalculando la
suma de cada bloque. Esto permite responder consultas de suma en un rango y actualizaciones
(puntuales o por rango, con *lazy propagation*) en `O(√N)`, en vez de `O(N)` con fuerza bruta.

**Importante — la animación no está "hecha a mano":** cada video se genera a partir de un log de
eventos (`eventos.json` / `eventos_bordes.json`) producido por la propia implementación de C++
al ejecutarse. El script de Manim solo lee ese log y dibuja exactamente lo que el algoritmo hizo,
paso a paso. Si cambian los datos de entrada en el `.cpp`, el video cambia solo con volver a
compilar y renderizar — no hay pasos animados manualmente.

## Estructura del repositorio

```
.
├── README.md
├── requirements.txt
├── src/
│   ├── sqrt_decomp_instrumentado.cpp   # Implementación + logging de eventos (operaciones principales)
│   └── casos_borde.cpp                 # Implementación + logging de eventos (casos borde)
└── animacion/
    ├── animacion_sqrt_decomp.py        # Escena Manim: operaciones principales
    ├── animacion_casos_borde.py        # Escena Manim: casos borde
    ├── eventos.json                    # Generado al correr sqrt_decomp_instrumentado.cpp
    └── eventos_bordes.json             # Generado al correr casos_borde.cpp
```

## Qué cubre cada parte

**`sqrt_decomp_instrumentado.cpp` / `animacion_sqrt_decomp.py`** — operaciones principales:
- Construcción / inicialización (división en bloques + suma precalculada)
- `consultar(l, r)` — consulta de rango
- `actualizar(i, valor)` — actualización puntual
- `actualizarRango(l, r, aumento)` — actualización de rango con *lazy propagation*

**`casos_borde.cpp` / `animacion_casos_borde.py`** — casos borde:
- Estructura vacía (`N=0`): prueba que `max(1, sqrt(N))` evita división por cero y que
  `consultar` sobre un rango vacío no rompe nada.
- Un solo elemento (`N=1`): el bloque de tamaño 1 hace que hasta la consulta más chica entre
  por el camino de "bloque completo" en vez de "elemento suelto".

## Software requerido

- **Compilador C++** con soporte C++17 (g++ o clang++)
- **Python 3.10 – 3.12** (evitar 3.13+ por compatibilidad de wheels de Manim en Windows)
- **Manim Community** (`pip install manim`) — ver `requirements.txt`
- **ffmpeg** (Manim lo usa internamente para renderizar video; normalmente se instala solo como
  dependencia, pero si falla, instalarlo aparte)

## Instalación

```bash
# (opcional pero recomendado) crear un entorno virtual
python -m venv venv
source venv/Scripts/activate   # Git Bash / Linux / Mac
# venv\Scripts\activate.bat    # CMD en Windows

pip install -r requirements.txt
```

## Cómo compilar y correr la implementación en C++

```bash
cd src

# Operaciones principales
g++ -std=c++17 -O2 -o sqrt_demo sqrt_decomp_instrumentado.cpp
./sqrt_demo
# genera eventos.json en la carpeta actual

# Casos borde
g++ -std=c++17 -O2 -o casos_borde casos_borde.cpp
./casos_borde
# genera eventos_bordes.json en la carpeta actual
```

Copia (o mueve) los `eventos*.json` generados a la carpeta `animacion/` si corriste los binarios
desde otra ubicación — el script de Manim los busca ahí mismo.

## Cómo generar el video con Manim

```bash
cd animacion

# Operaciones principales (~1:10 min)
python -m manim -pql animacion_sqrt_decomp.py DemoSqrtDecomp

# Casos borde (~10 seg)
python -m manim -pql animacion_casos_borde.py CasosBorde
```

`-pql` = *preview, quality low* (rápido, para revisar). Para la entrega final, usar alta calidad:

```bash
python -m manim -qh animacion_sqrt_decomp.py DemoSqrtDecomp
python -m manim -qh animacion_casos_borde.py CasosBorde
```

Los `.mp4` quedan en `animacion/media/videos/.../1080p60/` (Manim crea esta estructura de
carpetas automáticamente).

## Unir ambos videos en uno solo

El enunciado pide **un único video** de 1 a 5 minutos. Para concatenar los dos renders con
`ffmpeg`:

```bash
# archivo lista.txt:
# file 'DemoSqrtDecomp.mp4'
# file 'CasosBorde.mp4'

ffmpeg -f concat -safe 0 -i lista.txt -c copy video_final.mp4
```

## Autores

_(completar con los integrantes del grupo)_

## Referencias

- Se usó como referencia de estilo el canal [3Blue1Brown](https://www.3blue1brown.com/).
- [Documentación de Manim Community](https://docs.manim.community/en/stable/)
