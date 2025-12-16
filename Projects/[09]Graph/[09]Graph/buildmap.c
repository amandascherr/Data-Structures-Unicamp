#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define lineMAX 16801
#define verticesMAX 5000
#define edgesMAX 20000
#define nameMAX 16

typedef struct vector *pVector;
typedef struct stringVector *pStringVector;
typedef struct node *pNode;
typedef struct edge *pEdge;
typedef struct graph *pGraph;

struct vector {
    pNode *nodes;
    int itemCount;
};

struct stringVector {
    char **names;
    int itemCount;
};

struct node {
    char *name;
    int index, edgeCount, visitStatus;
    pEdge *edges;
};

struct edge {
    pNode destination;
};

struct graph {
    pNode *adj;
    int nodeCount, edgeCount;
};



/* ===== GERENCIAMENTO DE MEMÓRIA ===== */
// Inicializa o grafo
pGraph initGraph() {   
    pGraph graph = malloc(sizeof(struct graph));
    graph->nodeCount = 0;

    graph->adj = malloc(verticesMAX * sizeof(pNode));
    for (int i = 0; i < verticesMAX; i++)
        graph->adj[i] = NULL;

    return graph;
}

// Inicializa um vetor
pVector initVector() {
    pVector vector = malloc(verticesMAX * sizeof(pNode));
    vector->nodes = malloc(verticesMAX * sizeof(pNode));
    vector->itemCount = 0;

    return vector;
}

// Libera as arestas de um nó
void freeEdges(pNode thisNode) {
    for (int i = 0; i < thisNode->edgeCount; i++) {
        free(thisNode->edges[i]);
    }
    free(thisNode->edges);
}

// Libera o grafo
void freeGraph(pGraph graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        freeEdges(graph->adj[i]);
        free(graph->adj[i]->name);
        free(graph->adj[i]);
    }
    free(graph->adj);
    free(graph);
}

// Libera um vetor
void freeVector(pVector vector) {
    free(vector->nodes);
    free(vector);
}

// Libera um vetor de strings
void freeStringVector(pStringVector vector) {
    for (int i = 0; i < vector->itemCount; i++) {
        free(vector->names[i]);
    }
    free(vector->names);
    free(vector);
}



/* ===== PRINTS ===== */
// Imprime um vetor (developer's tool)
void printStack(pVector stack) {
    for (int i = 0; i < stack->itemCount; i++) {
        printf("%s ", stack->nodes[i]->name);
    }
    printf("\n");
}

// Imprime o grafo em forma de lista de adjacência (developer's tool)
void printGraph(pGraph graph) {
    printf("\n");
    printf("======= GRAFO ATUAL =======\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        pNode this = graph->adj[i];
        printf("%s: ", this->name);
        for (int j = 0; j < this->edgeCount; j++) {
            printf("%s -> ", this->edges[j]->destination->name);
        }
        printf("\n");
    }
    printf("\n");
}

// Imprime a ordem topológica
void printTopologicalOrder(pVector sorted) {
    for (int i = 0; i < sorted->itemCount; i++) {
        printf("SEQ - %d: %s\n", i + 1, sorted->nodes[i]->name);
    }
}

// Imprime o ciclo
void printCycle(pGraph graph, pVector cycle) {
    printf("Erro: dependências circulares entre os arquivos necessários para construir '%s'\n", graph->adj[0]->name);

    // Imprime o elemento que aparece duas vezes em cycle e seus intermediários, da direita para a esquerda
    printf("Ciclo: %s -> ", cycle->nodes[0]->name);
    for (int i = cycle->itemCount - 2; i > 0; i--) {
        if (strcmp(cycle->nodes[i]->name, cycle->nodes[0]->name) == 0) {
            break;
        }
        printf("%s -> ", cycle->nodes[i]->name);
    }
    printf("%s\n", cycle->nodes[0]->name);
}

// Imprime a distância até o alvo
void printDistance(pGraph graph, pVector distances) {
    int distance = 0;
    for (int i = 0; i < distances->itemCount; i++) {
        if (distances->nodes[i] == NULL) {
            distance++;
            continue;
        }
        printf("Distância até %s - %d: %s\n", graph->adj[0]->name, distance, distances->nodes[i]->name);
    }
}



