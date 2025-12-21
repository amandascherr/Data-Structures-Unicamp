// Para o desenvolvimento do deque, utilizei uma lista duplamente ligada,
// uma vez que esta fornece acesso simplificado às suas extremidades,
// facilitando a implementação de operações de inserção/remoção

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria a estrutura nó para uma lista duplamente ligada
typedef struct no *pNo;
struct no {
    char hero[51];
    pNo next;
    pNo prev;
};

// Guarda o primeiro e o último elemento da lista
typedef struct {
    pNo init;
    pNo end;
} List;

// Cria um deck vazio
void createDeck(List *deck) {
    deck->init = NULL;
    deck->end = NULL;
}

// Libera a memória de um deck
void destroyDeck(List *deck) {
    pNo current, backup;
    current = deck->init;

    while (current != NULL) {
        backup = current->next;
        free(current);
        current = backup;
    }
}

// Imprime o deck atual
void printDeck(List *deck) {
    pNo current;

    if (deck->init != NULL) {
        for (current = deck->init; current != NULL; current = current->next) {
            printf("%s ", current->hero);
        }
    }
}

// Adiciona um elemento no início da lista
void summonForward(List *deck, pNo newElement) {

    if (deck->end == NULL) {
        deck->end = newElement;
    }
    newElement->prev = NULL;
    newElement->next = deck->init;
    if (deck->init != NULL) {
        deck->init->prev = newElement;
    }
    deck->init = newElement;
}

// Adiciona um elemento no fim da lista
void summonBehind(List *deck, pNo newElement) {

    if (deck->init == NULL) {
        deck->init = newElement;
    }
    newElement->prev = deck->end;
    newElement->next = NULL;
    if (deck->end != NULL) {
        deck->end->next = newElement;
    }
    deck->end = newElement;
}

// Remove o elemento do início da lista
void removeForward(List *deck) {
    pNo update;

    if (deck->init != NULL) {
        // Verificando se o elemento a ser removido é o único da lista
        if (deck->init->next == NULL) {
            destroyDeck(deck);
            createDeck(deck);
        } else {
            // Guardando o segundo elemento em update, liberando o primeiro elemento e então
            // definindo o segundo elemento como o novo primeiro
            update = deck->init->next;
            free(deck->init);
            deck->init = update;
            deck->init->prev = NULL;
        }
    }
}

// Remove o elemento do fim da lista
void removeBehind(List *deck) {
    pNo update;

    // Verificando se o elemento a ser removido é o único da lista
    if (deck->init != NULL) {
        if (deck->init->next == NULL) {
            destroyDeck(deck);
            createDeck(deck);
        }
        else {
            // Guardando o penúltimo elemento em update, liberando o último elemento e então
            // definindo o penúltimo elemento como o novo último
            update = deck->end->prev;
            free(deck->end);
            deck->end = update;
            deck->end->next = NULL;
        }
    }
}

// Remove um elemento do início da lista e o insere no fim
void attack(List *deck) {
    pNo backup;

    // Verificando se o elemento a ser removido não é o único da lista
    //Baseado na função "removeForward"
    if (deck->init->next != NULL) {
        backup = deck->init;
        deck->init = backup->next;
        deck->init->prev = NULL;

        // Baseado na função "summonBehind"
        backup->prev = deck->end;           
        backup->next = NULL;
        deck->end->next = backup;
        deck->end = backup;
    }

    printf("%s atacou e foi para o fim\n", deck->end->hero);
}

// Adiciona um herói na posição index
void setPosition(List *deck, pNo newElement, int index) {
    pNo current, backup;
    int i;

    // Verificando se o elemento será adicionado na frente
    if (index == 0) {
        summonForward(deck, newElement);
    } else {
        // Se não, iterando pela lista para inserir o elemento na posição index
        i = 0;
        for (current = deck->init; current != NULL; current = current->next) {
            if (current->next == NULL) {
                if (i == index) {
                    newElement->next = current;
                    newElement-> prev = current->prev;
                    backup = current->prev;
                    current->prev = newElement;
                    backup->next = newElement;
                } else {
                    summonBehind(deck, newElement);
                }
                break;
            } else if (i == index) {
                backup = current->prev;
                current->prev = newElement;
                newElement->prev = backup;
                newElement->next = current;
                backup->next = newElement;
                break;
            }
            i++;
        }
    }
}

// Remove o herói da posição index
void removePosition(List *deck, int index) {
    pNo current, backup1, backup2;
    char name[51];
    int i;

    // Verificando se o elemento será removido da frente
    if (index == 0 && deck->init != NULL) {
        strcpy(name, deck->init->hero);
        removeForward(deck);
    } else {
        // Se não, iterando pela lista para remover o elemento da posição index
        i = 0;
        for (current = deck->init; current != NULL; current = current->next) {
            if (current->next == NULL) {
                strcpy(name, deck->end->hero);
                removeBehind(deck);
                break;
            } else if (i == index) {
                strcpy(name, current->hero);
                backup1 = current->prev;
                backup2 = current->next;
                free(current);
                backup1->next = backup2;
                backup2->prev = backup1;
                break;
            }
            i++;
        }
    }
    printf("%s removido da posicao %d\n", name, index);
}

