# DNA Tree
Esta tarefa consiste em manipular uma sequência de DNA representada por uma árvore binária. O programa deve permitir inserções e remoções em posições arbitrárias, bem como a impressão de um trecho do DNA e sua impressão total.

## Critérios
Nesta atividade, é necessário:

1. Utilizar uma árvore binária como descrita abaixo como a estrutura primária para representar o seu sistema de manipulação de DNAs.
2. A árvore deve ser criada de forma balanceada, e deve ser mantida balanceada após cada operação de inserção ou remoção como descrito abaixo.
3. A remoção não pode deixar folhas com 0 caracteres, nem nós com apenas um filho.
4. A inserção não pode deixar folhas com mais de L caracteres na string (excluindo o \0) após o término da inserção.

# Descrição

O ano é 2125. A empresa NDA, líder mundial no estudo de dados genéticos, enfrenta um problema que ameaça sua posição no topo: seu sistema de manipulação de DNA é lento demais, e os rivais estão cada vez mais perto de ultrapassá-la.

É nesse cenário que você entra em cena. Como pesquisador-chefe de algoritmos, sua missão é desenvolver um novo sistema capaz de lidar com enormes sequências de DNA de forma ágil e confiável. Toda sequência de DNA manipulada por este sistema é composta exclusivamente pelos quatro nucleotídeos fundamentais: A (Adenina), T (Timina), C (Citosina) e G (Guanina).

Assim, a solução escolhida foi utilizar árvores binárias, já que elas permitem representar e manipular grandes cadeias de caracteres de forma estruturada, tornando operações como inserção e remoção muito mais rápidas do que em abordagens tradicionais baseadas apenas em strings lineares.

# Regras da Árvore Binária
Sua árvore binária deve seguir rigorosamente as seguintes regras de estrutura e comportamento.

Todos os nós da árvore têm a mesma estrutura base, que deve conter no mínimo:

- Um ponteiro para filho esquerdo,
- Um ponteiro para filho direito,
- Um valor inteiro,
- Uma string (alocada dinamicamente);
- Um campo altura (inteiro) que armazena a altura da subárvore enraizada nesse nó.
Campos adicionais no nó podem ser incluídos se forem úteis para a sua implementação.

Nó folha:

- Não tem filhos (esquerdo e direito são NULL);
- A string armazena um trecho do DNA, com no máximo L caracteres (onde L é fornecido na entrada);
- O valor inteiro indica o comprimento dessa string;
- A altura é 1.

Nó interno:

- Tem exatamente dois filhos (esquerdo e direito não nulos);
- A string deve ser ignorada (pode ser NULL);
- O valor inteiro representa a soma dos tamanhos de todos os caracteres contidos na subárvore esquerda — esse valor é essencial para navegar pela árvore;
- A altura deve ser mantida atualizada como: 1 + max(altura(esquerdo), altura(direito)).
- Note que o valor inteiro não é a soma dos tamanhos alocados das strings na subárvore esquerda, mas sim a soma dos tamanhos utilizados nas strings dessa subárvore.

# Construção Inicial da Árvore
Ao iniciar o programa, você receberá uma sequência de DNA de tamanho T e um limite L (máximo de caracteres por folha). A árvore deve ser construída dividindo a string recursivamente até que cada nó folha contenha no máximo L caracteres.

Essa construção inicial deve ser balanceada, garantindo que a altura da árvore seja mínima. Uma árvore é considerada balanceada nesta tarefa quando é construída de forma que, em cada nó interno, o número de caracteres à esquerda e à direita seja o mais equilibrado possível. Isto é, temos metade dos caracteres em cada lado (considerando a variação de 1 caractere a mais para um dos lados, caso o número total seja ímpar). Isso garante que a altura da árvore seja O(log n), o que é essencial para a eficiência das operações.

Seu desafio é garantir que, ao receber uma sequência de DNA de tamanho T e um limite L (fornecidos na entrada), você a transforme em uma árvore binária em que cada folha contenha no máximo L caracteres, e execute P operações de inserção, remoção e impressão.

# Funcionalidades
Seu programa deve suportar as seguintes operações:

    PRINTALL
Imprime toda a sequência de DNA armazenada na árvore;

    PRINT i j
Imprime os caracteres da posição i até j-1 ( intervalo [i, j) );

  
    INSERT pos c
Insere o nucleotídeo c na posição pos (isto é, para ficar na posição pos na sequência de DNA);

    REMOVE pos
Remove o caractere na posição pos.

## Balanceamento da Árvore
Sempre que uma operação de modificação (INSERT ou REMOVE) for realizada, verifique, ao longo do caminho de volta à raiz, se algum nó viola a condição de balanceamento: se a diferença absoluta entre as alturas das subárvores esquerda e direita for maior que 4, reconstrua a subárvore enraizada nesse nó a partir da sequência de DNA correspondente.

Essa abordagem assegurará que a árvore permaneça eficiente mesmo após múltiplas operações.

# Entrada
A entrada é composta por:


Primeira linha: três inteiros separados por espaço

P: número de operações a serem realizadas;\
T: tamanho da sequência inicial de DNA;\
L: limite máximo de caracteres por nó folha (L ≥ 1);


Segunda linha:

Uma string de exatamente T caracteres, que representam a sequência de DNA que será modificada;\
As próximas P linhas com as operações no formato:

INSERT <pos> <c>\
pos: inteiro (int) — posição de inserção (0-indexada);\
c: caractere (char) — pode ser apenas um dos nucleotídeos A, T, C ou G;\
REMOVE <pos>\
pos: inteiro (int) — posição do caractere a ser removido (0-indexada);\
PRINT <i> <j>\
i: inteiro (int) — índice inicial (inclusivo, 0-indexado);\
j: inteiro (int) — índice final (exclusivo, 0-indexado);\
PRINTALL

# Saída
Toda vez que PRINT for chamado, deve imprimir o cabeçalho Trecho i-j:, onde i é a posição inicial do trecho e j a final;\
Toda vez que PRINTALL for chamado, imprima o resultado desta operação com o cabeçalho: DNA inteiro:\
A última linha da saída do sistema deverá conter a contagem de cada nucleotídeo presente na sequência final de DNA, com o cabeçalho Contagem final: A:quantidade T:quantidade C:quantidade G:quantidade, onde quantidade é o número total de ocorrências de cada letra na cadeia final;

# Exemplo
### Entrada:

5 3 6\
AAG\
INSERT 1 G\
INSERT 0 T\
PRINTALL\
INSERT 2 A\
PRINTALL
### Saída:

DNA inteiro: TAGAG\
DNA inteiro: TAAGAG\
Contagem final: A:3 T:1 C:0 G:2