/* ===== MANIPULAÇÃO DE VETORES ===== */
// Insere uma string em um vetor ordenado lexicograficamente
pStringVector insertionSort(pStringVector vector, char *thisName) {
    int i;
    for (i = vector->itemCount - 1; i >= 0 && strcmp(vector->names[i], thisName) > 0; i--)
        vector->names[i + 1] = vector->names[i];
    vector->names[i + 1] = thisName;
    vector->itemCount++;

    return vector;
}

// Ordena um vetor lexicograficamente
pVector bubbleSort(pVector queue) {
    int loops = 0;
    while (loops < queue->itemCount - 1) {
        for (int i = 0; i < queue->itemCount - 1; i++) {
            pNode thisNode = queue->nodes[i];
            pNode next = queue->nodes[i + 1];
    
            if (strcmp(thisNode->name, next->name) > 0) {
                queue->nodes[i] = next;
                queue->nodes[i + 1] = thisNode;
                loops = 0;
            } else {
                loops++;
            }
        }
    }

    return queue;

}

// Remove um elemento de uma posição específica do vetor
pVector removeIndex(pVector vector, int position) {
    for(; position < vector->itemCount - 1; position ++)
        vector->nodes[position] = vector->nodes[position + 1];
    vector->itemCount--;

    return vector;
}

// Adiciona um elemento ao início de um vetor
pVector addForward(pVector vector, pNode element) {
    for (int i = vector->itemCount - 1; i >= 0; i--)
        vector->nodes[i + 1] = vector->nodes[i];
    vector->nodes[0] = element;
    vector->itemCount++;

    return vector;
}

// Insere o vetor second em uma posição específica do vetor first
pVector fuseVectors(pVector first, pVector second, int position) {
    int i;
    // Desloca o primeiro vetor para a direita o suficiente
    for (i = first->itemCount - 1; i >= position; i--)
        first->nodes[i + second->itemCount] = first->nodes[i];
    first->itemCount = first->itemCount + second->itemCount;
    i++;
    // Copia o segundo vetor no espaço que sobrou
    for (int j = 0; j < second->itemCount; j++) {
        first->nodes[i] = second->nodes[j];
        i++;
    }

    return first;
}



/* ===== MANIPULAÇÃO DO GRAFO ===== */
// Retorna o índice de uma string na lista de adjacência do grafo, ou -1 caso ainda não exista
int stringToIndex(pGraph graph, char *name) {
    int index = -1;
    for (int i = 0; i <= graph->nodeCount; i++) {
        if (graph->adj[i] == NULL) {
            continue;
        }
        if (strcmp(name, graph->adj[i]->name) == 0) {
            index = i;
            break;
        }
    }

    return index;
}

// Cria um novo vértice para o grafo
pNode createNode(pGraph graph, char *name) {
    int index = stringToIndex(graph, name);
    if (index != -1) {
        return graph->adj[index];
    }

    pNode newNode = malloc(sizeof(struct node));
    newNode->name = malloc(nameMAX * sizeof(char));
    newNode->edges = malloc(edgesMAX * sizeof(pEdge));
    newNode->index = graph->nodeCount;
    newNode->visitStatus = 0;
    strcpy(newNode->name, name);
    newNode->edgeCount = 0;

    graph->adj[graph->nodeCount] = newNode;
    graph->nodeCount++;

    return newNode;
}

// Cria uma aresta entre um nó pai e um nó filho
void insertEdge(pGraph graph, pNode parent, char *childName) {
    pNode child = createNode(graph, childName);
    pEdge newEdge = malloc(sizeof(struct edge));
    newEdge->destination = child;

    parent->edges[parent->edgeCount] = newEdge;
    parent->edgeCount++;
}



/* ===== DFS ===== */
/* visitStatus: -1 = Visitando; 0 = Não visitado; 1 = Visitado; 2 = Alerta de ciclo*/
// Adiciona o primeiro vizinho não visitado de um nó ao início do vetor
pVector getFirstNeighbor(pVector stack, pNode thisNode) {
    for (int i = 0; i < thisNode->edgeCount; i++) {
        pNode neighbor = thisNode->edges[i]->destination;
        if (neighbor->visitStatus == 0) {
            stack = addForward(stack, neighbor);
            return stack;
        }
    }
    return stack;
}

