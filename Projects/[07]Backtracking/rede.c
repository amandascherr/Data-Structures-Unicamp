#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define verticesMAX 200
#define edgesMAX 12000

typedef struct vector *pVector;
typedef struct node *pNode;
typedef struct graph *pGraph;
typedef struct clique *pClique;

struct vector {
    pNode *nodes;
    int itemCount, influence;
};

struct node {
    int index, influence, edgeCount;
    pNode *connections;
};

struct graph {
    pNode *adj, *sortedAdj;
    int nodeCount, edgeCount, mostInfluentialID, influenceLeft;
    bool disconnected;
};

struct clique {
    pNode *nodes;
    int itemCount; int sum;
};



/* ===== GERENCIAMENTO DE MEMÓRIA ===== */
// Inicializa o grafo
pGraph initGraph() {   
    pGraph graph = malloc(sizeof(struct graph));
    graph->nodeCount = 0;
    graph->mostInfluentialID = 0;
    graph->influenceLeft = 0;
    graph->disconnected = false;

    graph->adj = malloc(verticesMAX * sizeof(pNode));
    graph->sortedAdj = malloc(verticesMAX * sizeof(pNode));

    return graph;
}

// Inicializa um vetor
pVector initVector() {
    pVector vector = malloc(verticesMAX * sizeof(struct vector));
    vector->nodes = malloc(verticesMAX * sizeof(pNode));
    vector->itemCount = 0;
    vector->influence = 0;

    return vector;
}

// Inicializa um clique
pClique initClique() {
    pClique clique = malloc(sizeof(struct clique));
    clique->nodes = malloc(verticesMAX * sizeof(pNode));
    clique->itemCount = 0;
    clique->sum = 0;

    return clique;
}

// Libera o grafo
void freeGraph(pGraph graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        free(graph->adj[i]->connections);
        free(graph->adj[i]);
    }
    free(graph->adj);
    free(graph->sortedAdj);
    free(graph);
}

// Libera um vetor
void freeVector(pVector vector) {
    free(vector->nodes);
    free(vector);
}

// Libera um clique
void freeClique(pClique clique) {
    free(clique->nodes);
    free(clique);
}



/* ===== PRINTS ===== */
// Imprime o grafo em forma de lista de adjacência (developer's tool)
void printGraph(pGraph graph) {
    printf("\n");
    printf("======= GRAFO ATUAL =======\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        pNode this = graph->adj[i];
        printf("v[%d]: ", i);
        for (int j = 0; j < this->edgeCount; j++) {
            printf("%d -> ", this->connections[j]->index);
        }
        printf("\n");
    }
    printf("\n");
}

// Imprime o clique ótimo
void printClique(pClique clique) {
    printf("%d\n", clique->sum);
    for (int i = 0; i < clique->itemCount; i++) {
        printf("%d ", clique->nodes[i]->index);
    }
    printf("\n");
}    

// Imprime um vetor (developer's tool)
void printStack(pVector stack) {
    printf("VETOR:\n");
    for (int i = 0; i < stack->itemCount; i++) {
        printf("%d ", stack->nodes[i]->index);
    }    
    printf("\n");
}    



/* ===== MANIPULAÇÃO DO GRAFO ===== */
// Insere um nó na lista ordenada por influência do grafo
pGraph insertionSort(pGraph graph, pNode thisNode) {
    int i;
    for (i = graph->nodeCount - 2; i >= 0 && thisNode->influence > graph->sortedAdj[i]->influence; i--)
        graph->sortedAdj[i + 1] = graph->sortedAdj[i];
    graph->sortedAdj[i + 1] = thisNode;

    return graph;
}

// Retorna o nó de posição index no grafo
pNode searchNode(pGraph graph, int index) {
    pNode thisNode = graph->adj[index];
    return thisNode;
}

// Cria um novo vértice para o grafo
pNode createNode(pGraph graph, int index, int influence) {
    pNode newNode = malloc(sizeof(struct node));
    newNode->connections = malloc(verticesMAX * sizeof(pNode));

    newNode->influence = influence;
    newNode->edgeCount = 0;
    newNode->index = index;
    
    graph->adj[index] = newNode;
    graph->nodeCount++;
    graph->influenceLeft = graph->influenceLeft + influence;

    graph = insertionSort(graph, newNode);
        
    return newNode;
}

// Cria uma aresta entre dois nós
void insertEdge(pGraph graph, int one, int two) {
    pNode a = searchNode(graph, one);
    pNode b = searchNode(graph, two);

    // Insere um nó na lista de vizinhos do outro
    a->connections[a->edgeCount] = b;
    a->edgeCount++;

    b->connections[b->edgeCount] = a;
    b->edgeCount++;
}

// Verifica se existe uma aresta entre dois nós
bool checkEdge(pNode a, pNode b) {
    // if (a->connections[b->index] == b)
    //     return true;
    if (a->edgeCount <= b->edgeCount) {
        for (int i = 0; i < a->edgeCount; i++) {
            if (a->connections[i] == b) {
                return true;
            }
        }
    } else {
        for (int i = 0; i < b->edgeCount; i++) {
            if (b->connections[i] == a) {
                return true;
            }
        }
    }

    return false;
}



