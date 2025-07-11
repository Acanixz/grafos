// Trabalho M3.1 - AGM
// Grafos 2025 - 1
// Alunos:
//      Hérick Vitor Vieira Bittencourt
//      Rodrigo Rey de Oliveira Carrard
//      Paola Piran Zanella
//      Cauã Domingos

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>
#include <climits>
#include <chrono>
#include <set>
#include <stack>
#include <functional>
#include <numeric>
using namespace std;

// Classe base abstrata para grafos
class Grafos
{
public:
    bool direcionado; // Indica se o grafo é direcionado
    bool ponderado;   // Indica se o grafo possui pesos nas arestas
    int qtdVertices = 0;
    virtual vector<int> encontrarCaminhoAumentante(int origem, int destino) = 0;
    virtual int fordFulkerson(int origem, int destino) = 0;
    virtual pair<int, int> buscaLocalFluxoMaximo(int origem, int destino) = 0;

    // Construtor
    Grafos(bool Direcionado, bool Ponderado)
    {
        direcionado = Direcionado;
        ponderado = Ponderado;
    }

    // Métodos para leitura de arquivos
    bool lerArquivo(const string &filePath)
    {
        cout << "LEITURA DE ARQUIVO:" << endl;
        cout << "-----------------" << endl;
        try
        {
            fstream arquivo(filePath);
            int numVertices = 0, numArestas = 0;

            // Leitura do cabeçalho, separado por espaços
            // Deve ser feito a leitura de linha (tratativa para \n)
            // Seguido da leitura da stringstream (tratativa para espaços)
            string cabecalho;
            getline(arquivo, cabecalho);
            stringstream cabecalhoStringStream(cabecalho);
            for (int i = 0; i < 4; i++)
            {
                string trecho;
                getline(cabecalhoStringStream, trecho, ' ');

                switch (i)
                {
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
            for (int i = 0; i < numVertices; i++)
            {
                inserirVertice("VERTEX_" + i);
            }

            // Carregamentos das arestas
            for (int i = 0; i < numArestas; i++)
            {
                // Obtém linha completa
                string linha;
                getline(arquivo, linha);
                stringstream linhaStringStream(linha);

                string tmp_vertice_origem, tmp_vertice_destino;
                int vertice_origem, vertice_destino;
                float aresta_peso;

                // Obtém variaveis através da separação de espaços
                getline(linhaStringStream, tmp_vertice_origem, ' ');
                getline(linhaStringStream, tmp_vertice_destino, ' ');

                vertice_origem = stoi(tmp_vertice_origem);
                vertice_destino = stoi(tmp_vertice_destino);
                if (ponderado)
                {
                    string tmp_aresta_peso;
                    getline(linhaStringStream, tmp_aresta_peso, ' ');
                    aresta_peso = stof(tmp_aresta_peso);
                }

                inserirAresta(vertice_origem, vertice_destino, aresta_peso);

                cout << vertice_origem << " " << vertice_destino << " " << ((ponderado) ? aresta_peso : 0) << endl;
                if (!direcionado)
                {
                    cout << vertice_destino << " " << vertice_origem << " " << ((ponderado) ? aresta_peso : 0) << endl;
                }
            }

            cout << "Leitura concluida com exito\n" << endl;
            return true;
        }
        catch (exception &e)
        {
            cout << "Nao foi possivel ler o arquivo: " << filePath << "\nMotivo: " << e.what() << endl;
        }
        return false;
    }

    // Algoritmo de Breadth First Search (BFS)
    vector<int> breadthFirstSearch(int verticeOrigem)
    {
        vector<bool> visitado(qtdVertices, false); // Inicializa todos como não visitados
        vector<int> ordemVisita;                   // Guarda a ordem de visita
        queue<int> fila;                           // Fila para controlar a BFS

        // Marca o vértice inicial e o adiciona na fila
        visitado[verticeOrigem] = true;
        fila.push(verticeOrigem);

        while (!fila.empty())
        {
            int verticeAtual = fila.front();
            fila.pop();
            ordemVisita.push_back(verticeAtual); // Adiciona à ordem de visita

            // Pega os vizinhos do vértice atual
            vector<int> vizinhos = retornarVizinhos(verticeAtual);

            // Adiciona vizinhos não visitados na fila
            for (int vizinho : vizinhos)
            {
                if (!visitado[vizinho])
                {
                    visitado[vizinho] = true;
                    fila.push(vizinho);
                }
            }
        }

        // Imprime a ordem de visita
        cout << "Ordem de visita (BFS): ";
        for (int vertice : ordemVisita)
        {
            cout << vertice << " ";
        }
        cout << endl;

        return ordemVisita;
    }

    // Algoritmo de busca Depth First Search
    // Começa em uma vertice e chama recursivamente os vizinhos,
    // Marcando-os como "visitado" e retornando os índices visitados p/ chamada pai
    vector<int> depthFirstSearch(int verticeOrigem)
    {
        // Verificar o tamanho dos vértices e inicializar o vetor de visitados
        vector<bool> visitado(this->qtdVertices, false);
        vector<int> resultado;

        // Iniciar a DFS recursiva
        _dfsRec(verticeOrigem, visitado, resultado);

        if (resultado.empty())
        {
            cout << "Nenhum caminho encontrado" << endl;
        }
        else
        {
            cout << "Caminho encontrado: ";
            for (int vertice : resultado)
            {
                cout << vertice << " ";
            }
            cout << endl;
        }

        return resultado;
    }

    // Função recursiva auxiliar para DFS
    void _dfsRec(int verticeAtual, vector<bool> &visitado, vector<int> &resultado)
    {
        // Marcar o vértice atual como visitado e adicioná-lo ao resultado
        visitado[verticeAtual] = true;
        resultado.push_back(verticeAtual);

        // Obter vizinhos do vértice atual
        vector<int> vizinhos = retornarVizinhos(verticeAtual);

        // Percorrer os vizinhos
        for (int vizinho : vizinhos)
        {
            if (!visitado[vizinho])
            {
                _dfsRec(vizinho, visitado, resultado);
            }
        }
    }

    vector<pair<int, vector<int>>> dijkstra(int verticeOrigem) {
        // Verifica se o grafo é ponderado
        if (!ponderado) {
            cout << "Dijkstra requer um grafo ponderado!" << endl;
            return {};
        }
    
        // Inicializa distâncias com infinito e caminhos vazios
        vector<int> distancias(qtdVertices, INT_MAX);
        vector<vector<int>> caminhos(qtdVertices);
        vector<bool> visitado(qtdVertices, false);
    
        // A distância para o próprio vértice de origem é 0
        distancias[verticeOrigem] = 0;
        caminhos[verticeOrigem].push_back(verticeOrigem);
    
        // Fila de prioridade para processar os vértices (distância, vértice)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fila;
        fila.push({0, verticeOrigem});
    
        while (!fila.empty()) {
            // Extrai o vértice com menor distância atual
            int u = fila.top().second;
            fila.pop();
    
            // Se já foi visitado, pula
            if (visitado[u]) continue;
            visitado[u] = true;
    
            // Para cada vizinho do vértice atual
            for (int v : retornarVizinhos(u)) {
                float peso = pesoAresta(u, v);
                
                // Relaxamento da aresta
                if (distancias[v] > distancias[u] + peso) {
                    distancias[v] = distancias[u] + peso;
                    caminhos[v] = caminhos[u]; // Copia o caminho até u
                    caminhos[v].push_back(v);  // Adiciona v ao caminho
                    fila.push({distancias[v], v});
                }
            }
        }

        //Implementação do Dijkstra
        // Prepara o resultado (distância e caminho para cada vértice)
        vector<pair<int, vector<int>>> resultado;
        for (int i = 0; i < qtdVertices; i++) {
            resultado.emplace_back(distancias[i], caminhos[i]);
        }
    
        // Imprime os resultados
        cout << "\nResultados do Dijkstra a partir do vertice " << verticeOrigem << ":\n";
        for (int i = 0; i < qtdVertices; i++) {
            cout << "Vertice " << i << ": ";
            if (distancias[i] == INT_MAX) {
                cout << "Inalcancavel";
            } else {
                cout << "Distancia = " << distancias[i] << ", Caminho = ";
                for (int v : caminhos[i]) {
                    cout << v;
                    if (v != caminhos[i].back()) cout << " -> ";
                }
            }
            cout << endl;
        }
    
        return resultado;
    }

    bool coloracao_valida(const vector<int>& cores) {
        for (int vertice = 0; vertice < qtdVertices; vertice++) {
            for (int outraV : retornarVizinhos(vertice)) {
                if (cores[vertice] == cores[outraV]) return false;
            }
        }
        return true;
    }

    void coloracao_bruta() {
        cout << "\nColoracao Forca Bruta" << endl;
        auto t0 = chrono::high_resolution_clock::now();
        vector<int> cores(qtdVertices, 0);
        int sol_k = 0;
        // tenta números de cores de 1 até qtdVertices
        for (int k = 1; k <= qtdVertices; ++k) {
            // todas as combinações: cada vértice em [1..k]
            function<bool(int)> backtrack = [&](int idx) {
                if (idx == qtdVertices) {
                    if (coloracao_valida(cores)) return true;
                    return false;
                }
                for (int c = 1; c <= k; ++c) {
                    cores[idx] = c;
                    if (backtrack(idx + 1)) return true;
                }
                return false;
            };
            if (backtrack(0)) {
                sol_k = k;
                break;
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        cout << "Cores usadas: " << sol_k << " | Tempo: " << dur << " ms" << endl;
        if (qtdVertices < 10) {
            cout << "Atribuicoes: ";
            for (int i = 0; i < qtdVertices; ++i)
                cout << i << ":" << cores[i] << " ";
            cout << endl;
        }
    }

    void coloracao_welshpowell() {
        cout << "\nColoracao Welsh-Powell" << endl;
        auto t0 = chrono::high_resolution_clock::now();
        vector<pair<int,int>> grau_idx;
        for (int u = 0; u < qtdVertices; ++u) {
            grau_idx.emplace_back(retornarVizinhos(u).size(), u);
        }
        sort(grau_idx.rbegin(), grau_idx.rend()); // decrescente
        vector<int> cores(qtdVertices, 0);
        int proxima_cor = 1;
        for (int i = 0; i < qtdVertices; ++i) {
            int u = grau_idx[i].second;
            if (cores[u] != 0) continue;
            // tenta colorir todos restantes com cor atual
            cores[u] = proxima_cor;
            for (int j = i+1; j < qtdVertices; ++j) {
                int v = grau_idx[j].second;
                if (cores[v] == 0) {
                    bool ok = true;
                    for (int w : retornarVizinhos(v)) {
                        if (cores[w] == proxima_cor) { ok = false; break; }
                    }
                    if (ok) cores[v] = proxima_cor;
                }
            }
            proxima_cor++;
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        int cores_usadas = *max_element(cores.begin(), cores.end());
        cout << "Cores usadas: " << cores_usadas << " | Tempo: " << dur << " ms" << endl;
        if (qtdVertices < 10) {
            cout << "Atribuicoes: ";
            for (int i = 0; i < qtdVertices; ++i)
                cout << i << ":" << cores[i] << " ";
            cout << endl;
        }
    }

    void coloracao_DSATUR() {
        cout << "\nColoracao DSATUR" << endl;
        auto t0 = chrono::high_resolution_clock::now();
        vector<int> cores(qtdVertices, 0);
        vector<set<int>> neigh_colors(qtdVertices);
        vector<int> grau(qtdVertices);
        for (int u = 0; u < qtdVertices; ++u) {
            grau[u] = retornarVizinhos(u).size();
        }
        // inicial: escolhe maior grau
        int u0 = max_element(grau.begin(), grau.end()) - grau.begin();
        cores[u0] = 1;
        for (int v : retornarVizinhos(u0))
            neigh_colors[v].insert(1);
        // itera até colorir todos
        for (int count = 1; count < qtdVertices; ++count) {
            // escolhe vértice sem cor de maior saturação (tamanho de neigh_colors), tie-breaker grau
            int best = -1;
            pair<int,int> best_key = {-1,-1};
            for (int u = 0; u < qtdVertices; ++u) {
                if (cores[u] == 0) {
                    int sat = neigh_colors[u].size();
                    pair<int,int> key = {sat, grau[u]};
                    if (key > best_key) { best_key = key; best = u; }
                }
            }
            // atribui menor cor não em neigh_colors[best]
            int c = 1;
            while (neigh_colors[best].count(c)) ++c;
            cores[best] = c;
            for (int v : retornarVizinhos(best))
                neigh_colors[v].insert(c);
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        int cores_usadas = *max_element(cores.begin(), cores.end());
        cout << "Cores usadas: " << cores_usadas << " | Tempo: " << dur << " ms" << endl;
        if (qtdVertices < 10) {
            cout << "Atribuicoes: ";
            for (int i = 0; i < qtdVertices; ++i)
                cout << i << ":" << cores[i] << " ";
            cout << endl;
        }
    }

    void coloracao_sequencial() {
        cout << "\nColoracao Sequencial (sem criterio de ordem)" << endl;
        auto t0 = chrono::high_resolution_clock::now();

        // vetor 1..cores, 0 = sem cor
        vector<int> cores(qtdVertices, 0);

        for (int u = 0; u < qtdVertices; ++u) {
            set<int> usadas;
            for (int w : retornarVizinhos(u)) {
                if (cores[w] != 0)
                    usadas.insert(cores[w]);
            }
            int cor = 1;
            while (usadas.count(cor)) ++cor;
            cores[u] = cor;
        }

        auto t1 = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        int total = *max_element(cores.begin(), cores.end());
        cout << "Cores usadas: " << total << " | Tempo: " << dur << " ms" << endl;

        if (qtdVertices < 10) {
            cout << "Atribuicoes: ";
            for (int i = 0; i < qtdVertices; ++i)
                cout << i << ":" << cores[i] << " ";
            cout << endl;
        }
    }



    // Métodos virtuais que deverão ser implementados nas classes derivadas
    virtual bool inserirVertice(string label) = 0;
    virtual bool removerVertice(int indice) = 0;
    virtual string labelVertice(int indice) = 0;
    virtual void imprimeGrafo() = 0;
    virtual bool inserirAresta(int origem, int destino, float peso = 1.0) = 0;
    virtual bool removerAresta(int origem, int destino) = 0;
    virtual bool existeAresta(int origem, int destino) = 0;
    virtual float pesoAresta(int origem, int destino) = 0;
    virtual vector<int> retornarVizinhos(int vertice) = 0;
};

// Estrutura para representar uma aresta na lista de adjacência
struct Aresta
{
    int destino; // Índice do vértice de destino
    float peso;    // Peso da aresta
    Aresta(int d, float p) : destino(d), peso(p) {}
};

// Implementação usando matriz de adjacência
class GrafoMatriz : public Grafos
{
private:
    vector<vector<float>> matriz; // Matriz de adjacência para armazenar arestas e pesos
    vector<string> vertices;    // Vetor para armazenar os rótulos dos vértices

public:
    // Construtor que inicializa o grafo de matriz chamando o construtor da classe base
    GrafoMatriz(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo
    bool inserirVertice(string label) override
    {
        vertices.push_back(label);

        // Para cada linha já existente na matriz, adiciona 0 para a nova coluna
        for (auto &linha : matriz)
        {
            linha.push_back(0); // Inicializa com 0 (sem aresta)
        }

        // Adiciona uma nova linha com 0s equivalente a qtd. de vertices
        matriz.push_back(vector<float>(vertices.size(), 0)); // Nova linha para o novo vértice
        qtdVertices += 1;
        return true;
    }

    // Remove um vértice do grafo, removendo a linha e a coluna correspondente
    bool removerVertice(int indice) override
    {
        // Verifica se o índice é válido
        if (indice < 0 || indice >= vertices.size())
            return false;

        // Remove o vértice do vetor de rótulos
        vertices.erase(vertices.begin() + indice);
        // Remove a linha correspondente na matriz de adjacência
        matriz.erase(matriz.begin() + indice);

        // Remove a coluna correspondente em todas as demais linhas da matriz
        for (auto &linha : matriz)
        {
            linha.erase(linha.begin() + indice);
        }

        return true;
    }

    // Retorna o rótulo de um vértice a partir do seu índice
    string labelVertice(int indice) override
    {
        if (indice < 0 || indice >= vertices.size())
            return "";
        return vertices[indice];
    }

    // Imprime o grafo mostrando os rótulos dos vértices e a matriz de adjacência
    void imprimeGrafo() override
    {
        cout << "Matriz de Adjacência:" << endl;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            cout << "V" << i << ": ";
            for (size_t j = 0; j < vertices.size(); j++)
            {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Insere uma aresta entre dois vértices, atribuindo o peso fornecido
    bool inserirAresta(int origem, int destino, float peso = 1.0) override
    {
        // Validação dos índices de origem e destino
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;

        // Atribui o peso na posição correspondente da matriz
        matriz[origem][destino] = peso;
        if (!direcionado)
        {
            matriz[destino][origem] = peso; // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices, validando os índices e definindo o valor para 0 (sem aresta)
    bool removerAresta(int origem, int destino) override
    {
        // Verifica se os índices estão dentro dos limites
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;

        // Remove a aresta atribuindo 0 à posição na matriz
        matriz[origem][destino] = 0;
        // Para grafos não direcionados, remove também a aresta inversa
        if (!direcionado)
        {
            matriz[destino][origem] = 0; // Remove a aresta bidirecional
        }
        return true;
    }

    // Verifica se existe uma aresta entre os vértices de origem e destino
    // Aresta válida tem peso diferente de zero
    bool existeAresta(int origem, int destino) override
    {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;
        return matriz[origem][destino] != 0;
    }

    // Retorna o peso da aresta entre dois vértices
    // se não existir, retorna -1
    float pesoAresta(int origem, int destino) override
    {
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return -1;
        return matriz[origem][destino];
    }

    // Retorna um vetor com os índices dos vértices vizinhos do vértice informado
    vector<int> retornarVizinhos(int vertice) override
    {
        vector<int> vizinhos;
        // Verifica se o índice do vértice é válido
        if (vertice < 0 || vertice >= vertices.size())
            return vizinhos;

        // Percorre a linha da matriz correspondente ao vértice e adiciona os índices com arestas existentes
        for (size_t i = 0; i < vertices.size(); i++)
        {
            if (matriz[vertice][i] != 0)
            {
                vizinhos.push_back(i);
            }
        }
        return vizinhos;
    }
};

// Implementação usando lista de adjacência
class GrafoLista : public Grafos
{
private:
    vector<vector<Aresta>> lista; // Lista de adjacência para armazenar as arestas de cada vértice
    vector<string> vertices;      // Vetor para armazenar os rótulos dos vértices

public:
    // Construtor
    GrafoLista(bool Direcionado, bool Ponderado) : Grafos(Direcionado, Ponderado) {}

    // Adiciona um vértice ao grafo [OK]
    bool inserirVertice(string label) override
    {
        vertices.push_back(label);
        lista.push_back(vector<Aresta>()); // Adiciona uma lista vazia para o novo vértice

        qtdVertices += 1;
        return true;
    }

    // Remove um vértice do grafo, removendo o rótulo, a lista de arestas e ajustando as referências
    bool removerVertice(int indice) override
    {
        // Valida se o índice é válido
        if (indice < 0 || indice >= vertices.size())
            return false;

        // Remove o vértice do vetor de rótulos e da lista de adjacência
        vertices.erase(vertices.begin() + indice);
        lista.erase(lista.begin() + indice);

        // Remove todas as arestas que apontam para o vértice removido
        for (auto &vizinhos : lista)
        {
            vizinhos.erase(
                remove_if(vizinhos.begin(), vizinhos.end(),
                          [indice](const Aresta &a)
                          { return a.destino == indice; }),
                vizinhos.end());
        }

        // Ajusta os índices de destino nas arestas que apontavam para vértices com índice maior que o removido
        for (auto &vizinhos : lista)
        {
            for (auto &aresta : vizinhos)
            {
                if (aresta.destino > indice)
                {
                    aresta.destino--;
                }
            }
        }

        return true;
    }

    // Retorna o rótulo de um vértice, após validação do índice
    string labelVertice(int indice) override
    {
        if (indice < 0 || indice >= vertices.size())
            return "";
        return vertices[indice];
    }

    // Imprime o grafo mostrando a lista de adjacência com os rótulos e pesos das arestas
    void imprimeGrafo() override
    {
        cout << "Lista de Adjacência:" << endl;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            cout << vertices[i] << ": ";
            for (const auto &aresta : lista[i])
            {
                cout << "(" << vertices[aresta.destino] << ", " << aresta.peso << ") ";
            }
            cout << endl;
        }
    }

    // Insere uma aresta entre dois vértices na lista de adjacência, adicionando aresta inversa se necessário
    bool inserirAresta(int origem, int destino, float peso = 1.0) override
    {
        // Verifica se os índices dos vértices são válidos
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;

        // Adiciona a aresta na lista do vértice de origem
        lista[origem].emplace_back(destino, peso);
        // Se o grafo não for direcionado, adiciona a aresta na direção oposta
        if (!direcionado)
        {
            lista[destino].emplace_back(origem, peso); // Aresta bidirecional
        }
        return true;
    }

    // Remove uma aresta entre dois vértices na lista de adjacência
    bool removerAresta(int origem, int destino) override
    {
        // Valida os índices dos vértices
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;

        // Remove a aresta da lista do vértice de origem utilizando remove_if
        lista[origem].erase(
            remove_if(lista[origem].begin(), lista[origem].end(),
                      [destino](const Aresta &a)
                      { return a.destino == destino; }),
            lista[origem].end());

        // Para grafos não direcionados, remove também a aresta na direção inversa
        if (!direcionado)
        {
            lista[destino].erase(
                remove_if(lista[destino].begin(), lista[destino].end(),
                          [origem](const Aresta &a)
                          { return a.destino == origem; }),
                lista[destino].end());
        }

        return true;
    }

    // Verifica se existe uma aresta entre os vértices de origem e destino na lista de adjacência
    bool existeAresta(int origem, int destino) override
    {
        // Valida os índices e percorre a lista do vértice de origem para encontrar a aresta
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return false;

        for (const auto &aresta : lista[origem])
        {
            if (aresta.destino == destino)
            {
                return true;
            }
        }
        return false;
    }

    // Retorna o peso da aresta entre dois vértices; se não existir, retorna -1
    float pesoAresta(int origem, int destino) override
    {
        // Valida os índices e procura a aresta na lista do vértice de origem
        if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size())
            return -1;

        for (const auto &aresta : lista[origem])
        {
            if (aresta.destino == destino)
            {
                return aresta.peso;
            }
        }
        return -1;
    }

    // Retorna um vetor com os índices dos vértices vizinhos do vértice informado
    vector<int> retornarVizinhos(int vertice) override
    {
        vector<int> vizinhos;
        // Verifica se o índice é válido
        if (vertice < 0 || vertice >= vertices.size())
            return vizinhos;

        // Adiciona cada destino presente na lista de arestas do vértice
        for (const auto &aresta : lista[vertice])
        {
            vizinhos.push_back(aresta.destino);
        }
        return vizinhos;
    }

    // Implementação do metodo para encontrar caminho aumentante usando DFS
    vector<int> encontrarCaminhoAumentante(int origem, int destino) override {
        vector<bool> visitado(qtdVertices, false);
        vector<int> caminho;
        vector<int> parent(qtdVertices, -1);
        
        stack<int> pilha;
        pilha.push(origem);
        visitado[origem] = true;
        
        while (!pilha.empty()) {
            int u = pilha.top();
            pilha.pop();
            
            if (u == destino) {
                // Reconstruir o caminho
                int v = destino;
                while (v != origem) {
                    caminho.insert(caminho.begin(), v);
                    v = parent[v];
                }
                caminho.insert(caminho.begin(), origem);
                return caminho;
            }
            
            for (const Aresta& aresta : lista[u]) {
                int v = aresta.destino;
                if (!visitado[v] && aresta.peso > 0) {
                    visitado[v] = true;
                    parent[v] = u;
                    pilha.push(v);
                }
            }
        }
        
        return {}; // Retorna vazio se não encontrar caminho
    }

    // Implementação do algoritmo Ford-Fulkerson
    int fordFulkerson(int origem, int destino) override {
        // Criar uma cópia do grafo original para trabalhar com capacidades residuais
        GrafoLista grafoResidual(this->direcionado, this->ponderado);
        for (int i = 0; i < qtdVertices; i++) {
            grafoResidual.inserirVertice(labelVertice(i));
        }
        
        // Inicializar grafo residual com capacidades originais
        for (int u = 0; u < qtdVertices; u++) {
            for (const Aresta& aresta : lista[u]) {
                int v = aresta.destino;
                float capacidade = aresta.peso;
                grafoResidual.inserirAresta(u, v, capacidade);
                //if (!direcionado) {
                    grafoResidual.inserirAresta(v, u, 0); // Aresta reversa inicialmente com 0
                //}
            }
        }
        
        int fluxoMaximo = 0;
        
        while (true) {
            // Encontrar caminho aumentante usando DFS
            vector<int> caminho = grafoResidual.encontrarCaminhoAumentante(origem, destino);
            
            if (caminho.empty()) {
                break; // Não há mais caminhos aumentantes
            }
            
            // Encontrar capacidade residual mínima no caminho
            float capacidadeMinima = numeric_limits<float>::max();
            for (size_t i = 0; i < caminho.size() - 1; i++) {
                int u = caminho[i];
                int v = caminho[i+1];
                float capacidade = grafoResidual.pesoAresta(u, v);
                if (capacidade < capacidadeMinima) {
                    capacidadeMinima = capacidade;
                }
            }
            
            // Atualizar capacidades residuais
            for (size_t i = 0; i < caminho.size() - 1; i++) {
                int u = caminho[i];
                int v = caminho[i+1];
                
                // Atualizar aresta direta
                float capacidadeAtual = grafoResidual.pesoAresta(u, v);
                grafoResidual.removerAresta(u, v);
                grafoResidual.inserirAresta(u, v, capacidadeAtual - capacidadeMinima);
                
                // Atualizar aresta reversa
                float capacidadeReversa = grafoResidual.pesoAresta(v, u);
                grafoResidual.removerAresta(v, u);
                grafoResidual.inserirAresta(v, u, capacidadeReversa + capacidadeMinima);
            }
            
            fluxoMaximo += capacidadeMinima;
        }
        
        return fluxoMaximo;
    }

    // Implementação da busca local para otimizar o fluxo máximo
    pair<int, int> buscaLocalFluxoMaximo(int origem, int destino) override {
        // Fluxo máximo da solução inicial
        int fluxoInicial = fordFulkerson(origem, destino);
        int fluxoAtual = fluxoInicial;
        int passos = 0;
        
        // Criar cópia do grafo original para modificações
        GrafoLista melhorGrafo = *this;
        
        bool melhorou;
        do {
            melhorou = false;
            
            // Gerar vizinhos invertendo direção de cada aresta
            for (int u = 0; u < qtdVertices; u++) {
                vector<Aresta> arestas = lista[u]; // Copia para iterar
                for (const Aresta& aresta : arestas) {
                    int v = aresta.destino;
                    
                    // Criar vizinho invertendo a aresta u->v
                    GrafoLista vizinho = melhorGrafo;
                    vizinho.removerAresta(u, v);
                    vizinho.inserirAresta(v, u, aresta.peso);
                    
                    // Calcular fluxo máximo do vizinho
                    int fluxoVizinho = vizinho.fordFulkerson(origem, destino);
                    passos++;
                    
                    // Se melhorou, atualizar melhor solução
                    if (fluxoVizinho > fluxoAtual) {
                        fluxoAtual = fluxoVizinho;
                        melhorGrafo = vizinho;
                        melhorou = true;
                        break; // Primeira melhora (estratégia greedy)
                    }
                }
                if (melhorou) break;
            }
        } while (melhorou);
        
        cout << "Numero de passos: " << passos << endl;
        return make_pair(fluxoInicial, fluxoAtual);
    }

    // Algoritmo de Prim para para AGM
    void primMST() {
        cout << "\nExecutando PrimMST" << endl;
        auto t0 = chrono::high_resolution_clock::now(); // inicia a contagem de tempo

        // Inicia um conjunto S vazio de arestas para a solução
        float totalPeso = 0.0f;
        int arestasSelecionadas = 0;

        // Inicia um conjunto Q com todos os vértices do grafo para o controle
        vector<bool> inMST(qtdVertices, false);
        // fila de prioridade mínima: pares (peso, vértice)
        priority_queue<pair<float,int>, vector<pair<float,int>>, greater<>> pq;

        // Escolhe um vértice arbitrário A do grafo como vértice inicial
        // Vertice 0 com prioridade 0
        pq.emplace(0.0f, 0);

        // Enquanto Q não estiver vazio
        while (!pq.empty() && arestasSelecionadas < qtdVertices - 1) {
            // Remove A do conjunto Q
            auto [peso, u] = pq.top(); pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true; // Remove do conjunto Q o vértice desta aresta (u ou v) que pertencia a ele

            // Adiciona a aresta {u, v} para o conjunto solução S
            totalPeso += peso;
            arestasSelecionadas++;

            // Encontra a menor aresta {u, v}, onde um deles (u ou v) pertence ao conjunto Q e o outro não pertence ao conjunto Q
            for (auto &aresta : lista[u]) {
                int v = aresta.destino;
                float w = aresta.peso;
                if (!inMST[v]) {
                    pq.emplace(w, v); // adiciona possível nova aresta para análise
                }
            }
        }

        auto t1 = chrono::high_resolution_clock::now(); // finaliza tempo
        auto dur = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

        // imprime resultado
        cout << "PrimMST - Soma das arestas: " << totalPeso
             << " | Tempo: " << dur << " ms" << endl;
    }


    // Algoritmo de Kruskal para AGM
    void kruskalMST() {
        cout << "\nExecutando KruskalMST" << endl;
        auto tempo_inicio = chrono::high_resolution_clock::now();

        // Inicia um conjunto Q com todas as arestas do grafo para o controle
        struct Aresta { int origem, destino; float peso; };
        vector<Aresta> conjuntoArestas;
        for (int origem = 0; origem < qtdVertices; origem++) {
            for (auto &a : lista[origem]) {
                if (origem < a.destino) { // evita duplicar arestas em grafo não direcionado
                    conjuntoArestas.push_back({origem, a.destino, a.peso});
                }
            }
        }
        // ordena as arestas por peso crescente
        sort(conjuntoArestas.begin(), conjuntoArestas.end(),
             [](const Aresta &a, const Aresta &b){ return a.peso < b.peso; });

        // Inicia uma floresta (um conjunto de árvores) F com cada vértice isolado sendo uma árvore
        vector<int> pai(qtdVertices), nivel(qtdVertices,0);
        iota(pai.begin(), pai.end(), 0);
        function<int(int)> encontrar = [&](int x){
            return pai[x] == x ? x : pai[x] = encontrar(pai[x]);
        };
        auto unir = [&](int x, int y){
            x = encontrar(x); y = encontrar(y);
            if (x == y) return false;
            if (nivel[x] < nivel[y]) swap(x,y);
            pai[y] = x;
            if (nivel[x] == nivel[y]) nivel[x]++;
            return true;
        };

        // Inicia um conjunto S vazio de arestas para a solução
        float somaPesos = 0.0f;
        int totalArestas = 0;

        // Enquanto Q não estiver vazio {
        for (auto &aresta : conjuntoArestas) {
            // Seleciona a menor aresta {u, v} do conjunto Q
            // Remove a aresta {u, v} do conjunto Q

            // Se u e v pertencem a árvores diferentes no conjunto F {
            if (unir(aresta.origem, aresta.destino)) {
                // Adiciona a aresta {u, v} para o conjunto S
                somaPesos += aresta.peso;
                totalArestas++;

                // Une o conjunto das árvores que contêm u e que contêm v no conjunto F
                if (totalArestas == qtdVertices - 1) break;
            }
        }
        // }

        auto tempo_fim = chrono::high_resolution_clock::now();
        auto duracao = chrono::duration_cast<chrono::milliseconds>(tempo_fim - tempo_inicio).count();

        cout << "KruskalMST - Soma das arestas: " << somaPesos
             << " | Tempo: " << duracao << " ms" << endl;
    }
};

// Função principal para testar as implementações dos grafos
int main()
{
    GrafoLista G(false, true); // Grafo direcionado e ponderado
    G.inserirVertice("1");
    G.inserirVertice("2");
    G.inserirVertice("3");
    G.inserirVertice("4");

    G.inserirAresta(0, 1, 2.0f);
    G.inserirAresta(0, 3, 6.0f);
    G.inserirAresta(1, 2, 3.0f);
    G.inserirAresta(1, 3, 8.0f);
    G.inserirAresta(1, 4, 5.0f);
    G.inserirAresta(2, 4, 7.0f);
    G.inserirAresta(3, 4, 9.0f);

    // Chama Prim
    G.primMST();

    // Chama Kruskal
    G.kruskalMST();
    return 0;
}