#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row, col, memoryUsedI, memoryUsedJ;
    int **matrix;

} Matrix;

// Define uma nova matriz de tamanho row x col, alocando a memória necessária e definindo seus valores iniciais
Matrix *newMatrix(int row, int col) {
    Matrix *game = malloc(sizeof(Matrix));
    game->row = game->memoryUsedI = row;
    game->col = game->memoryUsedJ = col;

    // Aloca a memória necessária para a matriz do jogo de Tobias
    game->matrix = malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        game->matrix[i] = malloc(col * sizeof(int));
    }

    // Lê os valores iniciais da matriz
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            scanf(" %d", &game->matrix[i][j]);
        }
    }
    return game;
}

// Libera a memória alocada para uma matriz
void freeMatrix(Matrix *game) {
    for (int i = 0; i < game->memoryUsedI; i++) {
        free(game->matrix[i]);
    }
    free(game->matrix);
    free(game);
}

// Adiciona um personagem (linha) na matriz
Matrix *addRow(Matrix *game) {
    Matrix *gameUpdate = malloc(sizeof(Matrix));;
    // Copia os dados da matriz atual para a nova
    gameUpdate->row = game->row + 1;
    gameUpdate->col = game->col;
    gameUpdate->memoryUsedI = game->memoryUsedI;
    gameUpdate->memoryUsedJ = game->memoryUsedJ;

    // Verifica se há espaço suficiente na matriz para a alocação da nova linha
    if (game->memoryUsedI < gameUpdate->row) {
        gameUpdate->memoryUsedI = 2 * game->memoryUsedI;
        // Aloca a memória necessária para a matriz atualizada (com o dobro do tamanho)
        gameUpdate->matrix = malloc(gameUpdate->memoryUsedI * sizeof(int*));
        for (int i = 0; i < gameUpdate->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(gameUpdate->memoryUsedJ * sizeof(int));
        }
        // Imprime as alterações da memória
        printf("Tamanho da matriz: %d x %d\n", gameUpdate->row, gameUpdate->col);
        printf("Memória realocada: %d x %d -> %d x %d\n", game->memoryUsedI, game->memoryUsedJ, gameUpdate->memoryUsedI, gameUpdate->memoryUsedJ);
    } else {
        // Aloca a memória necessária para a matriz atualizada que requer o mesmo tamanho de memória que a anterior
        gameUpdate->matrix = malloc(game->memoryUsedI * sizeof(int*));
        for (int i = 0; i < game->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(game->memoryUsedJ * sizeof(int));
        }
    }

    // Copia os valores da matriz atual para a nova matriz
    for (int i = 0; i < game->row; i++) {
        for (int j = 0; j < game->col; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i][j];
        }
    }

    // Lê o valor da nova linha
    for (int j = 0; j < game->col; j++) {
        scanf(" %d", &gameUpdate->matrix[game->row][j]);
    }

    // Libera a memória da antiga matriz e retorna a nova
    freeMatrix(game);
    return gameUpdate;
}

