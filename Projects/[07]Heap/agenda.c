#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX 155

// Se não subir, necessariamente vai descer? Não né, como fazer então

typedef struct item *pItem;

struct item{
    int ID, duration, priority;
    char *name;
};

typedef struct {
    pItem *vMax, *vMin;
    int *IDmax, *IDmin;
    int Max, currentSize;
} Heaps;

#define parent(i) ((i - 1) / 2) 
#define Lkid(i) ((2 * i) + 1)
#define Rkid(i) ((2 * i) + 2)

// Verifica os critérios de desempate para o maxHeap
bool climbMaxCheck(Heaps *agenda, int otherIndex, int k) {
    bool climb = false;

    if (agenda->vMax[otherIndex]->priority < agenda->vMax[k]->priority)
            climb = true;

    if (agenda->vMax[otherIndex]->priority == agenda->vMax[k]->priority) {
        if (agenda->vMax[otherIndex]->duration < agenda->vMax[k]->duration)
            climb = true;

        if (agenda->vMax[otherIndex]->duration == agenda->vMax[k]->duration) {
            if (agenda->vMax[otherIndex]->ID > agenda->vMax[k]->ID)
                climb = true; 
        }
    }
    return climb;
}

// Verifica os critérios de desempate para o minHeap
bool climbMinCheck(Heaps *agenda, int otherIndex, int k) {
    bool climb = false;

    if (agenda->vMin[otherIndex]->duration > agenda->vMin[k]->duration)
            climb = true;

    if (agenda->vMin[otherIndex]->duration == agenda->vMin[k]->duration) {
        if (agenda->vMin[otherIndex]->priority < agenda->vMin[k]->priority)
            climb = true;

        if (agenda->vMin[otherIndex]->priority == agenda->vMin[k]->priority) {
            if (agenda->vMin[otherIndex]->ID > agenda->vMin[k]->ID)
                climb = true;   
        }
    }
    return climb;
}

// Troca a posição de dois itens em um heap e atualiza o vetor de índices
void swap(pItem *heap, int *vID, int firstIndex, int lastIndex) {
    pItem aux;

    // Utilizando um item auxiliar, troca a posição de dois itens no heap
    aux = heap[firstIndex];
    heap[firstIndex] = heap[lastIndex];
    heap[lastIndex] = aux;

    // Inverte as posições dos elementos trocados no vetor de índices
    vID[heap[firstIndex]->ID - 1] = firstIndex;
    vID[heap[lastIndex]->ID - 1] = lastIndex;
}

// Sobe no maxHeap
void climbMaxHeap(Heaps *agenda, int k) {
    if (k > 0) {
        if (climbMaxCheck(agenda, parent(k), k) == true) {
            swap(agenda->vMax, agenda->IDmax, k, parent(k));
            climbMaxHeap(agenda, parent(k));
        }
    }
}

// Sobe no minHeap
void climbMinHeap(Heaps *agenda, int k) {
    if (k > 0) {
        if (climbMinCheck(agenda, parent(k), k) == true) {
            swap(agenda->vMin, agenda->IDmin, k, parent(k));
            climbMinHeap(agenda, parent(k));
        }
    }
}

// Insere um novo elemento no final de ambos os heaps
void insert(Heaps *agenda) {
    pItem newItem = malloc(sizeof(struct item));
    newItem->name = malloc(MAX * sizeof(char));

    // Atualizando os atributos do novo item a ser inserido
    scanf(" %d %d %d", &newItem->ID, &newItem->priority, &newItem->duration);
    scanf(" %[^\n]", newItem->name);

    // Inserindo o item no final das filas de prioridade e atualizando seu tamanho e o vetor auxiliar
    agenda->vMax[agenda->currentSize] = newItem;
    agenda->IDmax[newItem->ID - 1] = agenda->currentSize;
    agenda->vMin[agenda->currentSize] = newItem;
    agenda->IDmin[newItem->ID - 1] = agenda->currentSize;
    agenda->currentSize++;
    
    printf("%s adicionada - Atividades futuras: %d\n", newItem->name, agenda->currentSize);

    // Escalando o item nos heaps, caso necessário
    climbMaxHeap(agenda, agenda->currentSize - 1);
    climbMinHeap(agenda, agenda->currentSize - 1);
}

