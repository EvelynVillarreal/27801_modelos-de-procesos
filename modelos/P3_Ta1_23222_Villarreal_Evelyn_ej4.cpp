#include <iostream>
#include <vector>

using namespace std;

class Grafo {
    int V;
    vector<vector<int>> ady;

public:
    Grafo(int V) : V(V), ady(V) {}

    void agregarArista(int u, int v) {
        ady[u].push_back(v);
        ady[v].push_back(u); // Grafo no dirigido
    }

    void dfs(int origen, int destino, vector<bool>& visitado, vector<int>& camino) {
        visitado[origen] = true;
        camino.push_back(origen);

        if (origen == destino) {
            // Mostrar la trayectoria encontrada
            for (int v : camino)
                cout << v << " ";
            cout << endl;
        } else {
            for (int vecino : ady[origen]) {
                if (!visitado[vecino]) {
                    dfs(vecino, destino, visitado, camino);
                }
            }
        }

        // Backtracking
        visitado[origen] = false;
        camino.pop_back();
    }

    void listarTrayectoriasSimples(int inicio, int fin) {
        vector<bool> visitado(V, false);
        vector<int> camino;

        cout << "Trayectorias simples de " << inicio << " a " << fin << ":\n";
        dfs(inicio, fin, visitado, camino);
    }
};

int main() {
    int V = 5;
    Grafo g(V);

    // Definir aristas del grafo
    g.agregarArista(0, 1);
    g.agregarArista(0, 2);
    g.agregarArista(1, 3);
    g.agregarArista(2, 3);
    g.agregarArista(3, 4);

    int origen = 0, destino = 4;
    g.listarTrayectoriasSimples(origen, destino);

    return 0;
}
