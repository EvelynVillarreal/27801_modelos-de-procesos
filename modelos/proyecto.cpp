#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>   // para std::min
#include <limits>      // para limpiar buffer
using namespace std;

// ================== CONSTANTES DEL JUEGO ==================
const int RONDAS = 6;  // Puedes ajustar la cantidad de rondas
const int PUNTOS_DISTANCIA = 5;
const int PENAL_ESPERA = 2;
const int TIEMPO_PRINCIPAL = 5;    // tiempo base por ruta principal
const int TIEMPO_ALTERNATIVA = 7;  // tiempo base por ruta alternativa
const int TIEMPO_ESPERA = 3;       // tiempo extra por esperar en rojo

// ================== ESTADOS DEL AFD ==================
enum Estado { EN_MOVIMIENTO, ESPERANDO, ALTERNATIVA };

// ================== ESTRUCTURA JUGADOR ==================
struct Jugador {
    string nombre;
    int puntos = 0;
    int tiempo = 0;
    Estado estado = EN_MOVIMIENTO;
    int contadorEsperar = 0;    // para que el bot aprenda del jugador
    int contadorAlternativa = 0;
    bool esBot = false;
    int posicionNodo = 0;       // Posición en el árbol (nodo indexado)
};

// ================== MODULO: INICIO DEL JUEGO ==================
void mostrarInicio() {
    cout << "=== Minijuego: Ruta de Entrega ===\n";
    cout << "Dos jugadores compiten por entregar el paquete primero.\n";
    cout << "Rutas:\n";
    cout << "- Principal: mas rapida, pero con semaforos.\n";
    cout << "- Alternativa: sin semaforos, pero mas larga.\n";
    cout << "Puntaje:\n";
    cout << "+5 por ruta principal corta.\n";
    cout << "-2 por esperar en rojo.\n";
    cout << "+1 por alternativa (sin esperar).\n";
    cout << "====================================\n";
}

// ================== MODULO: DECISION DEL JUGADOR HUMANO ==================
int decisionHumano(Jugador &j) {
    int opcion;
    while (true) {
        cout << "\n" << j.nombre << ", semaforo en ROJO. Elige:\n";
        cout << "1) Esperar\n";
        cout << "2) Tomar ruta alternativa\n";
        cout << "Opcion: ";
        if (!(cin >> opcion)) {  // entrada inválida
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida, intenta de nuevo.\n";
            continue;
        }
        if (opcion == 1 || opcion == 2) break;
        cout << "Opcion invalida, intenta de nuevo.\n";
    }
    if (opcion == 1) {
        j.estado = ESPERANDO;
        j.contadorEsperar++;
    } else {
        j.estado = ALTERNATIVA;
        j.contadorAlternativa++;
    }
    return opcion;
}

// ================== MODULO: DECISION DEL BOT ==================
int decisionBot(Jugador &bot, Jugador &humano) {
    int total = humano.contadorEsperar + humano.contadorAlternativa;
    double probEsperar = (total > 0) ? (double)humano.contadorEsperar / total : 0.5;

    int eleccion;
    if (probEsperar > 0.5) {
        eleccion = 2; // alternativa
    } else {
        eleccion = 1; // esperar
    }

    if (eleccion == 1) {
        bot.estado = ESPERANDO;
        bot.contadorEsperar++;
    } else {
        bot.estado = ALTERNATIVA;
        bot.contadorAlternativa++;
    }

    return eleccion;
}

// ================== MODULO: ACTUALIZAR PUNTOS Y POSICION ==================
void actualizarPuntos(Jugador &j, bool semaforoVerde) {
    int nuevaPos;
    if (semaforoVerde) {
        j.estado = EN_MOVIMIENTO;
        j.puntos += PUNTOS_DISTANCIA;
        j.tiempo += TIEMPO_PRINCIPAL;
        nuevaPos = j.posicionNodo * 2 + 1;
    } else {
        if (j.estado == ESPERANDO) {
            j.puntos -= PENAL_ESPERA;
            j.tiempo += TIEMPO_PRINCIPAL + TIEMPO_ESPERA;
            nuevaPos = j.posicionNodo * 2 + 1;
        } else if (j.estado == ALTERNATIVA) {
            j.puntos += 1;
            j.tiempo += TIEMPO_ALTERNATIVA;
            nuevaPos = j.posicionNodo * 2 + 2;
        } else {
            nuevaPos = j.posicionNodo; // por si acaso
        }
    }

    // Limitar nuevaPos al máximo nodo visible (6)
    if (nuevaPos > 6) nuevaPos = 6;

    j.posicionNodo = nuevaPos;
}

