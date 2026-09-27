"""
Animación de Sqrt Decomposition impulsada por datos REALES.

Cómo funciona:
1. Corres tu C++ instrumentado -> genera eventos.json con cada sub-paso real.
2. Este script LEE eventos.json y dibuja exactamente esa secuencia.
   No hay ningún valor "inventado a mano" aquí: todo viene del JSON.

Para renderizar:
    manim -pql animacion_sqrt_decomp.py DemoSqrtDecomp
(usa -qh para alta calidad en la entrega final)
"""

import json
from manim import *

with open("eventos.json") as f:
    EVENTOS = json.load(f)

# Colores para los distintos estados de una celda
COLOR_NORMAL = BLUE_E
COLOR_SUELTO = YELLOW
COLOR_BLOQUE = GREEN
COLOR_UPDATE = RED


class DemoSqrtDecomp(Scene):
    def construct(self):
        titulo = Text("Sqrt Decomposition", font_size=40)
        subtitulo = Text("Animación impulsada por ejecución real", font_size=20, color=GRAY)
        subtitulo.next_to(titulo, DOWN)
        self.play(Write(titulo), FadeIn(subtitulo))
        self.wait(1)
        self.play(FadeOut(titulo), FadeOut(subtitulo))

        # Estructuras que vamos a ir llenando a medida que procesamos eventos
        celdas = {}          # indice -> Square (representa valores[indice])
        etiquetas = {}       # indice -> Text (representa el valor mostrado)
        bloques_rect = {}    # bloque -> SurroundingRectangle
        bloques_texto = {}   # bloque -> Text (suma del bloque)
        tamano_bloque = None
        acumulado_texto = None

        for evento in EVENTOS:
            tipo = evento["tipo"]

            # ---------- INICIALIZACIÓN ----------
            if tipo == "init_inicio":
                tamano_bloque = evento["tamanoBloque"]
                n = evento["n"]
                info = Text(
                    f"N={n}, tamaño de bloque=√N≈{tamano_bloque}, "
                    f"{evento['numeroDeBloques']} bloques",
                    font_size=24,
                )
                info.to_edge(UP)
                self.play(Write(info))
                self.info_text = info

            elif tipo == "init_elemento":
                idx = evento["indice"]
                val = evento["valor"]
                bloque = evento["bloque"]

                cuadro = Square(side_length=0.7, color=COLOR_NORMAL)
                cuadro.move_to(RIGHT * (idx - 8) * 0.75 + UP * 0.5)
                num = Text(str(val), font_size=20).move_to(cuadro.get_center())

                celdas[idx] = cuadro
                etiquetas[idx] = num
                self.play(FadeIn(cuadro), Write(num), run_time=0.15)

                # Si es el primer elemento de un bloque nuevo, dibuja el rectángulo del bloque
                if bloque not in bloques_rect:
                    inicio_bloque = bloque * tamano_bloque
                    fin_bloque = inicio_bloque + tamano_bloque - 1
                    celdas_bloque = [celdas[i] for i in range(inicio_bloque, min(fin_bloque, idx) + 1) if i in celdas]
                    rect = SurroundingRectangle(VGroup(*celdas_bloque), color=GRAY, buff=0.1)
                    bloques_rect[bloque] = rect
                    self.play(Create(rect), run_time=0.2)

            elif tipo == "init_fin":
                sumas = evento["sumaBloques"]
                for b, suma in enumerate(sumas):
                    txt = Text(f"Σ={suma}", font_size=18, color=GREEN)
                    txt.next_to(bloques_rect[b], DOWN)
                    bloques_texto[b] = txt
                    self.play(Write(txt), run_time=0.2)
                self.wait(0.5)

            # ---------- CONSULTA ----------
            elif tipo == "consulta_inicio":
                l, r = evento["izquierda"], evento["derecha"]
                texto = Text(f"consultar({l}, {r})", font_size=26, color=YELLOW)
                texto.to_edge(DOWN)
                acumulado_texto = Text("acumulado = 0", font_size=22).next_to(texto, UP)
                self.play(Write(texto), Write(acumulado_texto))
                self.consulta_texto = texto

            elif tipo == "consulta_suelto":
                idx = evento["indice"]
                valor_real = evento["valor"]
                self.play(celdas[idx].animate.set_color(COLOR_SUELTO), run_time=0.3)

                # Si el valor real (base + pendiente) no coincide con lo que muestra
                # la celda, es porque este elemento vive en un bloque que recibió un
                # aumento "perezoso" y nunca se repintó individualmente. Lo corregimos
                # AQUÍ, justo cuando el algoritmo real lo usa por primera vez.
                if etiquetas[idx].text != str(valor_real):
                    nota = Text("(incluye pendiente)", font_size=14, color=PURPLE)
                    nota.next_to(celdas[idx], UP, buff=0.05)
                    nuevo_num = Text(str(valor_real), font_size=20).move_to(celdas[idx])
                    self.play(Transform(etiquetas[idx], nuevo_num), FadeIn(nota), run_time=0.35)
                    self.play(FadeOut(nota), run_time=0.2)

                nuevo = Text(f"acumulado = {evento['acumulado']}", font_size=22).move_to(acumulado_texto)
                self.play(Transform(acumulado_texto, nuevo), run_time=0.3)
                self.play(celdas[idx].animate.set_color(COLOR_NORMAL), run_time=0.2)

            elif tipo == "consulta_bloque":
                b = evento["bloque"]
                self.play(bloques_rect[b].animate.set_color(COLOR_BLOQUE), run_time=0.4)
                nuevo = Text(f"acumulado = {evento['acumulado']}", font_size=22).move_to(acumulado_texto)
                self.play(Transform(acumulado_texto, nuevo), run_time=0.3)
                self.play(bloques_rect[b].animate.set_color(GRAY), run_time=0.2)

            elif tipo == "consulta_fin":
                resultado = Text(f"resultado = {evento['resultado']}", font_size=28, color=GREEN)
                resultado.next_to(self.consulta_texto, UP, buff=1)
                self.play(Write(resultado))
                self.wait(1)
                self.play(FadeOut(self.consulta_texto), FadeOut(acumulado_texto), FadeOut(resultado))

            # ---------- ACTUALIZACIÓN ----------
            elif tipo == "actualizar_inicio":
                idx = evento["indice"]
                texto = Text(
                    f"actualizar({idx}, {evento['valorNuevo']})", font_size=26, color=RED
                ).to_edge(DOWN)
                self.play(Write(texto))
                self.play(celdas[idx].animate.set_color(COLOR_UPDATE), run_time=0.3)
                nuevo_num = Text(str(evento["valorNuevo"]), font_size=20).move_to(etiquetas[idx])
                self.play(Transform(etiquetas[idx], nuevo_num))
                self.actualizar_texto = texto

            elif tipo == "actualizar_fin":
                b = evento["bloque"]
                nuevo_txt = Text(f"Σ={evento['nuevaSumaBloque']}", font_size=18, color=GREEN)
                nuevo_txt.move_to(bloques_texto[b])
                self.play(Transform(bloques_texto[b], nuevo_txt))
                idx = None
                # regresa la celda a su color normal
                self.wait(0.3)
                self.play(FadeOut(self.actualizar_texto))

            # ---------- ACTUALIZACIÓN DE RANGO (lazy propagation) ----------
            elif tipo == "actualizarRango_inicio":
                l, r, aumento = evento["izquierda"], evento["derecha"], evento["aumento"]
                texto = Text(
                    f"actualizarRango({l}, {r}, +{aumento})", font_size=26, color=PURPLE
                ).to_edge(DOWN)
                self.play(Write(texto))
                self.rango_texto = texto

            elif tipo == "actualizarRango_bloque":
                b = evento["bloque"]
                # Bloque completo: se marca "perezoso" (lazy) sin tocar cada celda una por una
                etiqueta_lazy = Text(
                    f"+{evento['nuevoAumentoPendiente']} pendiente", font_size=16, color=PURPLE
                ).next_to(bloques_rect[b], UP, buff=0.1)
                self.play(bloques_rect[b].animate.set_color(PURPLE), Write(etiqueta_lazy), run_time=0.4)
                nuevo_txt = Text(f"Σ={evento['nuevaSumaBloque']}", font_size=18, color=GREEN)
                nuevo_txt.move_to(bloques_texto[b])
                self.play(Transform(bloques_texto[b], nuevo_txt))
                self.play(bloques_rect[b].animate.set_color(GRAY), FadeOut(etiqueta_lazy), run_time=0.3)

            elif tipo == "actualizarRango_suelto":
                idx = evento["indice"]
                bloque_idx = idx // tamano_bloque
                self.play(celdas[idx].animate.set_color(COLOR_UPDATE), run_time=0.25)
                nuevo_num = Text(str(evento["nuevoValorBase"]), font_size=20).move_to(etiquetas[idx])
                self.play(Transform(etiquetas[idx], nuevo_num), run_time=0.25)
                nuevo_txt = Text(f"Σ={evento['nuevaSumaBloque']}", font_size=18, color=GREEN)
                nuevo_txt.move_to(bloques_texto[bloque_idx])
                self.play(Transform(bloques_texto[bloque_idx], nuevo_txt), run_time=0.2)
                self.play(celdas[idx].animate.set_color(COLOR_NORMAL), run_time=0.2)

            elif tipo == "actualizarRango_fin":
                self.wait(0.5)
                self.play(FadeOut(self.rango_texto))

        self.wait(2)
