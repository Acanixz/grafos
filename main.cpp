// Trabalho M1.1
// Grafos 2025 - 1
// Alunos: 
//      Hérick Vitor Vieira Bittencourt
//      Rodrigo Rey de Oliveira Carrard
//      Paola Piran Zanella

#include <iostream>
using namespace std;

class Grafos {
    // Referencia:
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
};

int main(){
    return 0;
}