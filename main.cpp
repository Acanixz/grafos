// Trabalho M1.1
// Grafos 2025 - 1
// Alunos: 
//      Hérick Vitor Vieira Bittencourt
//      Rodrigo Rey de Oliveira Carrard
//      Paola Piran Zanella

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Grafos {
public:
    bool direcionado;
    bool ponderado;

    // Construtor
    Grafos(bool Direcionado, bool Ponderado) {
        direcionado = Direcionado;
        ponderado = Ponderado;
    }

    // Métodos virtuais
    virtual bool inserirVertice(string label) = 0;
    virtual bool removerVertice(int indice) = 0;
    virtual string labelVertice(int indice) = 0;
    virtual void imprimeGrafo() = 0;
    virtual bool inserirAresta(int origem, int destino, int peso = 1) = 0;
    virtual bool removerAresta(int origem, int destino) = 0;
    virtual bool existeAresta(int origem, int destino) = 0;
    virtual float pesoAresta(int origem, int destino) = 0;
    virtual vector<int> retornarVizinhos(int vertice) = 0;
};

// Estrutura para representar uma aresta na lista de adjacência
struct Aresta {
    int destino;
    int peso;
    Aresta(int d, int p) : destino(d), peso(p) {}
};

// Implementação usando matriz de adjacência
class GrafoMatriz : public Grafos {
private:
    vector<vector<int>> matriz; // Matriz de adjacência
    vector<string> vertices;    // Rótulos dos vértices

public:
    // Construtor
    GrafoMatriz(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo
    bool inserirVertice(string label) override {
        vertices.push_back(label);
        for (auto& linha : matriz) {
            linha.push_back(0); // Inicializa com 0 (sem aresta)
        }
        matriz.push_back(vector<int>(vertices.size(), 0)); // Nova linha para o novo vértice
        return true;
    }

    // Remove um vértice do grafo
    bool removerVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return false;

        vertices.erase(vertices.begin() + indice);
        matriz.erase(matriz.begin() + indice);

        for (auto& linha : matriz) {
            linha.erase(linha.begin() + indice);
        }

        return true;
    }

    // Retorna o rótulo de um vértice
    string labelVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return "";
        return vertices[indice];
    }

    // Imprime o grafo
    void imprimeGrafo() override {
        cout << "Matriz de Adjacência:" << endl;
        for (size_t i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << ": ";
            for (size_t j = 0; j < vertices.size(); j++) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Insere uma aresta entre dois vértices
    bool inserirAresta(int origem, int destino, int peso = 1) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        matriz[origem][destino] = peso;
        if (!direcionado) {
            matriz[destino][origem] = peso; // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices
    bool removerAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        matriz[origem][destino] = 0;
        if (!direcionado) {
            matriz[destino][origem] = 0; // Remove a aresta bidirecional
        }
        return true;
    }

    // Verifica se existe uma aresta entre dois vértices
    bool existeAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;
        return matriz[origem][destino] != 0;
    }

    // Retorna o peso de uma aresta
    float pesoAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return -1;
        return matriz[origem][destino];
    }

    // Retorna os vizinhos de um vértice
    vector<int> retornarVizinhos(int vertice) override {
        vector<int> vizinhos;
        if (vertice < 0 || vertice >= vertices.size()) return vizinhos;

        for (size_t i = 0; i < vertices.size(); i++) {
            if (matriz[vertice][i] != 0) {
                vizinhos.push_back(i);
            }
        }
        return vizinhos;
    }
};

// Implementação usando lista de adjacência
class GrafoLista : public Grafos {
private:
    vector<vector<Aresta>> lista; // Lista de adjacência
    vector<string> vertices;      // Rótulos dos vértices

public:
    // Construtor
    GrafoLista(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo
    bool inserirVertice(string label) override {
        vertices.push_back(label);
        lista.push_back(vector<Aresta>()); // Adiciona uma lista vazia para o novo vértice
        return true;
    }

    // Remove um vértice do grafo
    bool removerVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return false;

        vertices.erase(vertices.begin() + indice);
        lista.erase(lista.begin() + indice);

        for (auto& vizinhos : lista) {
            vizinhos.erase(
                remove_if(vizinhos.begin(), vizinhos.end(),
                          [indice](const Aresta& a) { return a.destino == indice; }),
                vizinhos.end());
        }

        for (auto& vizinhos : lista) {
            for (auto& aresta : vizinhos) {
                if (aresta.destino > indice) {
                    aresta.destino--;
                }
            }
        }

        return true;
    }

    // Retorna o rótulo de um vértice
    string labelVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return "";
        return vertices[indice];
    }

    // Imprime o grafo
    void imprimeGrafo() override {
        cout << "Lista de Adjacência:" << endl;
        for (size_t i = 0; i < vertices.size(); i++) {
            cout << vertices[i] << ": ";
            for (const auto& aresta : lista[i]) {
                cout << "(" << vertices[aresta.destino] << ", " << aresta.peso << ") ";
            }
            cout << endl;
        }
    }

    // Insere uma aresta entre dois vértices
    bool inserirAresta(int origem, int destino, int peso = 1) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        lista[origem].emplace_back(destino, peso);
        if (!direcionado) {
            lista[destino].emplace_back(origem, peso); // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices
    bool removerAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        lista[origem].erase(
            remove_if(lista[origem].begin(), lista[origem].end(),
                      [destino](const Aresta& a) { return a.destino == destino; }),
            lista[origem].end());

        if (!direcionado) {
            lista[destino].erase(
                remove_if(lista[destino].begin(), lista[destino].end(),
                          [origem](const Aresta& a) { return a.destino == origem; }),
                lista[destino].end());
        }

        return true;
    }

    // Verifica se existe uma aresta entre dois vértices
    bool existeAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        for (const auto& aresta : lista[origem]) {
            if (aresta.destino == destino) {
                return true;
            }
        }
        return false;
    }

    // Retorna o peso de uma aresta
    float pesoAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return -1;

        for (const auto& aresta : lista[origem]) {
            if (aresta.destino == destino) {
                return aresta.peso;
            }
        }
        return -1;
    }

    // Retorna os vizinhos de um vértice
    vector<int> retornarVizinhos(int vertice) override {
        vector<int> vizinhos;
        if (vertice < 0 || vertice >= vertices.size()) return vizinhos;

        for (const auto& aresta : lista[vertice]) {
            vizinhos.push_back(aresta.destino);
        }
        return vizinhos;
    }
};

// Função principal para testar as implementações
int main() {
    // Exemplo com GrafoMatriz
    GrafoMatriz grafoMatriz(false, true); // Grafo não direcionado e ponderado
    grafoMatriz.inserirVertice("A");
    grafoMatriz.inserirVertice("B");
    grafoMatriz.inserirVertice("C");
    grafoMatriz.inserirAresta(0, 1, 5); // A -> B com peso 5
    grafoMatriz.inserirAresta(1, 2, 3); // B -> C com peso 3
    grafoMatriz.imprimeGrafo();

    // Exemplo com GrafoLista
    GrafoLista grafoLista(true, false); // Grafo direcionado e não ponderado
    grafoLista.inserirVertice("X");
    grafoLista.inserirVertice("Y");
    grafoLista.inserirVertice("Z");
    grafoLista.inserirAresta(0, 1); // X -> Y
    grafoLista.inserirAresta(1, 2); // Y -> Z
    grafoLista.imprimeGrafo();

    return 0;
}