// Inverte um deck
void reverseDeck(List *deck) {
    List deckReversed;
    deckReversed.init = NULL;
    deckReversed.end = NULL;

    //createDeck(&deckReversed);
    pNo current, reverse, last;

    reverse = NULL;
    last = NULL;
    current = deck->init;
    deckReversed.end = current;
    while (current != NULL) {
        last = current;
        current = last->next;
        last->next = reverse;
        reverse = last;
        reverse->prev = current;
    }
    deckReversed.init = last;

    if (deckReversed.init != NULL)
        deckReversed.init->prev = NULL;
    if (deckReversed.end != NULL)
        deckReversed.end->next = NULL;

    deck->init = deckReversed.init;
    deck->end = deckReversed.end;
}

// Copia os primeiros K itens de um deck
List copyToK(List *deck, int K) {
    List newDeck;
    createDeck(&newDeck);
    pNo newCurrent, current, last;
    int i;

    i = 1;
    for (current = deck->init; current != NULL; current = current->next) {
        // Criando o nó
        newCurrent = malloc(sizeof(struct no));
        strcpy(newCurrent->hero, current->hero);
        newCurrent->next = NULL;
        newCurrent->prev = NULL;
        // Definindo sua posição
        if (newDeck.init == NULL) {
            newDeck.init = newCurrent;
        } else {
            last->next = newCurrent;
            newCurrent->prev = last;
        }
        last = newCurrent;
        // Parando de copiar quando se passarem os K primerios
        if (i == K) {
            last->next = NULL;
            newDeck.end = last;
            break;
        }
        i++;
    }

    return newDeck;
}

// Copia os itens depois dos K primeiros de um deck
List copyAfterK(List *deck, int K) {
    List newDeck;
    createDeck(&newDeck);
    pNo newCurrent, current, last, starter;
    int i;

    i = 1;
    starter = deck->init;
    while (i != K) {
        starter = starter->next;
        i++;
    }
    starter = starter->next;

    for (current = starter; current != NULL; current = current->next) {
        // Criando o nó
        newCurrent = malloc(sizeof(struct no));
        strcpy(newCurrent->hero, current->hero);
        newCurrent->next = NULL;
        newCurrent->prev = NULL;
        // Definindo sua posição
        if (newDeck.init == NULL) {
            newDeck.init = newCurrent;
        } else {
            last->next = newCurrent;
            newCurrent->prev = last;
        }
        last = newCurrent;
    }
    last->next = NULL;
    newDeck.end = last;

    return newDeck;
}

// Copia os últimos K itens de um deck
List copyLastK(List *deck, int K) {
    List newDeck;
    createDeck(&newDeck);
    pNo newCurrent, current, last;
    int i;

    i = 1;
    for (current = deck->end; current != NULL; current = current->prev) {
        // Criando o nó
        newCurrent = malloc(sizeof(struct no));
        strcpy(newCurrent->hero, current->hero);
        newCurrent->next = NULL;
        newCurrent->prev = NULL;
        // Definindo sua posição
        if (newDeck.init == NULL) {
            newDeck.init = newCurrent;
        } else {
            last->next = newCurrent;
            newCurrent->prev = last;
        }
        last = newCurrent;
        // Parando de copiar depois de K repetições
        if (i == K) {
            last->next = NULL;
            newDeck.end = last;
            break;
        }
        i++;
    }

    reverseDeck(&newDeck);
    return newDeck;
}

// Copia os itens antes dos K últimos de um deck
List copyBeforeK(List *deck, int K) {
    List newDeck;
    createDeck(&newDeck);
    pNo newCurrent, current, last, starter;
    int i;

    i = 1;
    starter = deck->end;
    while (i != K) {
        starter = starter->prev;
        i++;
    }
    starter = starter->prev;

    for (current = starter; current != NULL; current = current->prev) {
        // Criando o nó
        newCurrent = malloc(sizeof(struct no));
        strcpy(newCurrent->hero, current->hero);
        newCurrent->next = NULL;
        newCurrent->prev = NULL;
        // Definindo sua posição
        if (newDeck.init == NULL) {
            newDeck.init = newCurrent;
        } else {
            last->next = newCurrent;
            newCurrent->prev = last;
        }
        last = newCurrent;
    }
    last->next = NULL;
    newDeck.end = last;

    reverseDeck(&newDeck);
    return newDeck;
}

// Concatena dois decks
void linkDeck(List *firstPart, List *secondPart, List *deck) {
    List finalDeck;
    firstPart->end->next = secondPart->init;
    secondPart->init->prev = firstPart->end;

    finalDeck.init = firstPart->init;
    finalDeck.end = secondPart->end;

    deck->init = finalDeck.init;
    deck->end = finalDeck.end;
}

