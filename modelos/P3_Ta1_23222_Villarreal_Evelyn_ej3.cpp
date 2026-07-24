#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Grafo {
    int V;
    vector<vector<int>> ady;

public:
    Grafo(int V) : V(V), ady(V) {}

    void agregarArista(int u, int v) {
        ady[u].push_back(v);
        ady[v].push_back(u); // grafo no dirigido
    }

    bool esBipartita() {
        vector<int> color(V, -1); // -1: no coloreado

        for (int inicio = 0; inicio < V; inicio++) {
            if (color[inicio] == -1) {
                queue<int> q;
                q.push(inicio);
                color[inicio] = 0;

                while (!q.empty()) {
                    int u = q.front();
                    q.pop();

                    for (int v : ady[u]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q.push(v);
                        } else if (color[v] == color[u]) {
                            return false; // conflicto de color
                        }
                    }
                }
            }
        }

        // Si llegamos aquí, es bipartita. Mostramos los conjuntos.
        cout << "La grafica es bipartita.\n";
        cout << "Conjunto 1: ";
        for (int i = 0; i < V; i++)
            if (color[i] == 0)
                cout << i << " ";
        cout << "\nConjunto 2: ";
        for (int i = 0; i < V; i++)
            if (color[i] == 1)
                cout << i << " ";
        cout << endl;

        return true;
    }
};

int main() {
    int V = 6; // Número de vértices
    Grafo g(V);

    // Ejemplo: Grafo bipartito
    g.agregarArista(0, 1);
    g.agregarArista(0, 3);
    g.agregarArista(1, 2);
    g.agregarArista(2, 3);
    g.agregarArista(4, 5);

    if (!g.esBipartita())
        cout << "La grafica NO es bipartita.\n";

    return 0;
}
