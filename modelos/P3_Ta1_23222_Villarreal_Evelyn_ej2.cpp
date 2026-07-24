#include <iostream>
#include <vector>
#include <stack>
#include <list>

using namespace std;

class Grafo {
    int V; // Número de vértices
    vector<list<int>> ady; // Lista de adyacencia

public:
    Grafo(int V) : V(V), ady(V) {}

    void agregarArista(int u, int v) {
        ady[u].push_back(v);
        ady[v].push_back(u); // No dirigido
    }

    // Verifica si todos los grados son pares
    bool todosGradosPares() {
        for (int i = 0; i < V; i++)
            if (ady[i].size() % 2 != 0)
                return false;
        return true;
    }

    // Encuentra un ciclo de Euler usando el algoritmo de Hierholzer
    void cicloEuler() {
        if (!todosGradosPares()) {
            cout << "No todos los vértices tienen grado par.\n";
            return;
        }

        stack<int> pila;
        vector<int> ciclo;
        int u = 0; // Empezamos desde el vértice 0
        pila.push(u);

        while (!pila.empty()) {
            int v = pila.top();

            if (!ady[v].empty()) {
                int w = ady[v].front();
                pila.push(w);

                // Remover arista v-w en ambas direcciones
                ady[v].remove(w);
                ady[w].remove(v);
            } else {
                ciclo.push_back(v);
                pila.pop();
            }
        }

        cout << "Ciclo de Euler: ";
        for (int vertice : ciclo)
            cout << vertice << " ";
        cout << endl;
    }
    
    void imprimirMatrizAdyacencia() {
    vector<vector<int>> matriz(V, vector<int>(V, 0));
    for (int u = 0; u < V; ++u)
        for (int v : ady[u])
            matriz[u][v] = 1;

    cout << "Matriz de adyacencia:\n";
    for (const auto& fila : matriz) {
        for (int val : fila)
            cout << val << " ";
        cout << endl;
    }
}
};

int main() {
    int V = 5; // Número de vértices
    Grafo g(V);

    // Aristas del grafo
    g.agregarArista(0, 1);
    g.agregarArista(1, 2);
    g.agregarArista(2, 0);
    g.agregarArista(0, 3);
    g.agregarArista(3, 4);
    g.agregarArista(4, 0);
    
	g.imprimirMatrizAdyacencia();
    g.cicloEuler();

    return 0;
}