// Desce no maxHeap
void descendMaxHeap(Heaps *agenda, int k) {
    int biggestKidIndex;

    if (Lkid(k) < agenda->currentSize) {
        biggestKidIndex = Lkid(k);
        if (Rkid(k) < agenda->currentSize && climbMaxCheck(agenda, biggestKidIndex, Rkid(k)) == true)
            biggestKidIndex = Rkid(k);

        if (climbMaxCheck(agenda, biggestKidIndex, k) == false) {
            swap(agenda->vMax, agenda->IDmax, k, biggestKidIndex);
            descendMaxHeap(agenda, biggestKidIndex);
        }
    }
}

// Desce no minHeap
void descendMinHeap(Heaps *agenda, int k) {
    int smallestKidIndex;

    if (Lkid(k) < agenda->currentSize - 1) {
        smallestKidIndex = Lkid(k);
        if (Rkid(k) < agenda->currentSize - 1 && climbMinCheck(agenda, smallestKidIndex, Rkid(k)) == true)
            smallestKidIndex = Rkid(k);
        
        if (climbMinCheck(agenda, smallestKidIndex, k) == false) {
            swap(agenda->vMin, agenda->IDmin, k, smallestKidIndex);
            descendMinHeap(agenda, smallestKidIndex);
        }
    }
}

// Remove o primeiro elemento do minHeap
void removeMin(Heaps *agenda) {
    int posMax = agenda->IDmax[agenda->vMin[0]->ID - 1];

    agenda->currentSize--;
    // Tendo a posição do primeiro elemento do minHeap no maxHeap, troca este elemento com o último de ambos os heaps e desce-o no minHeap
    if (agenda->vMin[0]->ID != agenda->vMin[agenda->currentSize]->ID) {
        swap(agenda->vMin, agenda->IDmin, 0, agenda->currentSize);
        descendMinHeap(agenda, 0);
    }
    if (agenda->vMax[posMax]->ID != agenda->vMax[agenda->currentSize]->ID) {
        swap(agenda->vMax, agenda->IDmax, posMax, agenda->currentSize);
        // Verifica se o elemento subirá ou descerá no maxHeap
        if (posMax == 0) {
            descendMaxHeap(agenda, posMax);
        } else if (posMax == agenda->currentSize) {
            climbMaxHeap(agenda, posMax);
        } else {
            if (climbMaxCheck(agenda, parent(posMax), posMax)) {
                climbMaxHeap(agenda, posMax);
            } else {
                descendMaxHeap(agenda, posMax);
            }
        }
    }
    
    // Libera o último elemento
    free(agenda->vMin[agenda->currentSize]->name);
    free(agenda->vMin[agenda->currentSize]);
}

// Remove o primeiro elemento do maxheap
void removeMax(Heaps *agenda) {
    int posMin = agenda->IDmin[agenda->vMax[0]->ID - 1];
    
    agenda->currentSize--;
    // Tendo a posição do primeiro elemento do maxHeap no minHeap, troca este elemento com o último de ambos os heaps e desce-o no maxHeap
    if (agenda->vMax[0]->ID != agenda->vMax[agenda->currentSize]->ID) {
        swap(agenda->vMax, agenda->IDmax, 0, agenda->currentSize);
        descendMaxHeap(agenda, 0);
    } 

    if (agenda->vMin[posMin]->ID != agenda->vMin[agenda->currentSize]->ID) {
        swap(agenda->vMin, agenda->IDmin, posMin, agenda->currentSize);
        // Verifica se o elemento subirá ou descerá no minHeap
        if (posMin == 0) {
            descendMinHeap(agenda, posMin);
        } else if (posMin == agenda->currentSize) {
            climbMinHeap(agenda, posMin);
        } else {
            if (climbMinCheck(agenda, parent(posMin), posMin)) {
                climbMinHeap(agenda, posMin);
            } else {
                descendMinHeap(agenda, posMin);
            }
        }
    }

    // Libera o último elemento
    free(agenda->vMax[agenda->currentSize]->name);
    free(agenda->vMax[agenda->currentSize]);
}

// Altera a prioridade de um item pelo seu ID
void editPriority(Heaps *agenda) {
    int ID, X, posMax, posMin;
    scanf(" %d %d", &ID, &X);

    // Define as posições do item a ser editado no heapMax e no heapMin
    posMax = agenda->IDmax[ID - 1];
    posMin = agenda->IDmin[ID - 1];

    // Altera a prioridade, imprime a mudança e escala o item no maxHeap
    printf("Alterado %s %d -> ", agenda->vMax[posMax]->name, agenda->vMax[posMax]->priority);
    agenda->vMax[posMax]->priority = agenda->vMax[posMax]->priority + X;
    printf("%d\n", agenda->vMax[posMax]->priority);

    // Se o valor da prioridade aumentou, verifica se é possível subir em ambos os heaps e vice-versa
    if (X > 0) {
        if (posMax != 0)
            climbMaxHeap(agenda, posMax);
        if (posMin != 0)
            climbMinHeap(agenda, posMin);
    } else if (X < 0) {
        if (posMax != agenda->currentSize - 1)
            descendMaxHeap(agenda, posMax);
        if (posMin != agenda->currentSize - 1)
            descendMinHeap(agenda, posMin);
    }
}