// Inverte a ordem dos K primeiros heróis
void chaos(List *deck) {
    List firstPart, secondPart;
    createDeck(&firstPart);
    createDeck(&secondPart);
    int K;
    scanf(" %d", &K);

    printf("caos ");
    // Verificando se K é do tamanho do deck
    pNo current;
    current = deck->init;
    for (int i = 1; i <= K && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printDeck(deck);
        reverseDeck(deck);
        printf("-> ");
        printDeck(deck);
    } else {
        // Dividindo o deck em duas partes, uma do início até K e outra de K até o fim
        firstPart = copyToK(deck, K);
        printDeck(&firstPart);
        secondPart = copyAfterK(deck, K);

        // Invertendo a primeira parte do deck
        reverseDeck(&firstPart);
        printf("-> ");
        printDeck(&firstPart);

        // Concatenando ambas as partes
        linkDeck(&firstPart, &secondPart, deck);
    }
    printf("\n");
}

// Inverte a ordem dos K últimos heróis
void confusion(List *deck) {
    List firstPart, secondPart;
    createDeck(&firstPart);
    createDeck(&secondPart);
    int K;
    scanf(" %d", &K);

    printf("confusao ");
    // Verificando se K é do tamanho do deck
    pNo current;
    current = deck->init;
    for (int i = 1; i <= K && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printDeck(deck);
        reverseDeck(deck);
        printf("-> ");
        printDeck(deck);
    } else {
        // Dividindo o deck em duas partes, uma de K até o início e outra do fim até K
        firstPart = copyBeforeK(deck, K);
        secondPart = copyLastK(deck, K);
        printDeck(&secondPart);

        // Invertendo a segunda parte do deck
        reverseDeck(&secondPart);
        printf("-> ");
        printDeck(&secondPart);

        // Concatenando ambas as partes
        linkDeck(&firstPart, &secondPart, deck);
    }
    printf("\n");
}

// Lê a entrada do usuário e retorna um char* que contém a entrada 
char *readOperation() {
    char *string;

    // Alocando o espaço necessário para toda a entrada do usuário
    string = malloc(30 * sizeof(char));
    scanf(" %s", string);
    return string;
}

// Agrupa código comum às operações de adição
void addOperations(char *string, List *deck) {
    pNo newElement;
    int index;
    newElement = malloc(sizeof(struct no));
    scanf(" %s", newElement->hero);

    if (strcmp(string, "convocar-frente") == 0) {
        summonForward(deck, newElement);
        printf("%s entrou no inicio\n", deck->init->hero);
    } else if (strcmp(string, "convocar-fim") == 0) {
        summonBehind(deck, newElement);
        printf("%s entrou no fim\n", newElement->hero);
    } else if (strcmp(string, "posicionar") == 0) { 
        scanf(" %d", &index);
        setPosition(deck, newElement, index);
        printf("%s inserido na posicao %d\n", newElement->hero, index);
    }
}

// Agrupa código comum às operações de remoção
void removeOperations(char *string, List *deck) {
    char name[51];
    int index;

    if (strcmp(string, "puxar-frente") == 0) {
        strcpy(name, deck->init->hero);
        removeForward(deck);
        printf("%s removido do inicio\n", name);
    } else if (strcmp(string, "puxar-tras") == 0) {
        strcpy(name, deck->end->hero);
        removeBehind(deck);
        printf("%s removido do fim\n", name);
    } else if (strcmp(string, "remover") == 0) { 
        scanf(" %d", &index);
        removePosition(deck, index);
    }
}

// Chama cada operação de acordo com a situação
void operations(char *string, List *deck) {

    // Chamando a função que o comando solicitou pela análise da string de entrada 
    if (strcmp(string, "convocar-frente") == 0 || strcmp(string, "convocar-fim") == 0 || (strcmp(string, "posicionar") == 0)) {
        addOperations(string, deck);
    } else if (strcmp(string, "puxar-frente") == 0 || strcmp(string, "puxar-tras") == 0 || (strcmp(string, "remover") == 0)) {
        removeOperations(string, deck);
    } else if (strcmp(string, "ataque") == 0) {
        attack(deck);
    } else if (strcmp(string, "caos") == 0) {                // Fazer 
        chaos(deck);
    } else if (strcmp(string, "confusao") == 0) {            // Fazer 
        confusion(deck);
    } 
}

// Executa o código
int main() {
    char *command;
    List deck;
    createDeck(&deck);

    command = readOperation();
    while (strcmp(command, "sair") != 0) {
        operations(command, &deck);
        printf("ordem de ataque: ");
        printDeck(&deck);
        printf("\n");

        free(command);
        command = readOperation();
    }

    free(command);
    destroyDeck(&deck);
    return 0;
}

// Deixar tudo dinâmico e ajeitar o Valgrind
// destroyDeck sobra 1 malloc
