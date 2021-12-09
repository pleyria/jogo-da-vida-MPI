# Jogo da Vida MPI - Atividade de Programação Concorrente e Distribuída

## Descrição

## Estrutura dos Arquivos
O arquivo `jogo-da-vida.c` contém o código da versão paralela do programa do jogo da vida, desenvolvido com a biblioteca MPI.

É necessário ter o pacote `mpich` instalado.
> No Ubuntu/Linux Mint: $ sudo apt-get install mpich

Para compilar o código usa-se o comando `mpicc jogo-da-vida.c -o jogo-da-vida`.

Para executar o programa compilado, usa-se `mpiexec -np 4 ./jogo-da-vida`. 
> -np: número de processos

## Resultados

