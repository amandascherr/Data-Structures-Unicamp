#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define filesMAX 20000
#define tagsMAX 3000
#define nameMAX 31
#define descriptionMAX 151

typedef struct file *pFile;
typedef struct tag *pTag;
typedef struct hash *pHash;

struct file {
    char *name, *description;
    pTag *tags;
    int tagCount, insertionOrder;
    bool removed;
};

struct tag {
    char *name;
    pFile *files;
    int fileCount;
    bool removed;
};

struct hash {
    pFile *everyFile;
    pTag *everyTag;
};

// Inicializa o acervo
pHash initHash() {
    pHash archive = malloc(sizeof(struct hash));
    archive->everyFile = calloc (filesMAX, sizeof(pFile));
    archive->everyTag = calloc (tagsMAX, sizeof(pTag));

    return archive;
}

// Libera a memória do acervo
void freeHash(pHash archive) {
    // Libera cada posição do hash de arquivos
    for (int i = 0; i < filesMAX; i++) {
        if (archive->everyFile[i] != 0) {
            free(archive->everyFile[i]->name);
            free(archive->everyFile[i]->description);
            free(archive->everyFile[i]->tags);
        }
        free(archive->everyFile[i]);
    }

    // Libera cada posição do hash de tags
    for (int i = 0; i < tagsMAX; i++) {
        if (archive->everyTag[i] != 0) {
            free(archive->everyTag[i]->name);
            free(archive->everyTag[i]->files);
        }
        free(archive->everyTag[i]);
    }

    free(archive->everyFile);
    free(archive->everyTag);
    free(archive);
}

// Soluciona colisões de arquivos com endereçamento aberto
int fileColisionSolver(pHash archive, char *key, int index) {
    // Se o vetor chegar ao final, volta do início
    if (index >= filesMAX) {
        index = fileColisionSolver(archive, key, 0);
    }
    // Verifica se a posição não é nula. Se não, verifica se é o mesmo elemento
    if (archive->everyFile[index] != 0) {
        if (strcmp(key, archive->everyFile[index]->name) != 0) {
            index = fileColisionSolver(archive, key, index + 1);
        }
    }

    return index;
}

// Soluciona colisões de tags com endereçamento aberto
int tagColisionSolver(pHash archive, char *key, int index) {
    // Se o vetor chegar ao final, volta do início
    if (index >= tagsMAX) {
        index = tagColisionSolver(archive, key, 0);
    }
    // Verifica se a posição não é nula. Se não, verifica se é o mesmo elemento
    if (archive->everyTag[index] != 0) {
        if (strcmp(key, archive->everyTag[index]->name) != 0) {
            index = tagColisionSolver(archive, key, index + 1);
        }
    }

    return index;
}

// Verifica se um índice indica um arquivo ou uma tag, para resolver colisões
int colisionSolver(pHash archive, char *key, int index, bool isFile) {
    if (isFile == true) {
        index = fileColisionSolver(archive, key, index);
    } else {
        index = tagColisionSolver(archive, key, index);
    }

    return index;
}

// Gera um índice para a tabela hash
int hash(char *key, int M, pHash archive, bool isFile) {
    int index = 0;
    for (int i = 0; i < strlen(key); i++) {
        index = (((873 * index) + key[i]) % M);
    }

    // Resolve colisões
    index = colisionSolver(archive, key, index, isFile);

    return index;
}

// Retorna um arquivo específico a partir de seu nome, caso ele exista no sistema
pFile searchFile(pHash archive, char *searchedName) {
    pFile searchedFile = NULL;
    if (archive->everyFile[hash(searchedName, filesMAX, archive, true)] != 0) {
        searchedFile = archive->everyFile[hash(searchedName, filesMAX, archive, true)];
    }

    return searchedFile;
}

// Retorna uma tag específica a partir de seu nome, caso ela exista no sistema
pTag searchTag(pHash archive, char *searchedName) {
    pTag searchedTag = NULL;
    if (archive->everyTag[hash(searchedName, tagsMAX, archive, false)] != 0) {
        searchedTag = archive->everyTag[hash(searchedName, tagsMAX, archive, false)];
    }

    return searchedTag;
}

// Insere thisFile no vetor de arquivos de thisTag de forma ordenada, baseada na inserção
void sortTags(pHash archive, pFile thisFile, pTag thisTag) {
    int i;
    for (i = thisTag->fileCount - 1; (i >= 0) && (thisTag->files[i]->insertionOrder > thisFile->insertionOrder); i--) {
        thisTag->files[i + 1] = thisTag->files[i];
    }
    thisTag->files[i + 1] = thisFile;
    thisTag->fileCount++;
}

