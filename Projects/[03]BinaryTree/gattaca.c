#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct node *pNode;

struct node {
    int sumLeft, height;
    char *string;
    pNode left, right;
    bool isLeaf;
};

// Define o maior valor entre dois inteiros
int max(int a, int b) {
    if (a >= b) {
        return a;
    } else if (a < b) {
        return b;
    }
    return 0;
}

// Divide a string adequadamente entre as folhas
pNode buildTree(char *currentSequence, int currentSize, int limit) {
    int newSize;
    char *Lsequence, *Rsequence;
    pNode node = malloc(sizeof(struct node));

    // Verifica se o tamanho da sequência atual é maior do que o limite por folha
    if (currentSize > limit) {

        // Trata as strings a serem passadas para a esquerda e para a direita
        newSize = currentSize / 2;
        Lsequence = malloc((newSize + 2) * sizeof(char));
        Rsequence = malloc((currentSize - newSize + 2) * sizeof(char));
        Lsequence = strncpy(Lsequence, currentSequence, newSize);
        Lsequence[newSize] = '\0';
        Rsequence = strncpy(Rsequence, currentSequence + newSize, currentSize - newSize);
        Rsequence[currentSize - newSize] = '\0';

        // Constrói as subárvores
        node->left = buildTree(Lsequence, newSize, limit);
        free(Lsequence);
        node->right = buildTree(Rsequence, currentSize - newSize, limit);
        free(Rsequence);

        // Atualiza os atributos do nó atual
        node->sumLeft = newSize;
        node->isLeaf = false;
        node->string = NULL;
        node->height = 1 + max(node->left->height, node->right->height);
    
    // Se não, atribui-se a sequence aos nós atuais (folhas)
    } else {
        node->string = malloc((limit + 2) * sizeof(char));
        strcpy(node->string, currentSequence);
        node->left = node->right = NULL;
        node->isLeaf = true;
        node->height = 1;
        node->sumLeft = currentSize;
    }

    return node;
}

// Libera a memória da árvore
void destroyTree(pNode node) {
    if (node->isLeaf == true) {
        free(node->string);
    } else {
        destroyTree(node->left);
        destroyTree(node->right);
    }
    free(node);
}

// Imprime as strings em cada folha
void printall(pNode node) {
    if (node->isLeaf == true) {
        printf("%s", node->string);
    } else {
        printall(node->left);
        printall(node->right);
    }
}

// Imprime o caracter na posição i
void printij(pNode node, int i) {
    if (node->isLeaf == true) {
        printf("%c", node->string[i]);
    } else {
        if (i < node->sumLeft) {
            printij(node->left, i);
        } else if (i >= node->sumLeft) {
            printij(node->right, i - node->sumLeft);
        } 
    }
}

// Atualiza a contagem de cada letra conforme passa 1 vez por cada nó
void counting(pNode node, int *count) {
    if (node->isLeaf == true) {
        for (int i = 0; i < node->sumLeft; i++) {
            if (node->string[i] == 'A')
                count[0]++;
            else if (node->string[i] == 'C')
                count[1]++;
            else if (node->string[i] == 'G')
                count[2]++;
            else if (node->string[i] == 'T')
                count[3]++;
        }
    } else {
        counting(node->left, count);
        counting(node->right, count);
    }
}

// Insere um caracter em uma posição pos do DNA
pNode insert(pNode node, pNode parent, int pos, char c, int limit) {
    int i;

    // Busca até encontrar a folha que contém a posição pos, enquanto atualiza os atributos de cada nó
    if (node->left != NULL) {
        if (pos < node->sumLeft) {
            node->left = insert(node->left, parent, pos, c, limit);
            node->sumLeft++;
        } else if (pos >= node->sumLeft) {
            node->right = insert(node->right, parent, pos - node->sumLeft, c, limit);
        }
        node->height = 1 + max(node->left->height, node->right->height);

    } else {
        // Move os elementos da string do nó atual para a direita para inserir o novo caracter
        for (i = node->sumLeft - 1; i >= 0 && i >= pos; i--) {
            node->string[i + 1] = node->string[i];
        }
        node->string[i + 1] = c;
        node->sumLeft++;
        node->string[node->sumLeft] = '\0';

        // Se a string estourar o limite de caracteres por nó, cria-se dois novos nós
        if (node->sumLeft > limit) {
            char *stringCopy;
            stringCopy = malloc((node->sumLeft + 2) * sizeof(char));
            strcpy(stringCopy, node->string);

            pNode newNode = buildTree(stringCopy, strlen(stringCopy), limit);
            free(stringCopy);
            if (parent->left == node)
                parent->left = newNode;
            if (parent->right == node)
                parent->right = newNode;

            free(node->string);
            free(node);
            return newNode;
        }
    }
    return node;
}

