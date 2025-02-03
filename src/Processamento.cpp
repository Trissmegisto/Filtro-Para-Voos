#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Processamento.hpp"
#include <limits> // Usado apenas no arquivo.ignore() para delimitar onde é o /n. 
                 //Não simula uma estrutura de dados. "This header defines elements with the characteristics of arithmetic types"


// Função para trocar dois elementos em um array
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Função de comparação genérica para ordenação com base na ordem especificada
bool compararVoos(const Voo& a, const Voo& b, const string& ordem) {
    for (char criterio : ordem) {
        switch (criterio) {
            case 'p': // Preço
                if (a.preco != b.preco) return a.preco < b.preco;
                break;
            case 's': // Número de paradas
                if (a.numeroDeParadas != b.numeroDeParadas) return a.numeroDeParadas < b.numeroDeParadas;
                break;
            case 'd': // Duração
                if (a.duracaoTotal != b.duracaoTotal) return a.duracaoTotal < b.duracaoTotal;
                break;
            default:
                break;
        }
    }
    return false; // Se todos os critérios forem iguais
}

// Função para ordenar os resultados com base na ordem especificada
void ordenarResultados(NodeEstatica* syntaxTree, const string& ordem, const Voo voos[]) {
    for (int i = 0; i < syntaxTree->count - 1; i++) {
        for (int j = 0; j < syntaxTree->count - i - 1; j++) {
            if (!compararVoos(voos[syntaxTree->voos[j]], voos[syntaxTree->voos[j + 1]], ordem)) {
                // Troca os índices dos voos
                int temp = syntaxTree->voos[j];
                syntaxTree->voos[j] = syntaxTree->voos[j + 1];
                syntaxTree->voos[j + 1] = temp;
            }
        }
    }
}

// Função para limitar o número de voos no array de resultados
void limitarResultados(NodeEstatica* syntaxTree, int numVoos) {
    if (syntaxTree->count > numVoos) {
        syntaxTree->count = numVoos;  // Ajusta o número de resultados
    }
}

time_t converterParaTimeT(const std::string& dataHora) {
    std::tm tm = {};
    std::istringstream ss(dataHora);
    char delimiter;

    ss >> tm.tm_year >> delimiter >> tm.tm_mon >> delimiter >> tm.tm_mday
       >> delimiter >> tm.tm_hour >> delimiter >> tm.tm_min >> delimiter >> tm.tm_sec;

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    return mktime(&tm);
}

void lerArquivo(string nomeArquivo, AVLTree<string>& arvoreOrigem, AVLTree<string>& arvoreDestino, AVLTree<float>& arvorePreco, AVLTree<int>& arvoreAssentos,
            AVLTree<string>& arvoreDataHoraDePartida, AVLTree<string>& arvoreDataHoraDeChegada, AVLTree<int>& arvoreNumeroDeParadas, AVLTree<double>& arvoreDuracaoTotal) {
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        return;
    }

    int numeroDeVoos;
    arquivo >> numeroDeVoos;

    Voo voos[numeroDeVoos];

    for (int i = 0; i < numeroDeVoos; i++) {
        string origem, destino, dataHoraDePartida, dataHoraDeChegada;
        float preco;
        int assentos, numeroDeParadas;

        arquivo >> origem >> destino >> preco >> assentos >> dataHoraDePartida >> dataHoraDeChegada >> numeroDeParadas;

        time_t chegada = converterParaTimeT(dataHoraDeChegada);
        time_t partida = converterParaTimeT(dataHoraDePartida);

        double duracao = difftime(chegada, partida);

        voos[i].origem = origem;
        voos[i].destino = destino;
        voos[i].preco = preco;
        voos[i].assentos = assentos;
        voos[i].dataHoraDePartida = dataHoraDePartida;
        voos[i].dataHoraDeChegada = dataHoraDeChegada;
        voos[i].numeroDeParadas = numeroDeParadas;
        voos[i].duracaoTotal = duracao;

        // Insere nas árvores AVL
        arvoreOrigem.insert(origem, i);
        arvoreDestino.insert(destino, i);
        arvorePreco.insert(preco, i);
        arvoreAssentos.insert(assentos, i);
        arvoreDataHoraDePartida.insert(dataHoraDePartida, i);
        arvoreDataHoraDeChegada.insert(dataHoraDeChegada, i);
        arvoreNumeroDeParadas.insert(numeroDeParadas, i);
        arvoreDuracaoTotal.insert(duracao, i);
    }

    // Verificando as consultas. Exemplo: 
    // 2
    // 3 psd ((dur>=36537)&&(sea<=5)))
    // 7 sdp (((dur>=39370)&&(sea>=1)&&(dst==SFO)))

    int numeroDeConsultas;
    arquivo >> numeroDeConsultas;
    arquivo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < numeroDeConsultas; i++) {
        string linha;
        getline(arquivo, linha);  // Lê uma linha de consulta

        // Extrai as informações da consulta
        stringstream ss(linha);
        int numVoos;
        string ordem, expressao;

        ss >> numVoos >> ordem;  // Lê o número de voos e a ordem de ordenação
        getline(ss, expressao);  // Lê o restante da linha (expressão lógica)

        // Remove espaços em branco no início e no fim da expressão
        expressao.erase(0, expressao.find_first_not_of(' '));
        expressao.erase(expressao.find_last_not_of(' ') + 1);
        if (!expressao.empty() && expressao.back() == '\r') {
            expressao = expressao.substr(0, expressao.length() - 1);
        }

        // Constrói a árvore sintática a partir da expressão lógica
        NodeEstatica* syntaxTree = buildSyntaxTree(expressao);

        // Avalia a árvore sintática e processa a consulta
        evaluateSyntaxTree(syntaxTree, arvoreOrigem, arvoreDestino, arvorePreco, arvoreAssentos,
                        arvoreDataHoraDePartida, arvoreDataHoraDeChegada, arvoreNumeroDeParadas, arvoreDuracaoTotal);

                
        // Ordena os resultados com base na ordem especificada
        ordenarResultados(syntaxTree, ordem, voos);

        // Limita o número de voos ao valor de numVoos
        limitarResultados(syntaxTree, numVoos);

        // Exibe os resultados
        std::cout << linha << std::endl;
        for (int j = 0; j < syntaxTree->count; j++) {
            int indiceVoo = syntaxTree->voos[j];  // Índice do voo no array `voos`

            std::cout << voos[indiceVoo].origem << " " << voos[indiceVoo].destino << " " << voos[indiceVoo].preco << " " <<
            voos[indiceVoo].assentos << " " << voos[indiceVoo].dataHoraDePartida << " "<< voos[indiceVoo].dataHoraDeChegada << " " <<
            voos[indiceVoo].numeroDeParadas << std::endl;
        }

        // Libera a memória da árvore sintática
        delete syntaxTree;
    }
    arquivo.close();
}