/* ===== MANIPULAÇÃO DE CLIQUES ===== */
// Remove um elemento de uma posição específica do clique
pClique removeIndexClique(pClique clique, int position) {
    clique->sum = clique->sum - clique->nodes[position]->influence;
    for(; position < clique->itemCount - 1; position ++)
        clique->nodes[position] = clique->nodes[position + 1];
    clique->itemCount--;    

    return clique;
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

// // Ordena o clique em ordem crescente
pClique bubbleSort(pClique clique) {
    int loops = 0;
    while (loops < clique->itemCount - 1) {
        for (int i = 0; i < clique->itemCount - 1; i++) {
            pNode thisNode = clique->nodes[i];
            pNode next = clique->nodes[i + 1];
    
            if (thisNode->index > next->index) {
                clique->nodes[i] = next;
                clique->nodes[i + 1] = thisNode;
                loops = 0;
            } else {
                loops++;
            }    
        }    
    }    

    return clique;
}    

// Adiciona um elemento a um clique
pClique addElement(pClique clique, pNode thisNode) {
    clique->nodes[clique->itemCount] = thisNode;
    clique->sum = clique->sum + thisNode->influence;
    clique->itemCount++;

    return clique;
}    

// Duplica um clique
void copyClique(pClique copy, pClique original) {
    for (int i = 0; i < original->itemCount; i++) {
        copy->nodes[i] = original->nodes[i];
    }
    copy->itemCount = original->itemCount;
    copy->sum = original->sum;
}

// Verifica se um nó existe em um clique
bool exists(pClique clique, pNode thisNode) {
    for (int i = 0; i < clique->itemCount; i++) {
        if (clique->nodes[i] == thisNode)
            return true;
    }
    return false;
}



/* ===== Prunnings ===== */
// Verifica se um nó tem o mesmo número de arestas que o número de vértices do clique (menos ele)
bool maximalCliqueCheck(pClique clique, pNode thisNode) {
    if (thisNode->edgeCount == clique->itemCount - 1) {
        return true;
    }    
    return false;
}

// Verifica se um nó pode entrar no clique atual
bool cliqueCheck(pClique clique, pNode thisNode) {
    for (int i = 0; i < clique->itemCount; i++) {
        if (checkEdge(thisNode, clique->nodes[i]) == false) {
            return false;
        }    
    }

    return true;
}    

// Verifica se é necessário podar a partir do nó atual
bool prune(pGraph graph, pClique bestClique, pClique partialSolution, pVector nodesLeft) {
    // Se o grafo for desconexo, a lógica de prune não se aplica
    if (graph->disconnected == true) {
        return false;
    }

    // Verifica o limite superior que é possível atingir
    if ((nodesLeft->influence + partialSolution->sum) <= bestClique->sum) {
        return true;
    } else {
        return false;
    }
}


/* ===== Backtracking ===== */
// Percorre soluções utilizando backtracking
void backtrack(pGraph graph, pVector nodesLeft, pClique partialSolution, pClique bestClique) {
    if (nodesLeft->itemCount == 0) 
        return;
    
    pNode this = nodesLeft->nodes[0];
    nodesLeft = removeIndex(nodesLeft, 0);
    nodesLeft->influence = nodesLeft->influence - this->influence;

    
    // Toma a decisão de adicionar um elemento ao clique
    if (cliqueCheck(partialSolution, this) && maximalCliqueCheck(partialSolution, this) == false) {
        if (prune(graph, bestClique, partialSolution, nodesLeft) == false) {
            // Adiciona o elemento atual à solução parcial
            partialSolution = addElement(partialSolution, this);
            backtrack(graph, nodesLeft, partialSolution, bestClique);
            
            // Verifica se a solução parcial encontrada é a melhor até então
            if (partialSolution->sum > bestClique->sum) {
                copyClique(bestClique, partialSolution);
            }

            // Retorna à última decisão para avaliar mais cliques
            partialSolution = removeIndexClique(partialSolution, partialSolution->itemCount - 1);
        }
    }
        
    // Toma a decisão de não adicionar
    backtrack(graph, nodesLeft, partialSolution, bestClique);

    nodesLeft = addForward(nodesLeft, this);
    nodesLeft->influence = nodesLeft->influence + this->influence;
}

// Encontra o clique ótimo
pClique findBestClique(pGraph graph, pClique bestClique) {
    pVector nodesLeft = initVector();
    for (int i = 0; i < graph->nodeCount; i++) {
        nodesLeft->nodes[i] = graph->sortedAdj[i];
        nodesLeft->itemCount++;
        nodesLeft->influence = nodesLeft->influence + graph->sortedAdj[i]->influence;
        if (graph->adj[i]->edgeCount == 0) {
            graph->disconnected = true;
        }
    }

    pClique partialClique = initClique();
    backtrack(graph, nodesLeft, partialClique, bestClique);

    freeClique(partialClique);
    freeVector(nodesLeft);

    return bestClique;
}



/* ===== MAIN ===== */
// Executa o código principal
int main() {
    int nodes, edges, influence, one, two;
    pGraph graph = initGraph();
    pClique bestClique = initClique();
    
    // Lê a entrada
    scanf("%d %d", &nodes, &edges);
    for (int i = 0; i < nodes; i++) {
        scanf("%d", &influence);
        createNode(graph, i, influence);
    }
    for (int i = 0; i < edges; i++) {
        scanf("%d %d", &one, &two);
        insertEdge(graph, one, two);
    }

    // Encontra, ordena e imprime o clique ótimo
    bestClique = findBestClique(graph, bestClique);
    bestClique = bubbleSort(bestClique);
    printClique(bestClique);

    freeGraph(graph);
    freeClique(bestClique);
    return 0;
}