// Adiciona uma habilidade (coluna) na matriz
Matrix *addCol(Matrix *game) {
    Matrix *gameUpdate = malloc(sizeof(Matrix));;
    // Copia os dados da matriz atual para a nova
    gameUpdate->row = game->row;
    gameUpdate->col = game->col + 1;
    gameUpdate->memoryUsedI = game->memoryUsedI;
    gameUpdate->memoryUsedJ = game->memoryUsedJ;

    // Verifica se há espaço suficiente na matriz para a alocação da nova coluna
    if (game->memoryUsedJ < gameUpdate->col) {
        gameUpdate->memoryUsedJ = 2 * game->memoryUsedJ;
        // Aloca a memória necessária para a matriz atualizada (com o dobro do tamanho)
        gameUpdate->matrix = malloc(gameUpdate->memoryUsedI * sizeof(int*));
        for (int i = 0; i < gameUpdate->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(gameUpdate->memoryUsedJ * sizeof(int));
        }
        // Imprime as alterações da memória
        printf("Tamanho da matriz: %d x %d\n", gameUpdate->row, gameUpdate->col);
        printf("Memória realocada: %d x %d -> %d x %d\n", game->memoryUsedI, game->memoryUsedJ, gameUpdate->memoryUsedI, gameUpdate->memoryUsedJ);
    } else {
        // Aloca a memória necessária para a matriz atualizada que requer o mesmo tamanho de memória que a anterior
        gameUpdate->matrix = malloc(game->memoryUsedI * sizeof(int*));
        for (int i = 0; i < game->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(game->memoryUsedJ * sizeof(int));
        }
    }

    // Copia os valores da matriz atual para a nova matriz
    for (int i = 0; i < game->row; i++) {
        for (int j = 0; j < game->col; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i][j];
        }
    }

    int value, j;
    // Lê o valor da nova coluna e insere-o na posição adequada, mantendo a ordem crescente (Vetor ordenado)
    for (int i = 0; i < game->row; i++) {
        scanf(" %d", &value);
        for (j = game->col - 1; j >= 0 && game->matrix[i][j] > value; j--) {
            gameUpdate->matrix[i][j + 1] = gameUpdate->matrix[i][j];
        }
        gameUpdate->matrix[i][j + 1] = value;
    }

    // Libera a memória da antiga matriz e retorna a nova
    freeMatrix(game);
    return gameUpdate;
}

// Remove um personagem (linha) da matriz
Matrix *removeRow(Matrix *game) {
    Matrix *gameUpdate = malloc(sizeof(Matrix));;
    int index;
    // Copia os dados da matriz atual para a nova
    gameUpdate->row = game->row - 1;
    gameUpdate->col = game->col;
    gameUpdate->memoryUsedI = game->memoryUsedI;
    gameUpdate->memoryUsedJ = game->memoryUsedJ;

    // Lê o índice da linha a ser removida
    scanf(" %d", &index);

    // Verifica se, após a remoção da linha indicada, apenas M/4 linhas estarão alocadas
    if (gameUpdate->row <= (game->memoryUsedI / 4)) {
        // Aloca a memória necessária (M/2) para a matriz atualizada
        gameUpdate->memoryUsedI = game->memoryUsedI / 2;
        gameUpdate->matrix = malloc((gameUpdate->memoryUsedI) * sizeof(int*));
        for (int i = 0; i < (gameUpdate->memoryUsedI); i++) {
            gameUpdate->matrix[i] = malloc(gameUpdate->memoryUsedJ * sizeof(int));
        }
        // Imprime as alterações da memória
        printf("Tamanho da matriz: %d x %d\n", gameUpdate->row, gameUpdate->col);
        printf("Memória realocada: %d x %d -> %d x %d\n", game->memoryUsedI, game->memoryUsedJ, gameUpdate->memoryUsedI, gameUpdate->memoryUsedJ);
    } else {
        // Aloca a memória necessária para a matriz atualizada que requer o mesmo tamanho de memória que a anterior
        gameUpdate->matrix = malloc(game->memoryUsedI * sizeof(int*));
        for (int i = 0; i < game->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(game->memoryUsedJ * sizeof(int));
        }
    }

    // Copia os valores das linhas de índice menor que o indicado da matriz atual para a nova matriz
    for (int i = 0; i < index; i++) {
        for (int j = 0; j < game->col; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i][j];
        }
    }
    // Copia os valores das linhas de índice maior que o indicado da matriz atual para a nova matriz
    for (int i = index; i < gameUpdate->row; i++) {
        for (int j = 0; j < game->col; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i + 1][j];
        }
    }

    // Libera a memória da antiga matriz e retorna a nova
    freeMatrix(game);
    return gameUpdate;
}