// Imprime a agenda do dia enquanto ainda houver tempo livre ou até que acabem os itens da agenda
void viewAgenda(Heaps *agenda) {
    int freeTime = 600;

    while (agenda->currentSize != 0 && freeTime >= agenda->vMax[0]->duration) {
        printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", agenda->vMax[0]->ID, agenda->vMax[0]->name, agenda->vMax[0]->priority, agenda->vMax[0]->duration);
        freeTime = freeTime - agenda->vMax[0]->duration;
        removeMax(agenda);
    }

    while (agenda->currentSize != 0 && freeTime >= agenda->vMin[0]->duration) {
        printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", agenda->vMin[0]->ID, agenda->vMin[0]->name, agenda->vMin[0]->priority, agenda->vMin[0]->duration);
        freeTime = freeTime - agenda->vMin[0]->duration;
        removeMin(agenda);
    }
}

// Auxilia a visualização do heap (developer's tool)
void viewHeap(Heaps *agenda) {
    printf("\nGDB PREMIUM:\n");
    printf("Max-heap (Ordenado pela maior prioridade): \n");
    for (int i = 0; i < agenda->currentSize; i++)
        printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", agenda->vMax[i]->ID, agenda->vMax[i]->name, agenda->vMax[i]->priority, agenda->vMax[i]->duration);

    printf("Min-heap (Ordenado pela menor duração): \n");
    for (int i = 0; i < agenda->currentSize; i++)
        printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", agenda->vMin[i]->ID, agenda->vMin[i]->name, agenda->vMin[i]->priority, agenda->vMin[i]->duration);
    printf("\n");

    printf("Índices-Max: \n");
    for (int i = 0; i < agenda->currentSize; i++)
        printf("[%d] ", agenda->IDmax[i]);

    printf("\n");
    printf("Índices-Min: \n");
    for (int i = 0; i < agenda->currentSize; i++)
        printf("[%d] ", agenda->IDmin[i]);
    printf("\nPosição: ID da atividade - 1\nValor armazenado: posição da atividade de id i + 1 no heap\n");

    printf("\n");
}

// Analisa a string de entrada e chama a função correspondente 
void commandCenter(Heaps *agenda, char *input) {
    if (strcmp(input, "atividade") == 0) {
        insert(agenda);
    } else if (strcmp(input, "agenda") == 0) {
        printf("--AGENDA--\n");
        viewAgenda(agenda);
        printf("----------\n");
    } else if (strcmp(input, "altera") == 0) {
        editPriority(agenda);
    } else if (strcmp(input, "heap") == 0) {
        viewHeap(agenda);
    }
}

// Libera cada item adicionado, suas strings, os heaps e a agenda
void freeHeaps(Heaps *agenda) {
    for (int i = 0; i < agenda->currentSize; i++) {
        free(agenda->vMax[i]->name);
        free(agenda->vMax[i]);
    }
    free(agenda->IDmax);
    free(agenda->IDmin);
    free(agenda->vMax);
    free(agenda->vMin);
    free(agenda);
}

// Executa o código principal
int main() {
    int max;
    char *input;
    scanf(" %d", &max);

    // Aloca os 4 vetores que serão utilizados em uma estrutura "Agenda" para facilitar a chamada de funções
    Heaps *agenda = malloc(sizeof(Heaps));
    agenda->vMax = malloc((max + 2) * sizeof(pItem));
    agenda->vMin = malloc((max + 2) * sizeof(pItem));
    agenda->IDmax = malloc((max + 2) * sizeof(int));
    agenda->IDmin = malloc((max + 2) * sizeof(int));
    agenda->currentSize = 0;

    // Solicita comandos até que o usuário digite "encerrar"
    input = malloc(30 * sizeof(char));
    scanf(" %s", input);
    while (strcmp(input, "encerrar") != 0) {
        commandCenter(agenda, input);
        free(input);
        input = malloc(30 * sizeof(char));
        scanf(" %s", input);
    }
    printf("Restaram %d atividades\n", agenda->currentSize);
    
    free(input);
    freeHeaps(agenda);
    return 0;
}
