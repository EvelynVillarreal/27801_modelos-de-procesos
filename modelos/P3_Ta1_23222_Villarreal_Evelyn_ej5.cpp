#include <iostream>
#include <vector>

using namespace std;

bool esIsomorfismo(const vector<vector<int>>& G1, const vector<vector<int>>& G2, const vector<int>& f) {
    int n = G1.size();
    vector<bool> usado(n, false);
    for (int v : f) {
        if (v < 0 || v >= n || usado[v]) return false;
        usado[v] = true;
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (G1[i][j] != G2[f[i]][f[j]])
                return false;

    return true;
}

void imprimirMatriz(const vector<vector<int>>& g, const string& nombre) {
    cout << nombre << ":\n";
    for (const auto& fila : g) {
        for (int v : fila) cout << v << " ";
        cout << endl;
    }
}

int main() {
    vector<vector<int>> G1 = {
        {0, 1, 1, 0},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 1, 0}
    };

	vector<vector<int>> G2 = {
        {0, 1, 0, 1},
        {1, 0, 0, 1},
        {0, 0, 0, 1},
        {1, 1, 1, 0}
    };
    vector<int> f = {0, 1, 3, 2}; // isomorfismo propuesto

    imprimirMatriz(G1, "Grafo G1");
    imprimirMatriz(G2, "Grafo G2");

    cout << "\nMapeo propuesto:\n";
    for (int i = 0; i < f.size(); i++)
        cout << "G1[" << i << "] -> G2[" << f[i] << "]\n";

    if (esIsomorfismo(G1, G2, f)) {
        cout << "\n El mapeo ES un isomorfismo.\n";
    } else {
        cout << "\n El mapeo NO es un isomorfismo.\n";
    }

    return 0;
}
