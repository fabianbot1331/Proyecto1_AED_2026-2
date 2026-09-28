import json
from manim import *

with open("eventos_bordes.json") as f:
    EVENTOS = json.load(f)


class CasosBorde(Scene):
    def construct(self):
        celdas = {}
        etiquetas = {}
        bloques_rect = {}

        for evento in EVENTOS:
            tipo = evento["tipo"]

            if tipo == "titulo":
                texto = Text(evento["texto"], font_size=28)
                self.play(Write(texto), run_time=0.6)
                self.wait(0.4)
                self.play(FadeOut(texto), run_time=0.3)
                celdas.clear()
                etiquetas.clear()
                bloques_rect.clear()

            elif tipo == "init_inicio":
                n = evento["n"]
                info = Text(
                    f"N={n}, tamañoBloque={evento['tamanoBloque']}, "
                    f"bloques={evento['numeroDeBloques']}",
                    font_size=22,
                ).to_edge(UP)
                self.play(Write(info), run_time=0.5)
                self.info_actual = info

                if n == 0:
                    vacio_txt = Text("(sin elementos)", font_size=24, color=GRAY)
                    self.play(FadeIn(vacio_txt), run_time=0.4)
                    self.vacio_txt = vacio_txt

            elif tipo == "init_elemento":
                idx = evento["indice"]
                val = evento["valor"]
                cuadro = Square(side_length=1.0, color=BLUE_E)
                num = Text(str(val), font_size=24).move_to(cuadro.get_center())
                celdas[idx] = cuadro
                etiquetas[idx] = num
                self.play(FadeIn(cuadro), Write(num), run_time=0.4)

                rect = SurroundingRectangle(cuadro, color=GRAY, buff=0.15)
                bloques_rect[0] = rect
                self.play(Create(rect), run_time=0.3)

            elif tipo == "consulta_inicio":
                l, r = evento["izquierda"], evento["derecha"]
                texto = Text(f"consultar({l}, {r})", font_size=24, color=YELLOW).to_edge(DOWN)
                self.play(Write(texto), run_time=0.4)
                self.consulta_texto = texto

            elif tipo == "consulta_bloque":
                b = evento["bloque"]
                self.play(bloques_rect[b].animate.set_color(GREEN), run_time=0.4)
                nota = Text(
                    f"bloque completo -> O(1), no 'suelto'",
                    font_size=18, color=GREEN
                ).next_to(bloques_rect[b], UP)
                self.play(Write(nota), run_time=0.4)
                self.wait(0.3)
                self.play(FadeOut(nota), run_time=0.2)

            elif tipo == "consulta_fin":
                resultado = Text(f"resultado = {evento['resultado']}", font_size=26, color=GREEN)
                resultado.next_to(self.consulta_texto, UP, buff=0.8)
                nota_extra = None
                if evento["resultado"] == 0 and not celdas:
                    nota_extra = Text(
                        "el while nunca entro: no crashea", font_size=18, color=GRAY
                    ).next_to(resultado, UP)
                self.play(Write(resultado), run_time=0.5)
                if nota_extra:
                    self.play(Write(nota_extra), run_time=0.5)
                self.wait(0.6)
                fadeouts = [FadeOut(self.consulta_texto), FadeOut(resultado)]
                if nota_extra:
                    fadeouts.append(FadeOut(nota_extra))
                if hasattr(self, "vacio_txt"):
                    fadeouts.append(FadeOut(self.vacio_txt))
                    del self.vacio_txt
                fadeouts.append(FadeOut(self.info_actual))
                for c in celdas.values():
                    fadeouts.append(FadeOut(c))
                for e in etiquetas.values():
                    fadeouts.append(FadeOut(e))
                for r in bloques_rect.values():
                    fadeouts.append(FadeOut(r))
                self.play(*fadeouts, run_time=0.4)
