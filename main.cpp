// Trabalho M1.1
// Grafos 2025 - 1
// Alunos: 
//      Hérick Vitor Vieira Bittencourt
//      Rodrigo Rey de Oliveira Carrard
//      Paola Piran Zanella

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// Classe base abstrata para grafos
class Grafos {
public:
    bool direcionado;  // Indica se o grafo é direcionado
    bool ponderado;     // Indica se o grafo possui pesos nas arestas

    // Construtor
    Grafos(bool Direcionado, bool Ponderado) {
        direcionado = Direcionado;
        ponderado = Ponderado;
    }

    // Métodos para leitura de arquivos
    bool lerArquivo(const string& filePath) {
        cout << "LEITURA DE ARQUIVO:" << endl;
        cout << "-----------------" << endl;
        try {
            fstream arquivo(filePath);
            int numVertices = 0, numArestas = 0;


            // Leitura do cabeçalho, separado por espaços
            // Deve ser feito a leitura de linha (tratativa para \n)
            // Seguido da leitura da stringstream (tratativa para espaços)
            string cabecalho;
            getline(arquivo, cabecalho);
            stringstream cabecalhoStringStream(cabecalho);
            for (int i = 0; i < 4; i++) {
                string trecho;
                getline(cabecalhoStringStream, trecho, ' ');

                switch (i) {
                    case 0:
                        numVertices = stoi(trecho);
                        break;
                    case 1:
                        numArestas = stoi(trecho);
                        break;
                    case 2:
                        direcionado = stoi(trecho);
                        break;
                    case 3:
                        ponderado = stoi(trecho);
                        break;
                    default:
                        break;
                }
            }

            cout << "V: " << numVertices << " | A: " << numArestas << " | D: " << direcionado << " | P: " << ponderado << endl;
            cout << "-----------------" << endl;

            // Carregamento das vertices
            for (int i = 0; i < numVertices; i++) {
                inserirVertice("VERTEX_" + i);
            }

            // Carregamentos das arestas
            for (int i = 0; i < numArestas; i++) {
                // Obtém linha completa
                string linha;
                getline(arquivo, linha);
                stringstream linhaStringStream(linha);

                string tmp_vertice_origem, tmp_vertice_destino;
                int vertice_origem, vertice_destino, aresta_peso;

                // Obtém variaveis através da separação de espaços
                getline(linhaStringStream, tmp_vertice_origem, ' ');
                getline(linhaStringStream, tmp_vertice_destino, ' ');

                vertice_origem = stoi(tmp_vertice_origem);
                vertice_destino = stoi(tmp_vertice_destino);
                if (ponderado) {
                    string tmp_aresta_peso;
                    getline(linhaStringStream, tmp_aresta_peso, ' ');
                    aresta_peso = stoi(tmp_aresta_peso);
                }

                inserirAresta(vertice_origem, vertice_destino, aresta_peso);

                cout << vertice_origem << " " << vertice_destino << " " << ((ponderado) ? aresta_peso : 0) << endl;
                if (!direcionado) {
                    cout << vertice_destino << " " << vertice_origem << " " << ((ponderado) ? aresta_peso : 0) << endl;
                }
            }

            return true;
        } catch (exception &e) {
            cout << "Não foi possivel ler o arquivo: " << filePath << "\nMotivo: " << e.what() << endl;
        }
        return false;
    }



    // Métodos virtuais que deverão ser implementados nas classes derivadas
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
    int destino;  // Índice do vértice de destino
    int peso;     // Peso da aresta
    Aresta(int d, int p) : destino(d), peso(p) {}
};