// ================== MODULO: MOSTRAR ARBOL ==================
void mostrarArbol(int posJ, int posB) {
    cout << "\nMapa del recorrido:\n\n";

    // Nivel 0 (raíz)
    cout << "            0";
    if (posJ == 0) cout << "(J)";
    if (posB == 0) cout << "(B)";
    cout << "\n";

    // Nivel 1
    cout << "          /     \\\n";
    cout << "        1";
    if (posJ == 1) cout << "(J)";
    if (posB == 1) cout << "(B)";
    cout << "         2";
    if (posJ == 2) cout << "(J)";
    if (posB == 2) cout << "(B)";
    cout << "\n";

    // Nivel 2
    cout << "      /   \\   /   \\\n";
    cout << "     3";
    if (posJ == 3) cout << "(J)";
    if (posB == 3) cout << "(B)";
    cout << "   4";
    if (posJ == 4) cout << "(J)";
    if (posB == 4) cout << "(B)";
    cout << "   5";
    if (posJ == 5) cout << "(J)";
    if (posB == 5) cout << "(B)";
    cout << "   6";
    if (posJ == 6) cout << "(J)";
    if (posB == 6) cout << "(B)";
    cout << "\n";
}

// ================== MODULO: MOSTRAR RESULTADOS ==================
void mostrarResultados(Jugador j1, Jugador j2) {
    cout << "\n=== RESULTADOS FINALES ===\n";
    cout << j1.nombre << " -> Puntos: " << j1.puntos << " | Tiempo: " << j1.tiempo << "s\n";
    cout << j2.nombre << " -> Puntos: " << j2.puntos << " | Tiempo: " << j2.tiempo << "s\n";

    if (j1.puntos > j2.puntos) {
        cout << "Ganador: " << j1.nombre << "\n";
    } else if (j2.puntos > j1.puntos) {
        cout << "Ganador: " << j2.nombre << "\n";
    } else {
        if (j1.tiempo < j2.tiempo) {
            cout << "Ganador por menor tiempo: " << j1.nombre << "\n";
        } else if (j2.tiempo < j1.tiempo) {
            cout << "Ganador por menor tiempo: " << j2.nombre << "\n";
        } else {
            cout << "Empate total.\n";
        }
    }
}

// ================== MAIN ==================
int main() {
    srand(time(0));

    Jugador jugador1, jugador2;
    jugador1.nombre = "Humano";
    jugador2.nombre = "Bot";
    jugador2.esBot = true;

    mostrarInicio();

    for (int ronda = 1; ronda <= RONDAS; ronda++) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // Reiniciar posición si están en nodo hoja
        if (jugador1.posicionNodo >= 3) {
            jugador1.posicionNodo = 0;
        }
        if (jugador2.posicionNodo >= 3) {
            jugador2.posicionNodo = 0;
        }

        cout << "--- Ronda " << ronda << " ---\n";

        mostrarArbol(jugador1.posicionNodo, jugador2.posicionNodo);

        bool semaforoVerde = (rand() % 2 == 0);

        if (semaforoVerde) {
            cout << "Semaforo en VERDE. Ambos avanzan por la ruta principal.\n";
        } else {
            cout << "Semaforo en ROJO.\n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar buffer

            decisionHumano(jugador1);
            decisionBot(jugador2, jugador1);
        }

        actualizarPuntos(jugador1, semaforoVerde);
        actualizarPuntos(jugador2, semaforoVerde);

        cout << "\nPuntos -> " << jugador1.nombre << ": " << jugador1.puntos
             << " | " << jugador2.nombre << ": " << jugador2.puntos << "\n";

        cout << "\nPresiona Enter para continuar...";
        cin.ignore();
        cin.get();
    }

    mostrarResultados(jugador1, jugador2);

    return 0;
}