// Verifica se um nó já foi processado (se todos os seus vizinhos já foram processados)
pNode hasBeenVisited(pNode thisNode) {
    int visitedNeighbors = 0;
    for (int i = 0; i < thisNode->edgeCount; i++) {
        if (thisNode->edges[i]->destination->visitStatus == 1) {
            visitedNeighbors++;
        }
        // Detecta o primeiro elemento que repete em ciclos
        else if (thisNode->edges[i]->destination->visitStatus == -1) {
            thisNode->edges[i]->destination->visitStatus = 2;
            return thisNode->edges[i]->destination;
        }
    }

    if (visitedNeighbors == thisNode->edgeCount)
        thisNode->visitStatus = 1;

    return thisNode;
}

// Retorna um vetor com o ciclo que estava na pilha 
pVector getCycle(pVector stack) {
    pVector cycle = initVector();
    cycle->nodes[0] = stack->nodes[0];
    cycle->itemCount++;

    for (int i = 1; i < stack->itemCount; i++) {
        if (stack->nodes[i] == stack->nodes[0]) {
            break;
        }
        cycle->nodes[i] = stack->nodes[i];
        cycle->itemCount++;
    }
    
    freeVector(stack);
    return cycle;
}

// Inicia o ciclo pelo elemento de menor valor lexicográfico
pVector sortCycle(pVector cycle) {
    // Encontra o menor elemento
    pNode first = cycle->nodes[0];
    for (int i = 1; i < cycle->itemCount; i++) {
        if (strcmp(cycle->nodes[i]->name, first->name) < 0) {
            first = cycle->nodes[i];
        }
    }

    // Gira o vetor para a esquerda até que a primeira posição contenha o menor
    while (cycle->nodes[0] != first) {
        for (int i = 0; i <= cycle->itemCount; i++) {
            if (i == 0) {
                cycle->nodes[cycle->itemCount] = cycle->nodes[i];
            } else {
                cycle->nodes[i - 1] = cycle->nodes[i];
            }
        }
    }
    cycle->nodes[0]->visitStatus = 2;
    cycle->nodes[cycle->itemCount] = cycle->nodes[0];
    cycle->itemCount++;


    return cycle;
}

// Realiza uma busca em profundidade no grafo
pVector depthFirstSearch(pGraph graph, pVector topologicalSort) {
    pVector stack = initVector();
    stack->nodes[0] = graph->adj[0];
    stack->itemCount++;

    // Repete até que a pilha esteja vazia
    while (stack->itemCount > 0) {
        pNode thisNode = stack->nodes[0];
        thisNode = hasBeenVisited(thisNode);

        // Se um ciclo for encontrado, retorna a pilha que contém o ciclo
        if (thisNode->visitStatus == 2) {
            freeVector(topologicalSort);
            addForward(stack, thisNode);

            pVector cycle = getCycle(stack);
            cycle = sortCycle(cycle);
            return cycle;
        }

        // Se thisNode já foi visitado, ele é removido da pilha e adicionado à resposta
        if (thisNode->visitStatus == 1) {
            topologicalSort->nodes[topologicalSort->itemCount] = thisNode;
            topologicalSort->itemCount++;
            removeIndex(stack, 0);
            continue;
        }

        thisNode->visitStatus = -1;
        // Adiciona o primeiro vizinho não visitado de thisNode à fila
        stack = getFirstNeighbor(stack, thisNode);
    }

    freeVector(stack);
    return topologicalSort;
}



/* ===== BFS ===== */
/* visitStatus: -1 = Visitando; 1 = Não visitado; 0 = Visitado*/
// Retorna um vetor com todos os vizinhos não visitados de thisNode
pVector getNeighbors(pNode thisNode) {
    pVector updater = initVector();

    for (int i = 0; i < thisNode->edgeCount; i++) {
        pNode thisNeighbor = thisNode->edges[i]->destination;
        if (thisNeighbor->visitStatus == 1) {
            thisNeighbor->visitStatus = 0;
            updater->nodes[updater->itemCount] = thisNeighbor;
            updater->itemCount++;
        }
    }

    return updater;
}

