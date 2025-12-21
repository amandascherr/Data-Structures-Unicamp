# Mapa de Build: DFS e BFS em um Makefile

Esta tarefa consiste em analisar um Makefile como um grafo de dependências para prever e explicar o processo de build. O programa deve gerar uma ordenação topológica sequencial usando DFS e a distância mínima em arestas do alvo principal até cada arquivo alcançável usando BFS. Sendo necessário, também, verificar a existência de ciclos durante a DFS.
## Critérios

Nesta tarefa, é necessário:

  1. Utilizar DFS para gerar a ordenação topológica e identificar ciclos.
  2. Utilizar BFS (a partir do alvo da primeira linha) para calcular a distância mínima até cada arquivo alcançável.

# Descrição

Você é a pessoa mais nova na equipe de engenharia de software que trabalha em um projeto legado gigantesco e crítico. Esse projeto tem milhões de linhas de código, espalhadas por milhares de arquivos. O coração que faz tudo isso funcionar é um arquivo muito grande e antigo: o Makefile. Este Makefile é uma teia de aranha de dependências, regras e scripts acumulados ao longo de décadas. Ninguém na equipe atual entende completamente como ele funciona, mas ele funciona.

O problema é que o build é lento. Uma compilação completa do projeto (um make all) pode levar horas. A produtividade da equipe inteira está sendo prejudicada por esse gargalo e ninguém sabe ao certo qual é a profundidade das dependências nem em que ordem as coisas realmente acontecem. Sem medir essa profundidade (a distância mínima de cada arquivo até o alvo), não dá para estimar o alcance de uma mudança, identificar gargalos que causam recompilações em cascata e priorizar quais módulos revisar primeiro.

Sua gerente decidiu iniciar uma força-tarefa para otimizar o Makefile. Como uma modificação “às cegas” é muito arriscada, seu primeiro passo não é executar o make, mas sim construir uma ferramenta de análise.

Ao analisar o formato das regras do Makefile, você percebe que as dependências podem ser representadas por um grafo direcionado. Cada nome de arquivo funciona como um vértice e as regras de dependência definem as arestas.

Considere, por exemplo, o seguinte trecho:

    all: app tests
    app: main.o core.o
    tests: test_run.o core.o

    main.o: main.c core.h
    core.o: core.c core.h
    test_run.o: test_run.c core.h
    
  Podemos construir este grafo criando arestas que apontam do alvo para suas dependências:\
  <img width="587" height="564" alt="graph" src="https://github.com/user-attachments/assets/1daafb92-6768-4e2d-8fe6-0ba544aebe9a" />

Enquanto você apresenta essa ideia do grafo para a equipe, um engenheiro sênior levanta um ponto crucial:

“Já que o nosso objetivo é mudar as regras para otimizar, precisamos de uma garantia. Aproveitando essa modelagem em grafo podemos ter uma funcionalidade que verifique se qualquer mudança que fizermos no Makefile não vai acabar gerando dependências circulares. Antes de aplicar qualquer otimização e arriscar quebrar o build, temos que ter certeza de que a nova estrutura é válida.”

Portanto, a equipe concorda: a primeira e mais crucial tarefa da sua ferramenta deve ser detectar esses ciclos. Se um ciclo for encontrado ao tentar analisar um Makefile, qualquer análise se torna inútil. A ferramenta deve parar e reportar o erro imediatamente.
# Análises
Sua ideia é propor duas análises para a equipe, partindo de um alvo raiz do Makefile (como all).
### Dependências Circulares (Tarefa 0)

Durante a DFS usada para a ordenação topológica, se um ciclo for detectado interrompa a execução e imprima exatamente duas linhas:

Erro: dependências circulares entre os arquivos necessários para construir '[alvo]'\
Ciclo: a -> b -> c -> a

Formatação da Linha do Ciclo:
1. Comece pelo vértice que tem o menor valor lexicográfico.
2. Formato: escreva exatamente Ciclo: v1 -> v2 -> ... -> vk -> v1, com um espaço ao redor de -> e sem espaço extra no fim.

### Análise Sequencial (Tarefa 1)

Para depurar e entender o fluxo de construção, a equipe precisa de uma ordem de compilação sequencial e previsível.

Sua Tarefa 1: Gerar uma ordenação topológica de todos os componentes. Esta lista linear representará uma “ordem de execução válida”, garantindo que nenhum alvo seja compilado antes de seus pré-requisitos.

Ao buscar uma forma direta de extrair essa sequência, surge a ideia de percorrer o grafo em profundidade (DFS):

### Distâncias a partir de all (Tarefa 2)

Depois de garantir uma ordem previsível para depurar, o próximo passo é medir em quantos passos mínimos cada arquivo está do alvo final all.

Sua Tarefa 2: calcular a distância mínima (em arestas) de all até cada vértice alcançável, usando BFS a partir de all no grafo com arestas alvo → dependência.

