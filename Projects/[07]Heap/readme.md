# Agenda
Esta tarefa consiste em organizar atividade em uma agenda seguindo a prioridade de cada atividade. O programa deve ser implementado utilizando a estrutura heap como uma fila de prioridades, permitindo operações de inserção de atividade, remoção de maior e menor prioridade, e alteração de prioridade.
## Critérios

Nesta tarefa, é necessário:

  1.Utilizar heap binário como estrutura primária para implementação das filas de prioridades.\
  2.Utilizar o ID para acessar rapidamente as atividades e permitir a alteração de prioridade, isto é, não é permitido fazer uma busca linear na fila de prioridades para encontrar a atividade.

# Descrição

Os poucos meses de férias passam depressa. Por isso, dando prioridade para curtir o máximo possível, você decide organizar uma agenda de atividades novas para aproveitar. Junto de melhores amigos e amigas, vocês sugerem todos os dias novas opções de atividades e estimam uma duração em minutos para a mesma. Além disso, atribuem um valor de prioridade, onde valores maiores indicam atividades que desejam realizar primeiro.

A partir da lista de atividades, começam a montar uma agenda para seu primeiro dia de férias. A agenda tem 10 horas no total (600 minutos). Primeiro adicionam à agenda as atividades de maior prioridade enquanto houver tempo livre para encaixar a próxima atividade de maior prioridade. No final, se ainda há tempo livre na agenda, então, para aproveitar cada minuto, decidem preencher o resto do dia adicionando atividades com a menor duração até não ser mais possível adicionar atividades.

Conforme novas ideias surgem, rapidamente percebem que a lista está muito grande e se torna difícil manter a ordem dos itens de maior prioridade e dos itens com menor duração. Com isso, você decide criar um programa para administrar uma agenda, na qual pode adicionar o nome da atividade, sua duração e sua prioridade. Para identificar facilmente a atividade, você também decide atribuir um número inteiro único para identificar cada uma (ID).

O seu programa deve permitir adicionar novas atividades e, quando desejado, organizar a agenda para o dia seguinte removendo-as da lista e imprimindo a agenda do dia no terminal. Atividades com prioridade maiores devem ser adicionadas primeiro na agenda do próximo dia, porém se não houver tempo o suficiente restante na agenda para adicionar a próxima atividade de maior prioridade, deve-se encaixar as de menor duração até não ser mais possível (não há mais tempo suficiente restante na agenda, ou a lista de atividade acabou).

## Regras da Fila de Prioridade

Como nossa lista de atividades pode ficar bem longa, é importante que a remoção seja eficiente, tanto para atividade de maior prioridade, quanto para de menor duração. Para isso, sua implementação deve conter duas estruturas heap que guardam as atividades:

  1. Um heap de máximo ordenado pela prioridade;
  2. Um heap de mínimo ordenado pela duração.

Como cada atividade possui um valor inteiro como identificador único, pode-se criar um vetor auxiliar para cada heap no qual o valor armazenado no índice i indica qual a posição da atividade de ID i em um dos heaps. A Figura abaixo apresenta um exemplo para o caso do heap de máximo. É apresentada a estrutura em árvore do heap, assim como sua organização em vetor e o vetor auxiliar de índices para acesso rápido aos elementos.

Por exemplo, se formos modificar a prioridade da atividade com ID 4, acessamos a posição 4 do vetor de índices, o qual indica que a atividade está na posição 2 do heap. Então podemos acessar a posição 2 do heap, modificar o valor de prioridade e por fim fazer o elemento subir ou descer no heap, respectivamente, se a prioridade aumentar ou diminuir (já que no exemplo temos um heap de máximo). É importante lembrar que todas as operações que modificam a sequência dos elementos no heap devem também modificar os valores no vetor de índices.

Com o vetor de índices podemos realizar modificações nos dois heaps de forma eficiente.

### Desempate de Ordenação no Heap

#### Heap de Máximo por prioridade:
Caso duas atividades tenham o mesmo valor de prioridade, deve-se considerar a com maior duração para ser escolhida primeiro (subir no heap). Caso o empate persista, as atividades de ID menor devem ser escolhidas primeiro.

#### Heap de Mínimo por duração:
Caso duas atividades tenham o mesmo valor de duração, deve-se considerar a com maior prioridade para ser escolhida primeiro (subir no heap). Caso o empate persista, as atividades de ID menor devem ser escolhidas primeiro.

# Entrada

A primeira linha da entrada possui um valor inteiro M que indica o número máximo de atividades que podem ser adicionadas à fila de prioridade. As demais linhas consistem em uma sequência de comandos, um por linha. Cada linha possui um comando seguido dos dados necessário para o comando:

    atividade ID P D
    
  Deve adicionar, à fila de prioridade, uma nova atividade com identificador ID (1 <= ID <=M), prioridade P (0<= P <= 100, e duração D (1<= D <= 700) em minutos.\
  A linha seguinte a este comando possui o nome da atividade que pode conter espaços e têm no máximo 150 caracteres.
    
    agenda
  Deve imprimir a agenda do próximo dia. As atividades devem ser removidas da fila de prioridade para formar a agenda do dia e, então, deve-se imprimir a agenda.
    
    altera ID X
  Modifica a prioridade da atividade com identificação ID. X é um valor inteiro que deve ser acrescido à prioridade da atividade.
    
    encerrar
  Finaliza o programa, imprimindo a quantidade de atividade que não foram realizadas e libera a memória.

# Saída

Após a adição de uma nova atividade deve-se imprimir NOME adicionada - Atividades futuras: K, onde NOME é o nome da atividade e K é a quantidade total de atividades na fila.

O comando agenda deve imprimir --AGENDA--, seguido das atividades da agenda, uma por linha, e por fim ---------- (10 caracteres -). Cada atividade deve estar no formato (ID) NOME - Prioridade P - Estimativa: D minutos. Onde ID é o identificador da atividade, NOME é seu nome, P sua prioridade, e D é a duração. As atividades devem estar na sequência em que foram removidas da fila.

Após alterar a prioridade de uma atividade deve ser impresso Alterado NOME P -> P’, onde NOME é o nome da atividade modificada, P é a prioridade antes da alteração, e P’ é a prioridade após a modificação.

Ao encerrar o programa deve ser impresso o número R de atividades que restaram na fila de prioridades na forma Restaram R atividades.

# Exemplo
### Entrada:

5\
atividade 1 10 500\
Passear na Praia\
atividade 2 8 200\
Jogar Volei\
atividade 3 6 60\
Jogar Boliche\
atividade 4 1 80\
Aprender Violao\
agenda\
encerrar

### Saída:

Passear na Praia adicionada - Atividades futuras: 1\
Jogar Volei adicionada - Atividades futuras: 2\
Jogar Boliche adicionada - Atividades futuras: 3\
Aprender Violao adicionada - Atividades futuras: 4\
--AGENDA--\
(1) Passear na Praia - Prioridade 10 - Estimativa: 500 minutos\
(3) Jogar Boliche - Prioridade 6 - Estimativa: 60 minutos\
\----------\
Restaram 2 atividades
