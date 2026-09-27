#include <bits/stdc++.h>
using namespace std;

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

        for (int indice = 0; indice < cantidadElementos; indice++) {
            int bloque = indice / tamanoBloque;
            sumaBloques[bloque] += valores[indice];
        }
    }

    long long consultar(int izquierda, int derecha) {
        long long resultado = 0;

        while (izquierda <= derecha) {
            int bloque = izquierda / tamanoBloque;
            int inicioBloque = bloque * tamanoBloque;
            int finBloque = min( (int)valores.size() - 1, inicioBloque + tamanoBloque - 1);

            bool iniciaBloque = izquierda == inicioBloque;
            bool bloqueCompleto = finBloque <= derecha;

            if (iniciaBloque && bloqueCompleto) {
                resultado += sumaBloques[bloque];
                izquierda = finBloque + 1;
            }
            else {
                resultado += valores[izquierda] + aumentoPendiente[bloque];
                izquierda++;
            }
        }

        return resultado;
    }

    void actualizar(int indice, long long nuevoValor) {
        int bloque = indice / tamanoBloque;

        long long valorActual = valores[indice] + aumentoPendiente[bloque];

        sumaBloques[bloque] -= valorActual;
        sumaBloques[bloque] += nuevoValor;

        valores[indice] = nuevoValor - aumentoPendiente[bloque];
    }

    void actualizarRango( int izquierda, int derecha, long long aumento) {
        while (izquierda <= derecha) {
            int bloque = izquierda / tamanoBloque;
            int inicioBloque = bloque * tamanoBloque;
            int finBloque = min( (int)valores.size() - 1, inicioBloque + tamanoBloque - 1);

            bool iniciaBloque = izquierda == inicioBloque;
            bool bloqueCompleto = finBloque <= derecha;

            if (iniciaBloque && bloqueCompleto) {
                int cantidadElementos = finBloque - inicioBloque + 1;

                aumentoPendiente[bloque] += aumento;
                sumaBloques[bloque] += aumento * cantidadElementos;

                izquierda = finBloque + 1;
            }
            else {
                valores[izquierda] += aumento;
                sumaBloques[bloque] += aumento;
                izquierda++;
            }
        }
    }
};

int main() {
    vector<long long> arreglo = {
        2, 5, 3, 7,
        1, 4, 6, 8,
        2, 3, 1, 5,
        4, 2, 7, 1
    };

    sqrtDecomposition estructura(arreglo);

    // Consulta normal: suma desde la posicion 2 hasta la 13.
    cout << estructura.consultar(2, 13) << '\n';

    // Cambia el valor de la posicion 5 por 10.
    estructura.actualizar(5, 10);
    cout << estructura.consultar(2, 13) << '\n';

    // Caso borde: consulta de un solo elemento.
    cout << estructura.consultar(5, 5) << '\n';

    // Suma 5 a todos los elementos desde la posicion 2 hasta la 13.
    estructura.actualizarRango(2, 13, 5);
    cout << estructura.consultar(2, 13) << '\n';

    // El elemento de la posicion 5 ahora vale 15.
    cout << estructura.consultar(5, 5) << '\n';
}