// Junta todas as strings abaixo do nó inicial em uma única string
char *nodeToSequence(pNode node, char *subSequence) {
    if (node->left == NULL && node->right == NULL) {
        if (subSequence[0] == '\0') {
            strcpy(subSequence, node->string);
        } else {
            strcat(subSequence, node->string);
        }
    } else {
        nodeToSequence(node->left, subSequence);
        nodeToSequence(node->right, subSequence);
    }
    return subSequence;
}

// Remove o caracter de uma posição pos
pNode removePos(pNode node, int pos, int limit) {

    // Busca até encontrar a folha que contém a posição pos, enquanto atualiza os atributos de cada nó
    if (node->left != NULL && node->right != NULL) {
        if (pos < node->sumLeft) {
            node->left = removePos(node->left, pos, limit);
            // Se o nó à esquerda já foi removido, seu irmão vira o novo filho de seu avô
            if (node->left == NULL) {
                pNode otherKid = node->right;
                free(node);
                return otherKid;
            }
            node->sumLeft--;
        } else {
            node->right = removePos(node->right, pos - node->sumLeft, limit);
            // Se o nó à direita já foi removido, seu irmão vira o novo filho de seu avô
            if (node->right == NULL) {
                pNode otherKid = node->left;
                free(node);
                return otherKid;
            }
        }
        if (node->left != NULL && node->right != NULL)
            node->height = 1 + max(node->left->height, node->right->height);
            
    } else {

        // Move os elementos da string do nó atual para a esquerda sobre a posição removida, caso este não seja o último elemento
        node->sumLeft = strlen(node->string);
        if ((node->sumLeft - 1) > 0) {
            for (; pos < node->sumLeft - 1; pos++)
                node->string[pos] = node->string[pos + 1];
            node->sumLeft--;
            node->string[node->sumLeft] = '\0';
        // Se o elemento for o último da string
        } else {
            node->sumLeft = 0;
        }

        // Se a folha ficar vazia, ela é removida da árvore
        if (node->sumLeft == 0) {
            free(node->string);
            free(node);
            return NULL;
        }
    }
    return node;
}

// Mantém a árvore balanceada
pNode balance(pNode node, int pos, int size, int limit) {

    // Verificando cada nó até a posição da última alteração para ver se algum está desbalanceado
    if (node->left != NULL && node->right != NULL) {
        // Se estiver desbalanceado, salva a subsequência a partir daquele nó, destrói essa subávore e recria essa subárvore
        if ((node->left->height - node->right->height) > 4 || (node->right->height - node->left->height) > 4) {
            char *sequence;
            sequence = malloc((node->sumLeft + 3) * 10000 * sizeof(char));
            sequence[0] = '\0';
            sequence = nodeToSequence(node, sequence);
            destroyTree(node);
            pNode newNode;
            newNode = buildTree(sequence, strlen(sequence), limit);
            free(sequence);
            return newNode;
        } else {
            if (pos < node->sumLeft) {
                node->left = balance(node->left, pos, node->sumLeft, limit);
            } else if (pos >= node->sumLeft) {
                node->right = balance(node->right, pos - node->sumLeft, node->sumLeft, limit);
            }
        }
    }
    return node;
}

// Chama a função correta de acordo com a entrada
pNode commandCenter(char *command, pNode root, int limit) {
    int i, j, pos;
    char c;

    if (strcmp(command, "PRINTALL") == 0) {
        printf("DNA inteiro: ");
        printall(root);
        printf("\n");
    } else if (strcmp(command, "PRINT") == 0) {
        scanf(" %d %d", &i, &j);
        printf("Trecho %d-%d: ", i, j);
        for(int k = i; k < j; k++)
            printij(root, k);
        printf("\n");
    } else if (strcmp(command, "INSERT") == 0) {
        scanf(" %d %c", &pos, &c);
        root = insert(root, root, pos, c, limit);
        root = balance(root, pos, root->sumLeft, limit);
    } else if (strcmp(command, "REMOVE") == 0) {
        scanf(" %d", &pos);
        root = removePos(root, pos, limit);
        root = balance(root, pos, root->sumLeft, limit);
    }

    return root;
}

// Executa o código
int main() {
    int operations, sequenceSize, limit, A, C, G, T;
    A = C = G = T = 0;
    int count[] = {A, C, G, T};
    char *sequence, *command;
    pNode root;

    // Lê as entradas iniciais e aloca a string dinamicamente
    scanf(" %d %d %d", &operations, &sequenceSize, &limit);
    sequence = malloc((sequenceSize + 2) * sizeof(char));
    scanf(" %s", sequence);

    // Constrói a árvore inicial
    root = buildTree(sequence, sequenceSize, limit);

    // Lê as operações 
    for (int i = 0; i < operations; i++) {
        command = malloc(30 * sizeof(char));
        scanf(" %s", command);
        root = commandCenter(command, root, limit);

        free(command);
    }

    // Imprime e realiza a contagem final
    printf("Contagem final: ");
    counting(root, count);
    printf("A:%d T:%d C:%d G:%d\n", count[0], count[3], count[1], count[2]);

    free(sequence);
    destroyTree(root);
    return 0;
}