// Implementação usando matriz de adjacência
class GrafoMatriz : public Grafos {
private:
    vector<vector<int>> matriz; // Matriz de adjacência para armazenar arestas e pesos
    vector<string> vertices;    // Vetor para armazenar os rótulos dos vértices

public:
    // Construtor que inicializa o grafo de matriz chamando o construtor da classe base
    GrafoMatriz(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo
    bool inserirVertice(string label) override {
        vertices.push_back(label);

        // Para cada linha já existente na matriz, adiciona 0 para a nova coluna
        for (auto& linha : matriz) {
            linha.push_back(0); // Inicializa com 0 (sem aresta)
        }

        // Adiciona uma nova linha com 0s equivalente a qtd. de vertices
        matriz.push_back(vector<int>(vertices.size(), 0)); // Nova linha para o novo vértice
        return true;
    }

    // Remove um vértice do grafo, removendo a linha e a coluna correspondente
    bool removerVertice(int indice) override {
        // Verifica se o índice é válido
        if (indice < 0 || indice >= vertices.size()) return false;

        // Remove o vértice do vetor de rótulos
        vertices.erase(vertices.begin() + indice);
        // Remove a linha correspondente na matriz de adjacência
        matriz.erase(matriz.begin() + indice);

        // Remove a coluna correspondente em todas as demais linhas da matriz
        for (auto& linha : matriz) {
            linha.erase(linha.begin() + indice);
        }

        return true;
    }

    // Retorna o rótulo de um vértice a partir do seu índice
    string labelVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return "";
        return vertices[indice];
    }

    // Imprime o grafo mostrando os rótulos dos vértices e a matriz de adjacência
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