Você percebeu que dá para enxergar isso fazendo uma busca em largura (BFS). Começando em all, você visita o que ele precisa (distância 1), depois o que esses precisam (distância 2), e segue até chegar aos demais níveis. Em seguida, apresente a saída por níveis de distância em ordem crescente e dentro de cada nível, liste os nomes em ordem lexicográfica.

## Regras de desempate

Para padronizar a saída e facilitar a correção automática, adote as seguintes convenções de desempate:

    DFS / ordenação topológica (Tarefa 1): ao percorrer o grafo, visite os vizinhos em ordem lexicográfica.
    BFS / distâncias (Tarefa 2): apresente a saída por níveis de distância em ordem crescente e, dentro de cada nível, liste os nomes em ordem lexicográfica, independentemente da ordem em que foram visitados pela BFS.

# Entrada

A entrada é somente o conjunto de regras de dependência no estilo Makefile, uma por linha, lida até o fim da entrada (EOF).

Cada nome de arquivo/alvo citado nas regras (ex.: core.c, main.o, app) representa vértices. As regras definem arestas direcionadas do alvo para cada dependência (isto é, alvo → dependência; ex.: main.o → core.h).

Formato de cada linha:

    alvo: dep1 dep2 dep3 ...

  1. O alvo principal de build (o “produto final” que queremos construir) será o alvo da primeira linha válida (ex.: all).
  2. Nomes de arquivos/alvos não contêm espaços.
  3. Linhas em branco são permitidas e devem ser ignoradas.
  4. Total de vértices distintos (arquivos/alvos) N: até 5 000
  5. Total de arestas (dependências) M: até 20 000
  6. Nomes: até 15 caracteres.
  7. Tamanho máximo de uma linha do MakeFile: 16800

Saída

O programa deve imprimir exatamente as seguintes linhas, nesta ordem, para o subgrafo alcançável a partir do alvo principal de build (isto é, o alvo da primeira linha válida, que pode ser all ou qualquer outro nome).
1) Ordenação topológica (Tarefa 1)

Imprima exatamente no formato:

    SEQ - <índice>: <nome>

  <índice> começa em 1.
    Use um espaço antes e depois de - e um espaço após :.
    Finalize cada linha com \n.

Exemplo (baseado no enunciado; aqui o alvo principal é all):

SEQ - 1: core.c
SEQ - 2: core.h
SEQ - 3: main.c
SEQ - 4: test_run.c
SEQ - 5: core.o
SEQ - 6: main.o
SEQ - 7: test_run.o
SEQ - 8: app
SEQ - 9: tests
SEQ - 10: all

2) Distâncias a partir do alvo principal (Tarefa 2)

A saída deve listar um item por linha, indicando a distância mínima (em arestas) do alvo principal até cada vértice alcançável.
Imprima as linhas por níveis de distância em ordem crescente e, dentro de cada nível, em ordem alfabética (lexicográfica).

    Distância até <alvo_principal> - <D>: <nome>

  D = 0 para o próprio <alvo_principal>.
    Cada linha termina com \n.
    Vértices não alcançáveis a partir de <alvo_principal> não aparecem.

Exemplo: (no exemplo abaixo, <alvo_principal> = all)

Distância até all - 0: all
Distância até all - 1: app
Distância até all - 1: tests
Distância até all - 2: core.o
Distância até all - 2: main.o
Distância até all - 2: test_run.o
Distância até all - 3: core.c
Distância até all - 3: core.h
Distância até all - 3: main.c
Distância até all - 3: test_run.c

3) Casos de dependência circular (não imprimir Sequência/Distâncias)

Se houver ciclo no subgrafo alcançável do alvo principal, imprima apenas:

Erro: dependências circulares entre os arquivos necessários para construir '<alvo_principal>'

    Ciclo: v1 -> v2 -> ... -> vk -> v1

Regra de impressão do ciclo:

  Comece pelo menor vértice em ordem lexicográfica que pertence ao ciclo.
  Formato: Ciclo: v1 -> v2 -> ... -> vk -> v1 (um espaço ao redor de ->, sem espaço no fim).

# Exemplos
#### Exemplo completo de saída (entrada acíclica)

SEQ - 1: core.c\
SEQ - 2: core.h\
SEQ - 3: main.c\
SEQ - 4: test_run.c\
SEQ - 5: core.o\
SEQ - 6: main.o\
SEQ - 7: test_run.o\
SEQ - 8: app\
SEQ - 9: tests\
SEQ - 10: all\
Distância até all - 0: all\
Distância até all - 1: app\
Distância até all - 1: tests\
Distância até all - 2: core.o\
Distância até all - 2: main.o\
Distância até all - 2: test_run.o\
Distância até all - 3: core.c\
Distância até all - 3: core.h\
Distância até all - 3: main.c\
Distância até all - 3: test_run.c\

#### Exemplo de saída (entrada cíclica)

Erro: dependências circulares entre os arquivos necessários para construir 'all'\
Ciclo: app -> core.h -> core.o -> app
