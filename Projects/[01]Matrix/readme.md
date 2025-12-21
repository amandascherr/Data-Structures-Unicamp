# Matriz
Esta tarefa consiste em manipular uma matriz dinâmica, adicionando ou removendo linhas e colunas, e adaptando a memória alocada para ela de acordo com essas operações.

## Critérios

Nesta atividade, é necessário:

  1. Utilizar alocação dinâmica para representar a matriz.
  2. Ajustar a quantidade de memória alocada para a matriz principal da tarefa conforme as regras do enunciado, e não apenas alocar uma matriz grande desde o início.
  3. Alocar a matriz como vetor de vetores, e não como um único vetor.
  4. Com exceção das funções malloc() e free(), está proibido o uso de outras funções da biblioteca stdlib.h.

# Descrição

Seu amigo Tobias quer desenvolver um jogo. A ideia é que o jogo tenha vários personagens, que aprendem habilidades cada vez mais poderosas durante sua aventura. Para haver equilíbrio entre os personagens, todos devem ter a mesma quantidade de habilidades. Porém, Tobias está sempre mudando de ideia quanto à quantidade de personagens e habilidades, o que deixou suas anotações uma bagunça. Assim, ele pediu ajuda a você para criar um programa que organize essas ideias em uma matriz, em que cada linha representa um personagem, com o poder de suas habilidades em ordem crescente.

Querendo exibir suas habilidades de programação para que Tobias te contrate para o resto do projeto, você decidiu implementar uma matriz dinâmica que guarda essas informações sem desperdiçar muita memória. Assim, seu programa deverá ter uma matriz M x N alocada, e realizar as seguintes operações:

    adicionar personagem (AP)
  Adiciona uma nova linha como última da matriz. Se não houver espaço suficiente alocado na matriz atual, a quantidade de linhas alocadas deve ser dobrada;
  
    adicionar habilidades (AH)
  Adiciona um novo elemento a cada linha, mantendo a ordem crescente em cada uma. Se não houver espaço suficiente alocado na matriz atual, a quantidade de colunas alocadas também deve ser dobrada;
    
    remover personagem (RP)
  Remove uma linha da matriz, fazendo com que as linhas seguintes sejam movidas para o índice anterior. Se essa remoção faz com que apenas M/4 linhas alocadas ou menos estejam ocupadas, a matriz deve ser realocada com apenas M/2 linhas;
    
    remover habilidades (RH)
  Remove uma coluna da matriz, fazendo com que as linhas seguintes sejam movidas para o índice anterior. Se essa remoção faz com que apenas N/4 colunas alocadas ou menos estejam ocupadas, a matriz deve ser realocada com apenas N/2 colunas;
    
    visualizar (V)
  Imprime a matriz atual;
    
    sair (S)
  Encerra a execução.

Toda vez que uma operação alterar o tamanho alocado para a matriz, o programa também deve imprimir uma mensagem informando essa alteração.
# Entrada

A primeira linha da entrada contem dois números inteiros M e N, que são a quantidade inicial de linhas e colunas alocadas para a matriz, respectivamente. As M linhas seguintes contém os N valores iniciais de cada linha da matriz, separados por espaço. Todos os valores são inteiros. As próximas linhas contêm a abreviação de uma das operações a ser executada pelo programa, seguidas dos dados necessários para sua execução:

  1. A abreviação AP é seguida dos x valores da linha a ser adicionada na matriz. Essa quantidade x sempre será igual à quantidade de colunas da matriz atual;
  2. Da mesma forma, a abreviação AH é seguida dos y valores a serem adicionados na matriz. Essa quantidade y sempre será igual à quantidade de linhas da matriz atual;
  3. As abreviações RP e RH são seguidas de um único número inteiro, indicando o índice da linha ou coluna a ser removido da matriz. Esse índice sempre será um índice válido;
  4. As letras V e S não são seguidas de nenhum dado.

# Saída

Após executar uma operação de adição ou remoção que levou a mudar o espaço alocado para a matriz, devem ser impressas duas mensagens, em linhas diferentes. A primeira linha deve ser: Tamanho da matriz: m x n, onde m x n são as ocupações da matriz (linhas e colunas ocupadas após a última inserção). A segunda linha deve dizer: Memória realocada: M x N -> M' x N', em que M x N são as antigas dimensões alocadas para a matriz, e M' x N' são as novas.

A cada operação visualizar (V), deve ser impresso uma linha com o cabeçalho MATRIZ ATUAL:, e nas próximas linhas os valores atuais da matriz, com os valores de cada linha separados por espaço.

# Exemplos

### Entrada:

3 3\
5 8 10\
3 7 12\
1 3 15\
AP 2 5 11\
RH 0\
V\
S

### Saída:

Tamanho da matriz: 4 x 3\
Memória realocada: 3 x 3 -> 6 x 3\
MATRIZ ATUAL:\
8 10\
7 12\
3 15\
5 11
