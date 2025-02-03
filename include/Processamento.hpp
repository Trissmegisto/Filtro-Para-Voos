#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include "ArvoreConsulta.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

struct Voo {
    string origem;
    string destino;
    float preco;
    int assentos;
    string dataHoraDePartida;
    string dataHoraDeChegada;
    int numeroDeParadas;
    double duracaoTotal;
};

// Declarações das funções
void swap(int& a, int& b);
bool compararVoos(const Voo& a, const Voo& b, const string& ordem);
void ordenarResultados(NodeEstatica* syntaxTree, const string& ordem, const Voo voos[]);
void limitarResultados(NodeEstatica* syntaxTree, int numVoos);
time_t converterParaTimeT(const std::string& dataHora);
void lerArquivo(string nomeArquivo, AVLTree<string>& arvoreOrigem, AVLTree<string>& arvoreDestino,
                AVLTree<float>& arvorePreco, AVLTree<int>& arvoreAssentos,
                AVLTree<string>& arvoreDataHoraDePartida, AVLTree<string>& arvoreDataHoraDeChegada,
                AVLTree<int>& arvoreNumeroDeParadas, AVLTree<double>& arvoreDuracaoTotal);

#endif