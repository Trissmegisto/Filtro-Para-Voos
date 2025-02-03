#include "ArvoreConsulta.hpp"

int findOperatorIndex(const string& expr) {
    int parenCount = 0;
    for (size_t i = 0; i < expr.length(); i++) {
        char ch = expr[i];
        if (ch == '(') parenCount++;
        else if (ch == ')') parenCount--;
        else if (parenCount == 0 && (expr.substr(i, 2) == "&&" || expr.substr(i, 2) == "||")) {
            return i;
        }
    }
    return -1;
}

NodeEstatica* buildSyntaxTree(const string& expr) {
    string expression = expr;  // Remove espaços em branco

    // Remove os 2 parênteses externos
    if (expression.length() >= 4 && 
        expression[0] == '(' && 
        expression[1] == '(' && 
        expression[expression.length() - 1] == ')' && 
        expression[expression.length() - 2] == ')') {
        expression = expression.substr(2, expression.length() - 4);
    }

    // Caso base: se a expressão é um predicado simples (ex: "dst==IAD")
    if (expression.find("&&") == string::npos && expression.find("||") == string::npos) {
        string leftExpr = expression.substr(1, 3);  // Pega de 1 até o operador

        string rightExpr = expression.substr(6, expression.find(')')); // Problema aqui, no operador '=='
        rightExpr = rightExpr.substr(0, rightExpr.find(')')); // Remove o ')'

        string op = expression.substr(4, 2);;  // Operador lógico (==, >=, <=)

        NodeEstatica* node = new NodeEstatica(op);  // Nó folha com a expressão completa. O parâmetro deve ser o operador lógico dessa expressão, ex: "<="
        node->left = new NodeEstatica(leftExpr);  // Filho esquerdo é a chave (ex: "dst")
        node->right = new NodeEstatica(rightExpr);  // Filho direito é o valor (ex: "IAD")

        return node;
    }

    // Encontra o índice do operador lógico mais externo
    int opIndex = findOperatorIndex(expression);
    if (opIndex == -1) {
        return nullptr;  // Expressão inválida
    }

    // Cria o nó raiz com o operador lógico
    string op = expression.substr(opIndex, 2);  // Operador lógico (&& ou ||)
    NodeEstatica* root = new NodeEstatica(op);

    // Sempre que root for um operador lógico, left e right serão expressões e executarão isso:
    // Divide a expressão em subexpressões esquerda e direita
    if (root->value == "&&" || root->value == "||" || root->value == "!") {
        string leftExpr = expression.substr(0, opIndex);  // Pega do 0 até o operador
        string rightExpr = expression.substr(opIndex + 2);  // Pega de dois caracteres após o operador até o final

        // Constrói as subárvores esquerda e direita
        root->left = buildSyntaxTree(leftExpr); // O root->value deve ser o valor deve ser o operador lógico dessa expressão
        root->right = buildSyntaxTree(rightExpr);
    }


    return root;
}

// Função para calcular a interseção das arrays estáticas. É necessário pra não utilizar vector
void intersectArrays(const int arr1[], int size1, const int arr2[], int size2, int result[], int& resultSize) {
    resultSize = 0;
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (arr1[i] == arr2[j]) {
                result[resultSize++] = arr1[i];
                break;
            }
        }
    }
}

void evaluateSyntaxTree(NodeEstatica* root, AVLTree<string>& arvoreOrigem, AVLTree<string>& arvoreDestino, AVLTree<float>& arvorePreco, AVLTree<int>& arvoreAssentos,
            AVLTree<string>& arvoreDataHoraDePartida, AVLTree<string>& arvoreDataHoraDeChegada, AVLTree<int>& arvoreNumeroDeParadas, AVLTree<double>& arvoreDuracaoTotal) {
    if (root == nullptr) return;

    
    // Avalia os filhos primeiro (pós-ordem)
    evaluateSyntaxTree(root->left, arvoreOrigem, arvoreDestino, arvorePreco, arvoreAssentos, arvoreDataHoraDePartida, arvoreDataHoraDeChegada, arvoreNumeroDeParadas, arvoreDuracaoTotal);
    evaluateSyntaxTree(root->right, arvoreOrigem, arvoreDestino, arvorePreco, arvoreAssentos, arvoreDataHoraDePartida, arvoreDataHoraDeChegada, arvoreNumeroDeParadas, arvoreDuracaoTotal);

    // Avalia o nó atual
    if (root->value == "&&") {
        // Interseção das listas de voos dos filhos
        int result[100];
        int resultSize = 0;
        intersectArrays(root->left->voos, root->left->count, root->right->voos, root->right->count, result, resultSize);

        // Copia o resultado para o nó atual
        root->count = 0;
        for (int i = 0; i < resultSize; i++) {
            root->voos[root->count++] = result[i];
        }
    } else if (root->value == ">=" || root->value == "<=" || root->value == "==") {
        // Acessa o índice correspondente e recupera os voos que satisfazem o predicado
        string var = root->left->value;
        string operador = root->value;
        string val = root->right->value;

        if (var == "dur") {
            double durValue = stod(val);
            arvoreDuracaoTotal.getVoosPorDuracao(arvoreDuracaoTotal, durValue, operador, root->voos, root->count);
            // Acessa o índice de duração e recupera os voos que satisfazem o predicado
            // (Implementação depende da estrutura do índice)
        } else if (var == "sea") {
            int seaValue = stoi(val);
            arvoreAssentos.getVoosPorAssentos(arvoreAssentos, seaValue, operador, root->voos, root->count);
            // Acessa o índice de assentos e recupera os voos que satisfazem o predicado
        } else if (var == "prc") {
            float prcValue = stof(val);
            arvorePreco.getVoosPorPreco(arvorePreco, prcValue, operador, root->voos, root->count);
            // Acessa o índice do preço e recupera os voos que satisfazem o predicado
        } else if (var == "sto") {
            int stoValue = stoi(val);
            arvoreNumeroDeParadas.getVoosPorParadas(arvoreNumeroDeParadas, stoValue, operador, root->voos, root->count);
            // Acessa o índice de paradas e recupera os voos que satisfazem o predicado
        } else if (var == "dst") {
            string dstValue = val;
            arvoreDestino.getVoosPorDestino(arvoreDestino, dstValue, root->voos, root->count);
            // Acessa o índice de destino e recupera os voos que satisfazem o predicado
        } else if (var == "org") {
            string orgValue = val;
            arvoreOrigem.getVoosPorOrigem(arvoreOrigem, orgValue, root->voos, root->count);
            // Acessa o índice de origem e recupera os voos que satisfazem o predicado
        }
    }
}