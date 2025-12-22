# Acervo Digital
Essa tarefa consiste em gerenciar arquivos de um acervo digital a partir de seus nomes e de marcadores de conteúdo, utilizando de funções de hashing e tabelas de espalhamento para armazenar e acessar os dados de forma eficiente.

## Critérios
Nesta atividade, é necessário:

1. Utilizar tabelas de espalhamento com endereçamento aberto como estrutura primária para guardar os dados da tarefa.
2. Armazenar de forma única todos os dados exceto nomes de arquivos e nomes de tags (salvo por variáveis temporárias usadas apenas para uma operação, se necessário).

# Descrição

O museu Símbolo da Cerquinha contratou você para criar o sistema de gerenciamento de seu novo acervo digital. Esse acervo consiste em diversos arquivos digitais, cada um com um nome único, uma descrição textual, e uma lista de marcadores (tags) que são palavras-chave sobre o conteúdo, para facilitar a organização e busca dos arquivos. O sistema deve ser capaz de inserir e remover arquivos do acervo, e de alterar dados de um arquivo, incluindo a inserção e remoção de tags dele. Toda nova tag é inserida no final da lista de tags do arquivo.

O sistema também deve ser capaz de fazer buscas eficientes por uma tag, listando todos os arquivos marcados com ela, e pelo próprio nome do arquivo, exibindo todos os seus dados.

Além disso, para evitar desperdício de memória, apenas os dados usados como chave de buscas (nome de arquivo e nome de tag) pode ser replicados em mais que uma estrutura de dados. Os outros dados devem ser armazenados de forma única no sistema.

# Entrada
A entrada consiste das operações requisitadas para o sistema, uma por linha.

    INSERIR ARQUIVO
É seguida de três linhas: uma com o nome do arquivo, uma com a descrição do arquivo, e uma com a quantidade de tags do arquivo, seguida das tags do arquivo, separadas por espaço;

    REMOVER ARQUIVO
É seguida do nome do arquivo a ser removido;

    ALTERAR ARQUIVO
É seguida do nome atual do arquivo a ser alterado, e, nas próximas duas linhas, o novo nome e descrição do arquivo;

    INSERIR TAG
É seguida do nome do arquivo e o nome da tag a ser inserida;

    REMOVER TAG
É seguida do nome do arquivo e o nome da tag a ser removida;

    BUSCAR TAG
É seguida do nome da tag

    BUSCAR ARQUIVO
É seguida do nome do arquivo

    ENCERRAR
Termina a execução do programa.

Todas as remoções e alterações serão feitas em arquivos e tags válidos, ou seja, que foram inseridos no sistema por alguma operação anterior. São inseridos no máximo 2000 arquivos durante a execução do programa, e no máximo 30 tags.

Um nome de arquivo ou tag tem no máximo 30 caracteres (sem espaços), e são todos distintos, isto é, não há dois arquivos ou tags com o mesmo nome. Uma descrição de arquivo tem no máximo 100 caracteres, incluindo espaços.

# Saída
A operação BUSCAR TAG deve sempre imprimir um cabeçalho Busca por tag: T, onde T é o nome da tag buscada. Nas linhas seguintes devem ser impressos os nomes dos arquivos marcados com a tag buscada, um por linha, na ordem em que foram inseridos no sistema. Se a tag buscada não existe no sistema, deve ser impressa a mensagem: 0 resultados encontrados.\
A operação BUSCAR ARQUIVO deve sempre imprimir um cabeçalho Acessando arquivo: X, onde X é o nome do arquivo buscado. Nas linhas seguintes devem ser impressas a descrição, e lista de tags do arquivo, com cada tag separada por vírgula, e seguindo a formatação abaixo.\
Descrição: [descrição]\
Tags: [lista de tags]\
Se o arquivo buscado não existe no sistema, deve ser impressa a mensagem: Arquivo X não existe.\
Ao fim de qualquer busca deve ser impresso o rodapé ---------- (10 caracteres -).

# Exemplo

### Entrada:

INSERIR ARQUIVO\
ipes.mp4\
Vista aérea de ipês na cidade de Campinas, 2023.\
4 arvore flores primavera ipe\
INSERIR ARQUIVO\
ipeamarelo.png\
Segunda florada de grande árvore em frente a residência, 2025.\
4 arvore flores primavera ipe\
INSERIR ARQUIVO\
caiu.png\
Árvore caída após tempestade, 2018.\
3 arvore desastrenatural tempestade\
BUSCAR ARQUIVO ipeamarelo.png\
BUSCAR TAG arvore\
REMOVER ARQUIVO ipeamarelo.png\
BUSCAR ARQUIVO ipeamarelo.png\
BUSCAR TAG arvore\
ENCERRAR

### Saída:

Acessando arquivo: ipeamarelo.png\
Descrição: Segunda florada de grande árvore em frente a residência, 2025.\
Tags: arvore flores primavera ipe\
\----------\
Busca por tag: arvore\
ipes.mp4\
ipeamarelo.png\
caiu.png\
\----------\
Acessando arquivo: ipeamarelo.png\
Arquivo ipeamarelo.png não existe.\
\----------\
Busca por tag: arvore\
ipes.mp4\
caiu.png\
\----------