// Adiciona a fila atual à distances e insere NULL no final, marcando a mudança de nível
pVector updateDistances(pVector queue, pVector distances) {
    for (int i = 0; i < queue->itemCount; i++) {
        distances->nodes[distances->itemCount] = queue->nodes[i];
        distances->itemCount++;
    }
    distances->nodes[distances->itemCount] = NULL;
    distances->itemCount++;

    return distances;
}

// Realiza uma busca em largura no grafo
pVector breadthFirstSearch(pGraph graph, pVector distances) {
    pVector queue = initVector();
    queue->nodes[0] = graph->adj[0];
    queue->nodes[0]->visitStatus = 0;
    queue->itemCount++;

    // Repete até que a fila esteja vazia
    while (queue->itemCount > 0) {

        bubbleSort(queue);
        distances = updateDistances(queue, distances);

        // Para cada item da fila:
        int initialCount = queue->itemCount;
        for (int i = 0; i < initialCount; i++) {
            // Se ele não tiver vizinhos, é removido da fila
            if (queue->nodes[0]->edgeCount == 0) {
                removeIndex(queue, 0);
                continue;
            }
            
            // Substitui-o pelos seus vizinhos não visitados
            pVector neighbors = getNeighbors(queue->nodes[0]);
            queue = fuseVectors(queue, neighbors, queue->itemCount);
            queue = removeIndex(queue, 0);
            freeVector(neighbors);
        }
    }

    freeVector(queue);
    return distances;
}



/* ===== PROCESSAMENTO DO INPUT ===== */
// Organiza adequadamente cada nome da linha de entrada em um vetor em ordem lexicográfica
pStringVector getDependencies(char *line, int beginning) {
    pStringVector sortedList = malloc(sizeof(struct vector));
    sortedList->names = malloc(lineMAX * sizeof(char*));
    sortedList->itemCount = 0;

    while (beginning <= strlen(line)) {     
        // Abstrai cada nome
        char *currentDependency = malloc(nameMAX * sizeof(char));
        for (int i = 0; i < nameMAX; i++) {
            if (line[beginning] == ' ' || line[beginning] == '\0') {
                currentDependency[i] = '\0';
                break;
            }
            currentDependency[i] = line[beginning];
            beginning++;
        }

        // Organiza-o no vetor
        sortedList = insertionSort(sortedList, currentDependency);
        beginning++;
    }

    return sortedList;
}

// Retorna o nome do alvo da linha
char *getTarget(char *line) {
    char *target = malloc(nameMAX * sizeof(char));
    for (int i = 0; i < nameMAX; i++) {
        if (line[i] == ':') {
            target[i] = '\0';
            break;
        }
        target[i] = line[i];
    }

    return target;
}

// Abstrai da entrada o alvo e suas dependências
void readLine(pGraph graph, char *line) {
    char *targetName = getTarget(line);
    pNode target = createNode(graph, targetName);
    pStringVector everyDependency = getDependencies(line, strlen(targetName) + 2);
    
    // Para cada dependência, cria uma aresta
    for (int i = 0; i < everyDependency->itemCount; i++)
        insertEdge(graph, target, everyDependency->names[i]);
    
    free(targetName);
    freeStringVector(everyDependency);
}



/* ===== MAIN ===== */
// Executa o código principal
int main() {
    char *line = malloc(lineMAX * sizeof(char));
    pGraph graph = initGraph();
    
    // Lê linhas até o EOF
    while (scanf(" %[^\n]", line) == 1) {
        if (line[0] == '\n' && line[1] == '\0') 
            continue;

        readLine(graph, line);
    }

    // Realiza a ordenação topológica
    pVector topologicalSort = initVector();
    pVector distances = initVector();
    topologicalSort = depthFirstSearch(graph, topologicalSort);

    // Se o DFS encontrar um ciclo:
    if (topologicalSort->nodes[0]->visitStatus == 2) {
        printCycle(graph, topologicalSort);
    } else {
        printTopologicalOrder(topologicalSort);
        distances = breadthFirstSearch(graph, distances);
        printDistance(graph, distances);
    }


    free(line);
    freeVector(topologicalSort);
    freeVector(distances);
    freeGraph(graph);
    return 0;
}