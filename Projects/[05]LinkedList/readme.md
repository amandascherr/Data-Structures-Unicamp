Esta tarefa consiste em manipular a ordem de ataque de heróis representada por um deque implementado com lista ligada. O programa deve permitir inserções e remoções nas duas extremidades, inserções em posições arbitrárias e aplicar feitiços mágicos que alteram a ordem dos heróis (como inverter prefixos e inverter sufixos).
## Critérios
Nesta atividade, é necessário:\
Utilizar lista ligada (simples, dupla, circular, com cabeça, etc.) como estrutura primária para representar o deque de heróis (isto é, não é permitido implementar o deque como vetor/array).

# Descrição

Você foi contratado por uma grande desenvolvedora de jogos para trabalhar no novo RPG tático Deque & Dragons.

Nesse jogo, os heróis não atacam em uma ordem fixa. Eles se organizam em um deque de combate, onde podem entrar e sair tanto pelo início quanto pelo fim. Esse tipo de estrutura é chamado de deque (double-ended queue): uma estrutura que permite inserir e remover elementos em ambas as extremidades, de forma eficiente, isto é, em O(1).

No campo de batalha, a ordem cicla naturalmente: sempre que um herói realiza sua ação, ele retorna para o fim do deque. Fora esse ciclo natural, apenas os feitiços mágicos conseguem reordenar de fato o deque, tornando cada batalha imprevisível.

Seu desafio é programar esse deque de heróis, garantindo que a ordem de ataque seja sempre atualizada corretamente, a cada comando recebido. O estúdio quer que o sistema seja eficiente, já que batalhas com muitos personagens devem rodar sem atrasos.

Assim, você foi contratado para escrever o módulo responsável pela ordem de ataque. Internamente, essa ordem deve ser representada por uma lista ligada (usando a versão da sua escolha), que permita manipulações rápidas nas duas pontas e também nas operações especiais de reordenação.

Para facilitar tanto os testes quanto o desenvolvimento do módulo, o estúdio pediu que a ordem de ataque atual seja impressa automaticamente após cada comando executado.
# Operações
    convocar-frente NOME
  Insere o herói NOME no início do deque.\
  Saída: NOME entrou no inicio

    convocar-fim NOME
  Insere o herói NOME no fim do deque.\
  Saída: NOME entrou no fim

    posicionar NOME P
  Insere o herói NOME na posição P (índice iniciando em 0).\
  Saída: NOME inserido na posicao P

    puxar-frente
  Remove o herói do início.\
  Saída: NOME removido do inicio

    puxar-tras
  Remove o herói do fim.\
  Saída: NOME removido do fim

    remover P
  Remove o herói na posição P (índice iniciado em 0).\
  Saída: NOME removido da posicao P

    ataque
  Remove o herói do início e o coloca no fim (simulando que ele atacou e voltou para o final da ordem).\
  Saída: NOME atacou e foi para o fim
  
    caos K
  Inverte a ordem dos K primeiros heróis do deque de heróis.\
  Saída: caos A -> B, onde A é o trecho antes e B o trecho depois.
  
    confusao K
  Inverte a ordem dos K últimos heróis do deque de heróis.\
  Saída: confusao A -> B, onde A é o trecho antes e B o trecho depois.

    sair
  Encerra a execução do programa, liberando toda a memória alocada.

## Entrada

A entrada consiste em uma sequência de comandos, um por linha, processados na ordem em que aparecem.

1. Os índices fornecidos são iniciados em 0.
2. Os nomes dos heróis são strings sem espaços, com até 50 caracteres.
3. Para os comandos com parâmetros numéricos (posicionar, remover, caos, confusao), pode-se assumir que os valores estarão sempre válidos conforme o estado atual da ordem de ataque.
4. Em particular, nos feitiços, caos K e confusao K aparecem apenas com K ≥ 2.
5. A entrada termina em sair.

## Saída

A cada comando executado, o programa deve imprimir exatamente as mensagens especificadas no enunciado.


## Regras de formatação

  Não devem existir espaços a mais no final de cada linha.
    A ordem de ataque é inicialmente vazia. Portanto, no início ou após todas as remoções, deve ser exibido apenas:
        ordem de ataque
