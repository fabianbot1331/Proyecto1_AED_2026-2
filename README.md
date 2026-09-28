# Anima tu Estructura de Datos — Sqrt Decomposition

**Curso:** CS2023 - Algoritmos y Estructuras de Datos (UTEC, 2026-2)


**Estructura asignada:** Sqrt Decomposition


**Integrantes:** 
- Carlos Condor
- Maydelith Zuñiga
- Luis Sanchez

## Descripción

Este proyecto implementa la estructura de datos **Sqrt Decomposition** en C++ y genera un video
educativo animado con Manim que explica su funcionamiento, sus operaciones principales y casos
borde relevantes. El estilo visual se inspira en el recomendado por el enunciado del curso
(canal 3Blue1Brown), pero todo el contenido (código, eventos, animación) es original del grupo.

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
├── SqrtDescomposition_with_log/
│   ├── sqrt_decomposition.cpp           # Implementación base, sin logging (la estructura tal cual)
│   ├── sqrt_decomp_instrumentado.cpp    # Misma implementación + logging de eventos (operaciones principales)
│   └── casos_borde.cpp                  # Misma implementación + logging de eventos (casos borde)
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

## Cómo se implementó el código y la animación

**1. La estructura de datos en sí (`SqrtDecomposition`)** está implementada desde cero en C++,
sin usar ninguna librería estándar o de terceros para la lógica de bloques/sumas (solo STL
genérico como `vector`). Tiene tres atributos: el arreglo `valores`, el arreglo `sumaBloques`
(una suma precalculada por bloque) y `aumentoPendiente` (para lazy propagation en
`actualizarRango`). El tamaño de bloque se calcula como `max(1, (int)sqrt(N))`.

**2. Instrumentación con eventos reales.** Cada método (`consultar`, `actualizar`,
`actualizarRango`, y el propio constructor) llama a una función `logEvento(...)` en cada
sub-paso relevante de su lógica — por ejemplo, cada vez que suma un elemento suelto, cada vez
que usa la suma precalculada de un bloque completo, o cada vez que marca un bloque como
"pendiente". La lógica del algoritmo **no cambia en nada**; `logEvento` solo escribe una línea
JSON describiendo lo que acaba de pasar:

```cpp
void logEvento(const string& json) {
    if (!primerEvento) logFile << ",\n";
    logFile << "  " << json;
    primerEvento = false;
}
```

**3. Al correr el programa (`main()`)**, se ejecutan las operaciones con datos concretos
(inicialización, consultas, actualizaciones) y cada llamada a `logEvento` va acumulando un
arreglo JSON en `eventos.json` (o `eventos_bordes.json` para los casos borde). Este archivo es
la prueba de que la animación está impulsada por una ejecución real: contiene, en orden, cada
decisión que tomó el algoritmo con esos datos de entrada.

**4. La animación en Manim (`animacion_sqrt_decomp.py` / `animacion_casos_borde.py`)** abre ese
JSON y recorre los eventos uno por uno. Por cada tipo de evento dibuja la acción correspondiente
(ej. `consulta_suelto` pinta una celda de amarillo y actualiza el acumulado en pantalla;
`consulta_bloque` pinta el rectángulo del bloque de verde y usa directamente su suma
precalculada). El script **no contiene ningún valor inventado a mano**: todos los números,
índices y sumas que aparecen en el video vienen de los campos del JSON, que a su vez vienen de
la ejecución real del `.cpp`.

**Consecuencia práctica:** si cambian el arreglo de entrada o los parámetros de las consultas en
el `main()` del `.cpp`, no hay que tocar ni una línea del script de Manim — solo recompilar,
volver a correr el binario (que regenera el JSON) y volver a renderizar. El video se actualiza
solo porque describe una ejecución distinta.

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

**1) Implementación base de `sqrtDecomposition` (sin logging, el código de la estructura tal cual)**

Este es el archivo que muestra la estructura de datos en su forma más limpia, sin nada relacionado
a la animación — útil para revisar la lógica pura de bloques, sumas precalculadas y lazy propagation:

```bash
cd src
g++ -std=c++17 -O2 -o sqrt_decomposition sqrt_decomposition.cpp
./sqrt_decomposition
```

Salida esperada:

```
46
52
10
112
15
```

**2) Versión instrumentada (genera los eventos que usa la animación)**

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

## Referencias

- Se usó como referencia de estilo el canal [3Blue1Brown](https://www.3blue1brown.com/).
- [Documentación de Manim Community](https://docs.manim.community/en/stable/)
