#include <bits/stdc++.h>
using namespace std;

// ============================================================
// Genera eventos_bordes.json con DOS casos borde reales:
//   1) Estructura vacía (N=0)
//   2) Un solo elemento (N=1)
// Cada evento lleva el campo "caso" para que la animación
// sepa a cuál de los dos pertenece.
// ============================================================

ofstream logFile;
bool primerEvento = true;

void logEvento(const string& json) {
    if (!primerEvento) logFile << ",\n";
    logFile << "  " << json;
    primerEvento = false;
}

class sqrtDecomposition {
private:
    vector<long long> valores;
    vector<long long> sumaBloques;
    vector<long long> aumentoPendiente;
    int tamanoBloque;
    string caso;

public:
    sqrtDecomposition(const vector<long long>& arregloInicial, const string& etiquetaCaso) {
        caso = etiquetaCaso;
        valores = arregloInicial;

        int cantidadElementos = valores.size();
        tamanoBloque = max(1, (int)sqrt(cantidadElementos));
        int numeroDeBloques = (cantidadElementos + tamanoBloque - 1) / tamanoBloque;

        sumaBloques.assign(numeroDeBloques, 0);
        aumentoPendiente.assign(numeroDeBloques, 0);

        logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"init_inicio\",\"n\":" + to_string(cantidadElementos) +
                   ",\"tamanoBloque\":" + to_string(tamanoBloque) +
                   ",\"numeroDeBloques\":" + to_string(numeroDeBloques) + "}");

        for (int indice = 0; indice < cantidadElementos; indice++) {
            int bloque = indice / tamanoBloque;
            sumaBloques[bloque] += valores[indice];

            logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"init_elemento\",\"indice\":" + to_string(indice) +
                       ",\"valor\":" + to_string(valores[indice]) +
                       ",\"bloque\":" + to_string(bloque) +
                       ",\"sumaBloqueParcial\":" + to_string(sumaBloques[bloque]) + "}");
        }

        logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"init_fin\",\"numeroDeBloques\":" + to_string(numeroDeBloques) + "}");
    }

    long long consultar(int izquierda, int derecha) {
        long long resultado = 0;

        logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"consulta_inicio\",\"izquierda\":" + to_string(izquierda) +
                   ",\"derecha\":" + to_string(derecha) + "}");

        while (izquierda <= derecha) {
            int bloque = izquierda / tamanoBloque;
            int inicioBloque = bloque * tamanoBloque;
            int finBloque = min((int)valores.size() - 1, inicioBloque + tamanoBloque - 1);

            bool iniciaBloque = izquierda == inicioBloque;
            bool bloqueCompleto = finBloque <= derecha;

            if (iniciaBloque && bloqueCompleto) {
                resultado += sumaBloques[bloque];
                logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"consulta_bloque\",\"bloque\":" + to_string(bloque) +
                           ",\"sumaBloque\":" + to_string(sumaBloques[bloque]) +
                           ",\"acumulado\":" + to_string(resultado) + "}");
                izquierda = finBloque + 1;
            } else {
                long long valorReal = valores[izquierda] + aumentoPendiente[bloque];
                resultado += valorReal;
                logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"consulta_suelto\",\"indice\":" + to_string(izquierda) +
                           ",\"valor\":" + to_string(valorReal) +
                           ",\"acumulado\":" + to_string(resultado) + "}");
                izquierda++;
            }
        }

        logEvento("{\"caso\":\"" + caso + "\",\"tipo\":\"consulta_fin\",\"resultado\":" + to_string(resultado) + "}");
        return resultado;
    }
};

int main() {
    logFile.open("eventos_bordes.json");
    logFile << "[\n";

    // ---------- CASO 1: ESTRUCTURA VACÍA ----------
    logEvento("{\"caso\":\"vacia\",\"tipo\":\"titulo\",\"texto\":\"Caso borde 1: estructura vacia (N=0)\"}");
    vector<long long> arregloVacio = {};
    sqrtDecomposition estructuraVacia(arregloVacio, "vacia");
    // Rango vacio: el while nunca se ejecuta, no revienta.
    long long r1 = estructuraVacia.consultar(0, -1);
    cout << "Consulta sobre estructura vacia = " << r1 << " (no crashea)\n";

    // ---------- CASO 2: UN SOLO ELEMENTO ----------
    logEvento("{\"caso\":\"un_elemento\",\"tipo\":\"titulo\",\"texto\":\"Caso borde 2: un solo elemento (N=1)\"}");
    vector<long long> arregloUno = {42};
    sqrtDecomposition estructuraUno(arregloUno, "un_elemento");
    long long r2 = estructuraUno.consultar(0, 0);
    cout << "Consulta sobre un solo elemento = " << r2 << "\n";

    logFile << "\n]\n";
    logFile.close();

    cout << "\nSe genero eventos_bordes.json\n";
    return 0;
}
