#ifndef ARVORECONSULTA_H
#define ARVORECONSULTA_H

#include "Arvore.hpp"
using namespace std;

struct NodeEstatica {
    string value;
    NodeEstatica* left;
    NodeEstatica* right;
    int voos[1100];
    int count;

    NodeEstatica(string val) : value(val), left(nullptr), right(nullptr), count(0) {}
};

// Declarações das funções
int findOperatorIndex(const string& expr);
NodeEstatica* buildSyntaxTree(const string& expr);
void intersectArrays(const int arr1[], int size1, const int arr2[], int size2, int result[], int& resultSize);
void evaluateSyntaxTree(NodeEstatica* root, AVLTree<string>& arvoreOrigem, AVLTree<string>& arvoreDestino, 
    AVLTree<float>& arvorePreco, AVLTree<int>& arvoreAssentos, AVLTree<string>& arvoreDataHoraDePartida, 
    AVLTree<string>& arvoreDataHoraDeChegada, AVLTree<int>& arvoreNumeroDeParadas, AVLTree<double>& arvoreDuracaoTotal);

#endif