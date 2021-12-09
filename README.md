# Jogo da Vida MPI - Atividade de Programação Concorrente e Distribuída

## Descrição
Implementação de uma versão paralela do jogo da vida em linguagem C, utilizando a biblioteca MPI para execução com multiplos processos de memória distribuída.

É possível escolher o número de processos a serem criados no programa. Os múltiplos processos recebem uma parte do domínio do problema a ser resolvido e trocam mensagens entre si para produzir o resultado global.

## Estrutura dos Arquivos
O arquivo `jogo-da-vida.c` contém o código da versão paralela do programa do jogo da vida, desenvolvido com a biblioteca MPI.

É necessário ter o pacote `mpich` instalado.
> No Ubuntu/Linux Mint: $ sudo apt-get install mpich

Para compilar o código usa-se o comando `mpicc -O3 jogo-da-vida.c -o jogo-da-vida`.

Para executar o programa compilado, usa-se `mpiexec -np 4 ./jogo-da-vida`. 
> -np: número de processos

O número de processos sempre deve ser um a mais do que o número esperado para executar a simulção. Isso porque um dos processos não participa do processamento do jogo da vida e serve apenas para agregar os resultados parciais produzidos pelos outros processos. Portanto, para executar o programa como, por exemplo, 4 processos em paralelo, é preciso usar o argumento `-np 5`, onde são empregados 5 processos: 4 deles para executar a simulação do jogo da vida e 1 para reunir e exibir os resultados.

## Resultados

