// Trabalho M1.1
// Grafos 2025 - 1
// Alunos: 
//      Hérick Vitor Vieira Bittencourt
//      Rodrigo Rey de Oliveira Carrard
//      Paola Piran Zanella

#include <iostream>
#include <vector>
using namespace std;

class Grafos {
    // https://slideplayer.com.br/slide/1268779/3/images/2/Conceitos+Grafo%3A+Conjunto+de+V%C3%A9rtices+e+Arestas.jpg

    public:
        // As arestas possuem cardinalidade, havendo casos que não pode voltar o caminho
        bool direcionado;
        // Cada vértice possui um peso, dois caminhos iguais podem levar +/- para completar
        bool ponderado;

        // Construtor
        Grafos(bool Direcionado, bool Ponderado){
            direcionado = Direcionado;
            ponderado = Ponderado;
        }

        //// MÉTODOS ////

        // Adiciona um vértice sem nenhuma aresta associada a ele, pode parecer igual em ambos os casos, mas não é.
        // Precisamos adicionar esse vértice no vetor de vértices e também alocar seu espaço para as arestas.
        virtual bool inserirVertice(string label);

        // Remove um vértice do grafo, elimina a linha e coluna dele da matriz e a referência dele da lista, 
        // junto com todas as arestas que chegam e saem dele.
        virtual bool removerVertice(int indice);

        // Funções básicas para retornar o nome de um vértice.
        virtual string labelVertice(int indice);

        // Imprimir o grafo no console, 
        // tentem deixar próximo da representação dos slides (não precisa da grade da matriz).
        virtual void imprimeGrafo();

        // Essa operação deve ter um cuidado especial, ela deve ser executada levando em conta o tipo do grafo. 
        // No caso de um grafo ponderado o peso deve ser aplicado e no caso de um grafo direcionado, 
        // uma ligação de volta deve ser adicionada;
        virtual bool inserirAresta(int origem, int destino, int peso = 1);

        // Remove uma aresta entre dois vértices no grafo, 
        // lembrando que no grafo não direcionado deve ser removida a aresta de retorno também;
        virtual bool removerAresta(int origem, int destino);

        // Verifica a existência de uma aresta, aqui vemos uma diferença grande entre matriz e lista.
        virtual bool existeAresta(int origem, int destino);

        // Retorne o peso de uma aresta, aqui vemos uma diferença grande entre matriz e lista.
        virtual float pesoAresta(int origem, int destino);

        // Função para retorno dos vizinhos de um vértice, 
        // necessária pois não teremos acesso a estrutura das arestas para os próximos algoritmos.
        virtual vector<int> retornarVizinhos(int vertice);
};

// Usa uma matriz de adjacência como representação do grafo.
// https://miro.medium.com/v2/resize:fit:960/1*gny0Hz4hd90zDeCyY0s72A.png
class GrafoMatriz: public Grafos {

};

// Usa uma lista de adjacência como representação do grafo 
// e tem uma estrutura Aresta como auxilio
// https://upload.wikimedia.org/wikipedia/commons/6/61/Grafos_lista_de_adjac%C3%AAncia.png
class GrafoLista: public Grafos {

};

int main(){
    return 0;
}