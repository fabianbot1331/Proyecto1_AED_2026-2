#include <bits/stdc++.h>
using namespace std;

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

public:
    sqrtDecomposition(const vector<long long>& arregloInicial) {
        valores = arregloInicial;

        int cantidadElementos = valores.size();
        tamanoBloque = max(1, (int)sqrt(cantidadElementos));
        int numeroDeBloques = (cantidadElementos + tamanoBloque - 1) / tamanoBloque;

        sumaBloques.assign(numeroDeBloques, 0);
        aumentoPendiente.assign(numeroDeBloques, 0);

        logEvento("{\"tipo\":\"init_inicio\",\"n\":" + to_string(cantidadElementos) +
                   ",\"tamanoBloque\":" + to_string(tamanoBloque) +
                   ",\"numeroDeBloques\":" + to_string(numeroDeBloques) + "}");

        for (int indice = 0; indice < cantidadElementos; indice++) {
            int bloque = indice / tamanoBloque;
            sumaBloques[bloque] += valores[indice];

            logEvento("{\"tipo\":\"init_elemento\",\"indice\":" + to_string(indice) +
                       ",\"valor\":" + to_string(valores[indice]) +
                       ",\"bloque\":" + to_string(bloque) +
                       ",\"sumaBloqueParcial\":" + to_string(sumaBloques[bloque]) + "}");
        }

        string sumasStr = "[";
        for (size_t i = 0; i < sumaBloques.size(); i++) {
            sumasStr += to_string(sumaBloques[i]);
            if (i + 1 < sumaBloques.size()) sumasStr += ",";
        }
        sumasStr += "]";
        logEvento("{\"tipo\":\"init_fin\",\"sumaBloques\":" + sumasStr + "}");
    }

    long long consultar(int izquierda, int derecha) {
        long long resultado = 0;

        logEvento("{\"tipo\":\"consulta_inicio\",\"izquierda\":" + to_string(izquierda) +
                   ",\"derecha\":" + to_string(derecha) + "}");

        while (izquierda <= derecha) {
            int bloque = izquierda / tamanoBloque;
            int inicioBloque = bloque * tamanoBloque;
            int finBloque = min((int)valores.size() - 1, inicioBloque + tamanoBloque - 1);

            bool iniciaBloque = izquierda == inicioBloque;
            bool bloqueCompleto = finBloque <= derecha;

            if (iniciaBloque && bloqueCompleto) {
                resultado += sumaBloques[bloque];

                logEvento("{\"tipo\":\"consulta_bloque\",\"bloque\":" + to_string(bloque) +
                           ",\"sumaBloque\":" + to_string(sumaBloques[bloque]) +
                           ",\"acumulado\":" + to_string(resultado) + "}");

                izquierda = finBloque + 1;
            } else {
                long long valorReal = valores[izquierda] + aumentoPendiente[bloque];
                resultado += valorReal;

                logEvento("{\"tipo\":\"consulta_suelto\",\"indice\":" + to_string(izquierda) +
                           ",\"valor\":" + to_string(valorReal) +
                           ",\"acumulado\":" + to_string(resultado) + "}");

                izquierda++;
            }
        }

        logEvento("{\"tipo\":\"consulta_fin\",\"resultado\":" + to_string(resultado) + "}");

        return resultado;
    }

    void actualizar(int indice, long long nuevoValor) {
        int bloque = indice / tamanoBloque;
        long long valorActual = valores[indice] + aumentoPendiente[bloque];

        logEvento("{\"tipo\":\"actualizar_inicio\",\"indice\":" + to_string(indice) +
                   ",\"valorViejo\":" + to_string(valorActual) +
                   ",\"valorNuevo\":" + to_string(nuevoValor) +
                   ",\"bloque\":" + to_string(bloque) + "}");

        sumaBloques[bloque] -= valorActual;
        sumaBloques[bloque] += nuevoValor;
        valores[indice] = nuevoValor - aumentoPendiente[bloque];

        logEvento("{\"tipo\":\"actualizar_fin\",\"bloque\":" + to_string(bloque) +
                   ",\"nuevaSumaBloque\":" + to_string(sumaBloques[bloque]) + "}");
    }

    void actualizarRango(int izquierda, int derecha, long long aumento) {
        logEvento("{\"tipo\":\"actualizarRango_inicio\",\"izquierda\":" + to_string(izquierda) +
                   ",\"derecha\":" + to_string(derecha) +
                   ",\"aumento\":" + to_string(aumento) + "}");

        while (izquierda <= derecha) {
            int bloque = izquierda / tamanoBloque;
            int inicioBloque = bloque * tamanoBloque;
            int finBloque = min((int)valores.size() - 1, inicioBloque + tamanoBloque - 1);

            bool iniciaBloque = izquierda == inicioBloque;
            bool bloqueCompleto = finBloque <= derecha;

            if (iniciaBloque && bloqueCompleto) {
                int cantidadElementos = finBloque - inicioBloque + 1;

                aumentoPendiente[bloque] += aumento;
                sumaBloques[bloque] += aumento * cantidadElementos;

                logEvento("{\"tipo\":\"actualizarRango_bloque\",\"bloque\":" + to_string(bloque) +
                           ",\"nuevoAumentoPendiente\":" + to_string(aumentoPendiente[bloque]) +
                           ",\"nuevaSumaBloque\":" + to_string(sumaBloques[bloque]) + "}");

                izquierda = finBloque + 1;
            } else {
                valores[izquierda] += aumento;
                sumaBloques[bloque] += aumento;

                logEvento("{\"tipo\":\"actualizarRango_suelto\",\"indice\":" + to_string(izquierda) +
                           ",\"nuevoValorBase\":" + to_string(valores[izquierda]) +
                           ",\"nuevaSumaBloque\":" + to_string(sumaBloques[bloque]) + "}");

                izquierda++;
            }
        }

        logEvento("{\"tipo\":\"actualizarRango_fin\"}");
    }
};

int main() {
    logFile.open("eventos.json");
    logFile << "[\n";

    vector<long long> arreglo = {
        2, 5, 3, 7,
        1, 4, 6, 8,
        2, 3, 1, 5,
        4, 2, 7, 1
    };

    sqrtDecomposition estructura(arreglo);

    // Consulta normal: suma desde la posicion 2 hasta la 13.
    cout << "Consulta(2,13) = " << estructura.consultar(2, 13) << '\n';

    // Cambia el valor de la posicion 5 por 10.
    estructura.actualizar(5, 10);
    cout << "Consulta(2,13) tras actualizar = " << estructura.consultar(2, 13) << '\n';

    // Caso borde: consulta de un solo elemento.
    cout << "Consulta(5,5) [caso borde] = " << estructura.consultar(5, 5) << '\n';

    // Suma 5 a todos los elementos desde la posicion 2 hasta la 13.
    estructura.actualizarRango(2, 13, 5);
    cout << "Consulta(2,13) tras actualizarRango = " << estructura.consultar(2, 13) << '\n';

    // El elemento de la posicion 5 ahora vale 15.
    cout << "Consulta(5,5) tras actualizarRango = " << estructura.consultar(5, 5) << '\n';

    logFile << "\n]\n";
    logFile.close();

    cout << "\nSe genero eventos.json con el registro real de cada paso.\n";
    return 0;
}
