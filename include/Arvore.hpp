#ifndef ARVORE_H
#define ARVORE_H

#include <iostream>
#include <string>
using namespace std;

// Estrutura do nó da árvore AVL
template <typename T>
struct Node {
    T chave;                // Chave: pode ser string, float, int, etc.
    int indices[1100];       // Lista de índices dos voos
    int count;              // Quantidade de voos associados a esta chave
    Node* left;             // Filho esquerdo
    Node* right;            // Filho direito
    Node* parent;
    int height;             // Altura do nó

    // Construtor
    // Construtor do Node
    Node(T key) : chave(key), count(0), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
};

// Classe da árvore AVL
template <typename T>
class AVLTree {
private:
    Node<T>* root;

    // Função auxiliar para destruir recursivamente a árvore
    void destroyTree(Node<T>* node) {
        if (node != nullptr) {
            destroyTree(node->left);   // Destrói a subárvore esquerda
            destroyTree(node->right);  // Destrói a subárvore direita
            delete node;               // Deleta o nó atual
        }
    }

    // Função para calcular a altura de um nó
    int height(Node<T>* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // Função para calcular o fator de balanceamento de um nó
    int getBalance(Node<T>* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    // Função para realizar uma rotação à direita
    Node<T>* rightRotate(Node<T>* y) {
        Node<T>* x = y->left;
        Node<T>* T2 = x->right;

        // Realiza a rotação
        x->right = y;
        y->left = T2;

        // Atualiza os pais
        x->parent = y->parent;
        if (y->parent != nullptr) {
            if (y->parent->left == y)
                y->parent->left = x;
            else
                y->parent->right = x;
        }
        y->parent = x;
        if (T2 != nullptr) {
            T2->parent = y;
        }

        // Atualiza as alturas
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Função para realizar uma rotação à esquerda
    Node<T>* leftRotate(Node<T>* x) {
        Node<T>* y = x->right;
        Node<T>* T2 = y->left;

        // Realiza a rotação
        y->left = x;
        x->right = T2;

        // Atualiza os pais
        y->parent = x->parent;
        if (x->parent != nullptr) {
            if (x->parent->left == x)
                x->parent->left = y;
            else
                x->parent->right = y;
        }
        x->parent = y;
        if (T2 != nullptr) {
            T2->parent = x;
        }

        // Atualiza as alturas
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Função para inserir um nó na árvore AVL
    Node<T>* insert(Node<T>* node, T chave, int vooIndex) {
        // 1. Inserção normal de uma árvore binária de busca
        if (node == nullptr) {
            Node<T>* newNode = new Node<T>(chave);
            newNode->indices[newNode->count++] = vooIndex;
            newNode->parent = nullptr;  // Explicitamente inicializa o parent
            return newNode;
        }

        if (chave < node->chave) {
            node->left = insert(node->left, chave, vooIndex);
            node->left->parent = node;  // Atualiza o pai do nó inserido
        } else if (chave > node->chave) {
            node->right = insert(node->right, chave, vooIndex);
            node->right->parent = node;  // Atualiza o pai do nó inserido
        } else {
            // Se a chave já existe, apenas adiciona o índice do voo à lista
            node->indices[node->count++] = vooIndex;
            return node;
        }

        // 2. Atualiza a altura do nó atual
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Verifica o fator de balanceamento e balanceia a árvore se necessário
        int balance = getBalance(node);

        // Casos de desbalanceamento:
        // Esquerda-Esquerda (LL)
        if (balance > 1 && chave < node->left->chave) {
            return rightRotate(node);
        }
        // Direita-Direita (RR)
        if (balance < -1 && chave > node->right->chave) {
            return leftRotate(node);
        }
        // Esquerda-Direita (LR)
        if (balance > 1 && chave > node->left->chave) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Direita-Esquerda (RL)
        if (balance < -1 && chave < node->right->chave) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Função para buscar uma chave na árvore AVL
    Node<T>* search(Node<T>* node, T chave) {
        if (node == nullptr || node->chave == chave) {
            return node;
        }

        if (chave < node->chave) {
            return search(node->left, chave);
        } else {
            return search(node->right, chave);
        }
    }

    // Função para imprimir a árvore em ordem
    void inOrder(Node<T>* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << "Chave: " << node->chave << " (Voos: ";
            for (int i = 0; i < node->count; i++) {
                cout << node->indices[i] << " ";
            }
            cout << ")" << endl;
            inOrder(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    // Destrutor
    ~AVLTree() {
        destroyTree(root);
    }

    // Função pública para inserir uma chave
    void insert(T chave, int vooIndex) {
        root = insert(root, chave, vooIndex);
    }

    // Função pública para buscar uma chave
    Node<T>* search(T chave) {
        return search(root, chave);
    }

    // Função pública para imprimir a árvore em ordem
    void inOrder() {
        inOrder(root);
    }

    Node<T>* searchGreaterOrEqual(T chave) {
        Node<T>* current = root;
        Node<T>* result = nullptr;

        while (current != nullptr) {
            if (current->chave >= chave) {
                result = current;  // Armazena o nó atual como candidato
                current = current->left;  // Continua buscando à esquerda para encontrar um nó menor, mas ainda >= chave
            } else {
                current = current->right;  // Busca à direita para encontrar um nó maior
            }
        }

        return result;  // Retorna o nó encontrado (ou nullptr se não houver)
    }

    
    Node<T>* searchLessOrEqual(T chave) {
        Node<T>* current = root;
        Node<T>* result = nullptr;

        while (current != nullptr) {
            if (current->chave <= chave) {
                result = current;
                current = current->right;
            } else {
                current = current->left;
            }
        }

        return result;
    }

    Node<T>* getNext(Node<T>* node) {
        if (node == nullptr) return nullptr;

        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    Node<T>* getPrevious(Node<T>* node) {
        if (node == nullptr) return nullptr;

        // Se houver filho esquerdo, o anterior é o maior nó da subárvore esquerda
        if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr) {
                node = node->right;
            }
            return node;
        }

        // Caso contrário, o anterior é o primeiro ancestral maior que o nó atual
        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    void getVoosPorDestino(AVLTree<string>& arvoreDestino, const string& destino, int result[], int& resultSize) {
        resultSize = 0;
        Node<string>* node = arvoreDestino.search(destino);  // Busca o nó com o destino especificado
        if (node != nullptr) {
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];  // Copia os índices dos voos
            }
        }
    }

    void getVoosPorOrigem(AVLTree<string>& arvoreOrigem, const string& origem, int result[], int& resultSize) {
        resultSize = 0;
        Node<string>* node = arvoreOrigem.search(origem);  // Busca o nó com a origem especificada
        if (node != nullptr) {
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];  // Copia os índices dos voos
            }
        }
    }

    void getVoosPorDuracao(AVLTree<double>& arvoreDuracao, double valor, const string& operador, int result[], int& resultSize) {
        resultSize = 0;
        Node<double>* node = nullptr;

        if (operador == "<=") {
            node = arvoreDuracao.searchLessOrEqual(valor);  // Busca voos com duração <= valor
        } else if (operador == ">=") {
            node = arvoreDuracao.searchGreaterOrEqual(valor);  // Busca voos com duração >= valor
        } else if (operador == "==") {
            node = arvoreDuracao.search(valor);  // Busca voos com duração == valor
        }

        while (node != nullptr) {
            // Adiciona os índices dos voos ao resultado
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];
            }

            // Avança para o próximo nó
            if (operador == "<=") {
                node = arvoreDuracao.getPrevious(node);  // Usa getPrevious para ordem decrescente
            } else {
                node = arvoreDuracao.getNext(node);  // Usa getNext para ordem crescente
            }
        }
    }

    void getVoosPorAssentos(AVLTree<int>& arvoreAssentos, int valor, const string& operador, int result[], int& resultSize) {
        resultSize = 0;
        Node<int>* node = nullptr;

        if (operador == "<=") {
            node = arvoreAssentos.searchLessOrEqual(valor);  // Busca voos com assentos <= valor
        } else if (operador == ">=") {
            node = arvoreAssentos.searchGreaterOrEqual(valor);  // Busca voos com assentos >= valor
        } else if (operador == "==") {
            node = arvoreAssentos.search(valor);  // Busca voos com assentos == valor
        }


        // Percorre os nós enquanto atenderem à condição
        while (node != nullptr) {
            // Adiciona os índices dos voos ao resultado
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];
            }

            // Avança para o próximo nó
            if (operador == "<=") {
                node = arvoreAssentos.getPrevious(node);  // Usa getPrevious para ordem decrescente
            } else {
                node = arvoreAssentos.getNext(node);  // Usa getNext para ordem crescente
            }
        }
    }

    void getVoosPorPreco(AVLTree<float>& arvorePreco, float valor, const string& operador, int result[], int& resultSize) {
        resultSize = 0;
        Node<float>* node = nullptr;

        if (operador == "<=") {
            node = arvorePreco.searchLessOrEqual(valor);  // Busca voos com preço <= valor
        } else if (operador == ">=") {
            node = arvorePreco.searchGreaterOrEqual(valor);  // Busca voos com preço >= valor
        } else if (operador == "==") {
            node = arvorePreco.search(valor);  // Busca voos com preço == valor
        }

        while (node != nullptr) {
            // Adiciona os índices dos voos ao resultado
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];
            }

            // Avança para o próximo nó
            if (operador == "<=") {
                node = arvorePreco.getPrevious(node);  // Usa getPrevious para ordem decrescente
            } else {
                node = arvorePreco.getNext(node);  // Usa getNext para ordem crescente
            }
        }
    }

    void getVoosPorParadas(AVLTree<int>& arvoreParadas, int valor, const string& operador, int result[], int& resultSize) {
        resultSize = 0;
        Node<int>* node = nullptr;  
        if (operador == "<=") {
            node = arvoreParadas.searchLessOrEqual(valor);  // Busca voos com preço <= valor
        } else if (operador == ">=") {
            node = arvoreParadas.searchGreaterOrEqual(valor);  // Busca voos com preço >= valor
        } else if (operador == "==") {
            node = arvoreParadas.search(valor);  // Busca voos com preço == valor
        }
        
        while (node != nullptr) {
            // Adiciona os índices dos voos ao resultado
            for (int i = 0; i < node->count; i++) {
                result[resultSize++] = node->indices[i];
            }

            // Avança para o próximo nó
            if (operador == "<=") {
                node = arvoreParadas.getPrevious(node);  // Usa getPrevious para ordem decrescente
            } else {
                node = arvoreParadas.getNext(node);  // Usa getNext para ordem crescente
            }
        }
    }
};

#endif