// Remove uma habilidade (coluna) da matriz
Matrix *removeCol(Matrix *game) {
    Matrix *gameUpdate = malloc(sizeof(Matrix));;
    int index;
    // Copia os dados da matriz atual para a nova
    gameUpdate->row = game->row;
    gameUpdate->col = game->col - 1;
    gameUpdate->memoryUsedI = game->memoryUsedI;
    gameUpdate->memoryUsedJ = game->memoryUsedJ;

    // Lê o índice da coluna a ser removida
    scanf(" %d", &index);

    // Verifica se, após a remoção da coluna indicada, apenas M/4 colunas estarão alocadas
    if (gameUpdate->col <= (game->memoryUsedJ / 4)) {
        // Aloca a memória necessária (M/2) para a matriz atualizada
        gameUpdate->memoryUsedJ = game->memoryUsedJ / 2;
        gameUpdate->matrix = malloc((gameUpdate->memoryUsedI) * sizeof(int*));
        for (int i = 0; i < (gameUpdate->memoryUsedI); i++) {
            gameUpdate->matrix[i] = malloc(gameUpdate->memoryUsedJ * sizeof(int));
        }
        // Imprime as alterações da memória
        printf("Tamanho da matriz: %d x %d\n", gameUpdate->row, gameUpdate->col);
        printf("Memória realocada: %d x %d -> %d x %d\n", game->memoryUsedI, game->memoryUsedJ, gameUpdate->memoryUsedI, gameUpdate->memoryUsedJ);
    } else {
        // Aloca a memória necessária para a matriz atualizada que requer o mesmo tamanho de memória que a anterior
        gameUpdate->matrix = malloc(game->memoryUsedI * sizeof(int*));
        for (int i = 0; i < game->memoryUsedI; i++) {
            gameUpdate->matrix[i] = malloc(game->memoryUsedJ * sizeof(int));
        }
    }

    // Copia os valores das colunas de índice menor que o indicado para ser removido da matriz atual para a nova matriz
    for (int i = 0; i < game->row; i++) {
        for (int j = 0; j < index; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i][j];
        }
    }

    // Remove uma coluna da matriz, deslocando os elementos em índices maiores para a esquerda
    for (int i = 0; i < game->row; i++) {
        for (int j = index; j < game->col - 1; j++) {
            gameUpdate->matrix[i][j] = game->matrix[i][j + 1];
        }
    }

    // Libera a memória da antiga matriz e retorna a nova
    freeMatrix(game);
    return gameUpdate;
}

// Lê a entrada do usuário e retorna um char* que contém a entrada 
char *readInput() {
    char *string;
    // Alocando o espaço necessário para a string no pior caso possível (2 caracteres + \0)
    string = malloc(3 * sizeof(char));
    scanf("%s", string);

    return string;
}

// Chama cada operação da matriz de acordo com a situação
Matrix *operations(char *string, Matrix *game) {

    // Chamando a função que o comando solicitou pela análise da string de entrada 
    if (string[0] == 'A') {
        if (string[1] == 'P') {
            game = addRow(game);
        } else {
           game = addCol(game);
        }
    } else {
        if (string[1] == 'P') {
            game = removeRow(game);
        } else {
           game = removeCol(game);
        }
    }
    return game;
}

// Imprime a matriz atual
void viewMatrix(Matrix *game) {
    printf("MATRIZ ATUAL:\n");
    for (int i = 0; i < game->row; i++) {
        for (int j = 0; j < game->col; j++) {
            printf("%d ", game->matrix[i][j]);
        }
        printf("\n");
    } 
}

// Executa o código principal
int main() {
    int M, N;
    char *string;

    // Lê a quantidade inicial de linhas M e de colunas N e cria uma matriz com a entrada fornecida
    scanf("%d %d", &M, &N);
    Matrix *game = newMatrix(M, N);

    // Lê o primeiro comando do usuário e executa-o, repete o processo até que o comando seja "S"
    string = readInput();
    while (string[0] != 'S') {

        // Verifica se o comando é 'V' ou se exige uma alteração da matriz 
        if (string[0] == 'V') {
            viewMatrix(game);
        } else {
            game = operations(string, game);
        }

        // Libera a memória da string utilizada até então e lê uma nova entrada
        free(string);
        string = readInput();
    }

    // Libera a memória da última string e do último "game"
    free(string);
    freeMatrix(game);
    return 0;
}