    // Insere uma aresta entre dois vértices, atribuindo o peso fornecido
    bool inserirAresta(int origem, int destino, int peso = 1) override {
        // Validação dos índices de origem e destino
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        // Atribui o peso na posição correspondente da matriz
        matriz[origem][destino] = peso;
        if (!direcionado) {
            matriz[destino][origem] = peso; // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices, validando os índices e definindo o valor para 0 (sem aresta)
    bool removerAresta(int origem, int destino) override {
        // Verifica se os índices estão dentro dos limites
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        // Remove a aresta atribuindo 0 à posição na matriz
        matriz[origem][destino] = 0;
        // Para grafos não direcionados, remove também a aresta inversa
        if (!direcionado) {
            matriz[destino][origem] = 0; // Remove a aresta bidirecional
        }
        return true;
    }

    // Verifica se existe uma aresta entre os vértices de origem e destino
    // Aresta válida tem peso diferente de zero
    bool existeAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;
        return matriz[origem][destino] != 0;
    }

    // Retorna o peso da aresta entre dois vértices
    // se não existir, retorna -1
    float pesoAresta(int origem, int destino) override {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return -1;
        return matriz[origem][destino];
    }

    // Retorna um vetor com os índices dos vértices vizinhos do vértice informado
    vector<int> retornarVizinhos(int vertice) override {
        vector<int> vizinhos;
        // Verifica se o índice do vértice é válido
        if (vertice < 0 || vertice >= vertices.size()) return vizinhos;

        // Percorre a linha da matriz correspondente ao vértice e adiciona os índices com arestas existentes
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
    vector<vector<Aresta>> lista; // Lista de adjacência para armazenar as arestas de cada vértice
    vector<string> vertices;      // Vetor para armazenar os rótulos dos vértices

public:
    // Construtor
    GrafoLista(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo [OK]
    bool inserirVertice(string label) override {
        vertices.push_back(label);
        lista.push_back(vector<Aresta>()); // Adiciona uma lista vazia para o novo vértice
        return true;
    }

    // Remove um vértice do grafo, removendo o rótulo, a lista de arestas e ajustando as referências
    bool removerVertice(int indice) override {
        // Valida se o índice é válido
        if (indice < 0 || indice >= vertices.size()) return false;

        // Remove o vértice do vetor de rótulos e da lista de adjacência
        vertices.erase(vertices.begin() + indice);
        lista.erase(lista.begin() + indice);

        // Remove todas as arestas que apontam para o vértice removido
        for (auto& vizinhos : lista) {
            vizinhos.erase(
                remove_if(vizinhos.begin(), vizinhos.end(),
                          [indice](const Aresta& a) { return a.destino == indice; }),
                vizinhos.end());
        }

        // Ajusta os índices de destino nas arestas que apontavam para vértices com índice maior que o removido
        for (auto& vizinhos : lista) {
            for (auto& aresta : vizinhos) {
                if (aresta.destino > indice) {
                    aresta.destino--;
                }
            }
        }

        return true;
    }

    // Retorna o rótulo de um vértice, após validação do índice
    string labelVertice(int indice) override {
        if (indice < 0 || indice >= vertices.size()) return "";
        return vertices[indice];
    }

    // Imprime o grafo mostrando a lista de adjacência com os rótulos e pesos das arestas
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

    // Insere uma aresta entre dois vértices na lista de adjacência, adicionando aresta inversa se necessário
    bool inserirAresta(int origem, int destino, int peso = 1) override {
        // Verifica se os índices dos vértices são válidos
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        // Adiciona a aresta na lista do vértice de origem
        lista[origem].emplace_back(destino, peso);
        // Se o grafo não for direcionado, adiciona a aresta na direção oposta
        if (!direcionado) {
            lista[destino].emplace_back(origem, peso); // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices na lista de adjacência
    bool removerAresta(int origem, int destino) override {
        // Valida os índices dos vértices
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        // Remove a aresta da lista do vértice de origem utilizando remove_if
        lista[origem].erase(
            remove_if(lista[origem].begin(), lista[origem].end(),
                      [destino](const Aresta& a) { return a.destino == destino; }),
            lista[origem].end());

        // Para grafos não direcionados, remove também a aresta na direção inversa
        if (!direcionado) {
            lista[destino].erase(
                remove_if(lista[destino].begin(), lista[destino].end(),
                          [origem](const Aresta& a) { return a.destino == origem; }),
                lista[destino].end());
        }

        return true;
    }

    // Verifica se existe uma aresta entre os vértices de origem e destino na lista de adjacência
    bool existeAresta(int origem, int destino) override {
        // Valida os índices e percorre a lista do vértice de origem para encontrar a aresta
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return false;

        for (const auto& aresta : lista[origem]) {
            if (aresta.destino == destino) {
                return true;
            }
        }
        return false;
    }

    // Retorna o peso da aresta entre dois vértices; se não existir, retorna -1
    float pesoAresta(int origem, int destino) override {
        // Valida os índices e procura a aresta na lista do vértice de origem
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) return -1;

        for (const auto& aresta : lista[origem]) {
            if (aresta.destino == destino) {
                return aresta.peso;
            }
        }
        return -1;
    }

    // Retorna um vetor com os índices dos vértices vizinhos do vértice informado
    vector<int> retornarVizinhos(int vertice) override {
        vector<int> vizinhos;
        // Verifica se o índice é válido
        if (vertice < 0 || vertice >= vertices.size()) return vizinhos;

        // Adiciona cada destino presente na lista de arestas do vértice
        for (const auto& aresta : lista[vertice]) {
            vizinhos.push_back(aresta.destino);
        }
        return vizinhos;
    }
};

// Função principal para testar as implementações dos grafos
int main() {
    GrafoMatriz grafoMatriz(false, true);
    grafoMatriz.lerArquivo("teste.txt");
    /*
    // Exemplo com GrafoMatriz
    GrafoMatriz grafoMatriz(false, true); // Cria um grafo não direcionado e ponderado usando matriz de adjacência
    grafoMatriz.inserirVertice("A");
    grafoMatriz.inserirVertice("B");
    grafoMatriz.inserirVertice("C");
    grafoMatriz.inserirAresta(0, 1, 5);       // Insere aresta de A para B com peso 5
    grafoMatriz.inserirAresta(1, 2, 3);       // Insere aresta de B para C com peso 3
    grafoMatriz.imprimeGrafo();

    // Exemplo com GrafoLista
    GrafoLista grafoLista(true, false);     // Cria um grafo direcionado e não ponderado usando lista de adjacência
    grafoLista.inserirVertice("X");
    grafoLista.inserirVertice("Y");
    grafoLista.inserirVertice("Z");
    grafoLista.inserirAresta(0, 1);         // Insere aresta de X para Y
    grafoLista.inserirAresta(1, 2);         // Insere aresta de Y para Z
    grafoLista.imprimeGrafo();              // Exibe o grafo em forma de lista de adjacência
    */


    return 0;
}