// Adiciona uma tag a um arquivo
void addTag(pHash archive, char *fileName) {
    char *newTag = malloc(nameMAX * sizeof(char));
    scanf(" %s", newTag);

    pFile thisFile = searchFile(archive, fileName);
    pTag thisTag = searchTag(archive, newTag);

    // Cria a tag caso ela não exista
    if (thisTag == NULL) {
        // Inicializa a nova tag
        thisTag = malloc(sizeof(struct tag));
        thisTag->name = malloc(nameMAX * sizeof(char));
        thisTag->files = malloc(filesMAX * sizeof(pFile));
        strcpy(thisTag->name, newTag);
        thisTag->fileCount = 0;
        thisTag->removed = false;
        archive->everyTag[hash(thisTag->name, tagsMAX, archive, false)] = thisTag;

    // Se ela existe, mas foi removida, "desremove" ela
    } else if (thisTag->removed == true) {
        thisTag->removed = false;
    }
    
    // Insere thisFile no vetor de arquivos de thisTag
    sortTags(archive, thisFile, thisTag);
    
    thisFile->tags[thisFile->tagCount] = thisTag;
    thisFile->tagCount++;

    free(newTag);
}

// Adiciona um arquivo ao acervo
void addFile(pHash archive, char *newFileName, int insertions) {
    int N;
    
    // Inicializa o novo arquivo
    pFile newFile = malloc(sizeof(struct file));
    newFile->name = malloc(nameMAX * sizeof(char));
    newFile->tags = malloc(tagsMAX * sizeof(pTag));
    newFile->description = malloc(descriptionMAX * sizeof(char));
    newFile->tagCount = 0;
    newFile->insertionOrder = insertions;
    newFile->removed = false;

    strcpy(newFile->name, newFileName);
    scanf(" %[^\n]", newFile->description);

   // Verifica se o arquivo que está sendo inserido já existe no sistema (já foi removido em algum momento)
   if (archive->everyFile[hash(newFile->name, filesMAX, archive, true)] != 0) {
        archive->everyFile[hash(newFile->name, filesMAX, archive, true)]->removed = false;
        // Adiciona N tags ao novo arquivo
        scanf(" %d", &N);
        while (archive->everyFile[hash(newFile->name, filesMAX, archive, true)]->tagCount != N) {
            addTag(archive, archive->everyFile[hash(newFile->name, filesMAX, archive, true)]->name);
        }
        free(newFile->name);
        free(newFile->tags);
        free(newFile->description);
        free(newFile);
    } else {
        archive->everyFile[hash(newFile->name, filesMAX, archive, true)] = newFile;
        // Adiciona N tags ao novo arquivo
        scanf(" %d", &N);
        while (newFile->tagCount != N) {
            addTag(archive, newFileName);
        }
    }
}

// Remove um arquivo de uma lista de arquivos da struct tag
void removeFileFromList(pHash archive, pTag thisTag, pFile thisFile) {
    // Remove thisFile da lista de arquivos da tag
    for (int i = 0; i < thisTag->fileCount; i++) {
        if (thisTag->files[i] == thisFile) {
            for (int position = i; position < thisTag->fileCount - 1; position++) {
                thisTag->files[position] = thisTag->files[position + 1];
            }
            thisTag->fileCount--;
            break;
        }
    }
}

// Remove uma tag de um arquivo
void removeTag(pHash archive, char *fileName, char *targetName) {
    pFile thisFile = searchFile(archive, fileName);
    pTag thisTag = searchTag(archive, targetName);

    // Retira thisTag da lista de tags do arquivo
    for (int i = 0; i < thisFile->tagCount; i++) {
        if (thisFile->tags[i] == thisTag) {
            for (int position = i; position < thisFile->tagCount - 1; position++) {
                thisFile->tags[position] = thisFile->tags[position + 1];
            }
            thisFile->tagCount--;
            break;
        }
    }
    
    // Remove thisFile da lista de arquivos da tag
    removeFileFromList(archive, thisTag, thisFile);
    // Verifica se o vetor de arquivos da tag atual ficou vazio e, se sim, apaga a tag do hash
    if (thisTag->fileCount == 0) {
        archive->everyTag[hash(thisTag->name, tagsMAX, archive, false)]->removed = true;
    }
}

// Remove um arquivo do acervo
void removeFile(pHash archive, char *targetName) {
    pFile thisFile = searchFile(archive, targetName);
    pTag currentTag;
    
    // Retira thisFile da lista de arquivos de cada tag dele
    for (int i = 0; i < thisFile->tagCount; i++) {
        currentTag = searchTag(archive, thisFile->tags[i]->name);
        removeFileFromList(archive, currentTag, thisFile);
        // Verifica se o vetor de arquivos da tag atual ficou vazio e, se sim, apaga a tag do hash
        if (currentTag->fileCount == 0) {
            archive->everyTag[hash(currentTag->name, tagsMAX, archive, false)]->removed = true;
        }
    }
    
    // Remove o arquivo do hash
    archive->everyFile[hash(targetName, filesMAX, archive, true)]->removed = true;
}

