# Maximum Weight Clique Problem  
Esta tarefa consiste em implementar um programa em que, dada uma rede de influenciadores, encontre o valor total da influência máxima que um grupo coeso pode atingir.

## Critérios
Nesta tarefa, é necessário:

1. Utilizar lista de adjacência ou matriz de adjacência para representar o grafo.
2. Utilizar backtracking para resolver o problema.

# Descrição

O Rancho do Zeferino Vaz é um dos maiores fenômenos midiáticos do ano: um reality show global filmado nas dependências da Unicamp, onde influenciadores digitais são confinados para competir por visibilidade, engajamento e, claro, prêmios milionários.

A organização do evento está em busca dos próximos participantes ideais — não apenas populares, mas que formem um grupo coeso e de alto impacto. Para isso, contratou sua equipe para analisar uma rede de colaborações entre criadores de conteúdo e identificar o conjunto mais influente possível.

Nessa rede:

- Cada influenciador é representado por um vértice, identificado por um número inteiro de 0 a N-1;
- Cada influenciador possui um índice de influência (um valor inteiro ≥ 1), que quantifica seu alcance e poder de persuasão;
- Uma aresta entre dois vértices indica que os influenciadores colaboram diretamente — por exemplo, fazem lives juntas, marcam uns aos outros ou compartilham audiência.
- Seu objetivo é encontrar um grupo coeso, ou seja, um conjunto de influenciadores em que todos colaboram entre si (par a par), e cuja soma dos índices de influência seja a maior possível. Esse grupo representa o núcleo ideal para integrar a próxima edição do Rancho.

Este problema pode ser visto como uma busca por um subconjunto de vértices ligados par a par (um clique) que maximiza uma função objetivo (soma dos pesos).

# Entrada
A entrada do programa possui o seguinte formato:

A primeira linha contém dois inteiros N e M, onde:

N é o número de influenciadores (vértices), com 1 ≤ N ≤ 200;\
M é o número de relações (arestas), com 0 ≤ M ≤ 12000.\
A segunda linha contém N inteiros w₀, w₁, ..., wₙ₋₁, onde wᵢ ≥ 1 representa o índice de influência do influenciador i.\

As próximas M linhas descrevem as colaborações. Cada linha contém dois inteiros u e v (0 ≤ u,v < N, u ≠ v), indicando que os influenciadores u e v colaboram diretamente.

# Saída
O programa deve imprimir duas linhas na saída padrão:

A primeira linha contém um único inteiro: o peso total máximo do clique.\
A segunda linha contém os IDs dos vértices que compõem um clique ótimo qualquer, separados por espaços, em ordem crescente.

# Exemplo
### Entrada:

5 2\
8 12 5 20 9\
0 1\
3 4

### Saída:

29\
3 4
