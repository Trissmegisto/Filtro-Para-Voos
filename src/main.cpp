#include "Processamento.hpp"
#include <chrono>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo_de_entrada>" << endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Cria uma árvore AVL para cada atributo
    AVLTree<string> arvoreOrigem;
    AVLTree<string> arvoreDestino;
    AVLTree<float> arvorePreco;
    AVLTree<int> arvoreAssentos;
    AVLTree<string> arvoreDataHoraDePartida;
    AVLTree<string> arvoreDataHoraDeChegada;
    AVLTree<int> arvoreNumeroDeParadas;
    AVLTree<double> arvoreDuracaoTotal;

    // Ler o arquivo e inserir nas árvores
    lerArquivo(argv[1], arvoreOrigem, arvoreDestino, arvorePreco, arvoreAssentos, 
               arvoreDataHoraDePartida, arvoreDataHoraDeChegada, arvoreNumeroDeParadas, arvoreDuracaoTotal);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Tempo de execução: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}