// Altera o nome e a descrição de um arquivo
void editFile(pHash archive, char *targetName) {
    pFile oldVersion = searchFile(archive, targetName);
    char *newName = malloc(nameMAX * sizeof(char));
    scanf(" %s", newName);

    // Verifica se apenas a descrição irá alterar
    if (strcmp(newName, oldVersion->name) == 0) {
        scanf(" %[^\n]", oldVersion->description);
    } else {
        // Cria a nova versão do arquivo, copiando os seus dados
        pFile newVersion = malloc(sizeof(struct file));
        newVersion->name = malloc(nameMAX * sizeof(char));
        newVersion->description = malloc(descriptionMAX * sizeof(char));
        newVersion->tags = malloc(tagsMAX * sizeof(pTag));
        newVersion->insertionOrder = oldVersion->insertionOrder;
        newVersion->tagCount = oldVersion->tagCount;
        newVersion->removed = false;
        strcpy(newVersion->name, newName);
        scanf(" %[^\n]", newVersion->description);
    
        // Para cada tag de oldVersion:
        for (int i = 0; i < oldVersion->tagCount; i++) {
            newVersion->tags[i] = oldVersion->tags[i];                          // Adiciona-a a lista de tags de newVersion
            removeFileFromList(archive, oldVersion->tags[i], oldVersion);       // Remove oldVersion da lista de arquivos dela
            sortTags(archive, newVersion, newVersion->tags[i]);                 // Insere newVersion na lista de arquivos dela
        }
    
        archive->everyFile[hash(oldVersion->name, filesMAX, archive, true)]->removed = true;
        archive->everyFile[hash(newVersion->name, filesMAX, archive, true)] = newVersion;
    }
    free(newName);
}

// Gerencia as operações de adição
int addOperations(pHash archive, char *commandPtII, int insertions) {
    char *newFileName = malloc(nameMAX * sizeof(char));
    scanf(" %s", newFileName);
    
    if (strcmp(commandPtII, "ARQUIVO") == 0) {
        addFile(archive, newFileName, insertions);
        insertions++;
        
    } else if (strcmp(commandPtII, "TAG") == 0) {
        addTag(archive, newFileName);
    }

    free(newFileName);
    return insertions;
}

// Gerencia as operações de busca
void searchOperations(pHash archive, char *commandPtII) {
    char *searchedName = malloc(nameMAX * sizeof(char));
    scanf(" %s", searchedName);

    if (strcmp(commandPtII, "ARQUIVO") == 0) {
        printf("Acessando arquivo: %s\n", searchedName);
        pFile thisFile = searchFile(archive, searchedName);

        if (thisFile != NULL && thisFile->removed == false) {
            printf("Descrição: %s\n", thisFile->description);
            printf("Tags: ");
            for (int i = 0; i < thisFile->tagCount; i++) {
                printf("%s ", thisFile->tags[i]->name);
            }
            printf("\n");
        } else {
            printf("Arquivo %s não existe.\n", searchedName);
        }

    } else if (strcmp(commandPtII, "TAG") == 0) {
        printf("Busca por tag: %s\n", searchedName);
        pTag thisTag = searchTag(archive, searchedName);

        if (thisTag != NULL && thisTag->removed == false) {
            for (int i = 0; i < thisTag->fileCount; i++) {
                printf("%s\n", thisTag->files[i]->name);
            }
        } else {
            printf("0 resultados encontrados.\n");
        }
    }  

    free(searchedName);
}

// Gerencia as operações de remoção
void removeOperations(pHash archive, char *commandPtII, char *targetName) {
    if (strcmp(commandPtII, "ARQUIVO") == 0) {
        removeFile(archive, targetName);

    } else if (strcmp(commandPtII, "TAG") == 0) {
        char *tagName = malloc(nameMAX * sizeof(char));
        scanf(" %s", tagName);
        removeTag(archive, targetName, tagName);
        free(tagName);
    }
}

// Executa o código
int main() {
    char *command = malloc(10 * sizeof(char));
    char *commandPtII = malloc(15 * sizeof(char));
    char *targetName = malloc(nameMAX * sizeof(char));
    pHash archive = initHash();
    int insertions = 0;
    
    scanf(" %s", command);
    while(strcmp(command, "ENCERRAR") != 0) {
        scanf(" %s", commandPtII);
        
        if (strcmp(command, "INSERIR") == 0) {
            insertions = addOperations(archive, commandPtII, insertions);

        } else if (strcmp(command, "BUSCAR") == 0) {
            searchOperations(archive, commandPtII);
            printf("----------\n");

        } else if (strcmp(command, "ALTERAR") == 0) {
            scanf(" %s", targetName);
            editFile(archive, targetName);

        } else if (strcmp(command, "REMOVER") == 0) {
            scanf(" %s", targetName);
            removeOperations(archive, commandPtII, targetName);
        }

        scanf(" %s", command);
    }

    freeHash(archive);
    free(targetName);
    free(commandPtII);
    free(command);
    return 0;
}