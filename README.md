# Jogo da Vida MPI - Atividade de Programação Concorrente e Distribuída

## Descrição
Implementação de uma versão paralela do jogo da vida em linguagem C, utilizando a biblioteca MPI para execução com multiplos processos de memória distribuída.

É possível escolher o número de processos a serem criados no programa. Os múltiplos processos recebem uma parte do domínio do problema a ser resolvido e trocam mensagens entre si para produzir o resultado global.

## Estrutura dos Arquivos
O arquivo `jogo-da-vida.c` contém o código da versão paralela do programa do jogo da vida, desenvolvido com a biblioteca MPI.

É necessário ter o pacote `mpich` instalado na máquina para fazer a compilação e execução.
> No Ubuntu/Linux Mint: $ sudo apt-get install mpich

Para compilar o código usa-se o comando `mpicc -O3 jogo-da-vida.c -o jogo-da-vida`.
> -O3: Opção de otimização nível 3 para o compilador gcc

Para executar o programa compilado, usa-se `mpiexec -np 4 ./jogo-da-vida`. 
> -np: Número de processos

**Importante**: O número de processos sempre deve ser um a mais do que o número esperado para executar a simulção. Isso porque um dos processos não participa do cálculo do jogo da vida e serve apenas para agregar os resultados parciais produzidos pelos outros processos. Portanto, para executar o programa com, por exemplo, 4 processos em paralelo, é preciso usar o argumento `-np 5`, onde são empregados 5 processos: 4 deles para executar a simulação do jogo da vida e 1 para reunir e exibir os resultados. O menor valor possível para este argumento é `-np 2`, resultando em um único processo responsável por executar o programa e outro para comunicação com o usuário.

## Resultados
Todos os testes foram feitos em uma máquina com as seguintes configurações:

* Processador AMD Ryzen 5 5600X (6 núcleos reais e 12 threads com *Simultaneous Multithreading* - SMT);
* 16 GB de memória RAM DDR4;
* Sistema Operacional Linux Mint 20.2 Cinnamon;
* Compilador GCC versão 9.3.0; e
* Pacote mpich versão 3.2.2-2 build 1.

Os resultados e uma comparação com uma implementação em OpenMP do jogo da vida, em termos de tempos de execução, *speedup* e eficiência, estão disponíveis [nesta tabela](https://docs.google.com/spreadsheets/d/1P5irgyL75FzMT8lc6uDxMPpiI4kLS472xhsHGUy-6-g/edit?usp=sharing).

O vídeo explicativo com apresentação do trabalho e demonstração de funcionamento está disponível [neste link](https://drive.google.com/file/d/1rSTXhJhExcR-xfVIdphZmaSmazEYFAf4/view?usp=sharing).
