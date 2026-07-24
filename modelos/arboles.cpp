#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

struct Nodo {
    string valor;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(string v) : valor(v), izquierda(nullptr), derecha(nullptr) {}
};

unordered_map<string, Nodo*> mapaNodos;
unordered_set<string> posiblesRaices;

void insertarRelacion(const string& padre, const string& hijo, const string& lado) {
    Nodo* nodoPadre = mapaNodos[padre];
    Nodo* nodoHijo = mapaNodos[hijo];

    if (lado == "I"||lado == "i")
        nodoPadre->izquierda = nodoHijo;
    else if (lado == "D"||lado=="d")
        nodoPadre->derecha = nodoHijo;

    posiblesRaices.erase(hijo); // ya no puede ser raíz
}

void preOrden(Nodo* nodo) {
    if (!nodo) return;
    cout << nodo->valor << " ";
    preOrden(nodo->izquierda);
    preOrden(nodo->derecha);
}

void inOrden(Nodo* nodo) {
    if (!nodo) return;
    inOrden(nodo->izquierda);
    cout << nodo->valor << " ";
    inOrden(nodo->derecha);
}

void postOrden(Nodo* nodo) {
    if (!nodo) return;
    postOrden(nodo->izquierda);
    postOrden(nodo->derecha);
    cout << nodo->valor << " ";
}

void recorridoPorNiveles(Nodo* raiz) {
    if (!raiz) return;
    queue<Nodo*> cola;
    cola.push(raiz);

    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();
        cout << actual->valor << " ";
        if (actual->izquierda) cola.push(actual->izquierda);
        if (actual->derecha) cola.push(actual->derecha);
    }
}

int main() {
    int n;
    cout << "Ingrese la cantidad de nodos: ";
    cin >> n;

    cout << "Ingrese los nombres de los " << n << " nodos:\n";
    for (int i = 0; i < n; ++i) {
        string nombre;
        cin >> nombre;
        mapaNodos[nombre] = new Nodo(nombre);
        posiblesRaices.insert(nombre); // todos podrían ser raíz al principio
    }

    int r;
    cout << "Ingrese la cantidad de relaciones (padre-hijo): ";
    cin >> r;

    cout << "Ingrese las relaciones en formato: PADRE HIJO LADO (I o D)\n";
    for (int i = 0; i < r; ++i) {
        string padre, hijo, lado;
        cin >> padre >> hijo >> lado;
        if (mapaNodos.count(padre) == 0 || mapaNodos.count(hijo) == 0) {
            cout << "Error: uno de los nodos no existe.\n";
            return 1;
        }
        insertarRelacion(padre, hijo, lado);
    }

    if (posiblesRaices.size() != 1) {
        cout << "Error: hay mas de una raiz posible o no se puede determinar.\n";
        return 1;
    }

    string raizNombre = *posiblesRaices.begin();
    Nodo* raiz = mapaNodos[raizNombre];

    cout << "\nNodo raiz: " << raizNombre << endl;

    cout << "Recorrido en Preorden: ";
    preOrden(raiz);
    cout << "\nRecorrido en Inorden: ";
    inOrden(raiz);
    cout << "\nRecorrido en Postorden: ";
    postOrden(raiz);
    cout << "\nRecorrido por Niveles: ";
    recorridoPorNiveles(raiz);
    cout << endl;

    return 0;
}