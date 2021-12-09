/* Programacao Concorrente e Distribuida
 * Entrega 3
 * Jogo da vida em MPI */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ALIVE 1
#define DEAD 0

/* Recebe um subgrid (size+2)xN.
 * Insere uma célula viva na posicao (i, j) se i estiver entre li e lf. */
void insertCell(int** grid, int i, int j, int li, int lf){
	if(i >= li && i <= lf)
		grid[i - li][j] = ALIVE;
}

/* Recebe um subgrid (size+2)xN.
 * Insere um glider a partir da posicao (0, 0) se ela existir no subgrid.
 * Insere um R-pentomino a partir da posicao (10, 30) se ela existir no subgrid. 
 * O intervalo FECHADO de linhas existentes no subgrid é dado por li e lf. */
void startGrid(int** grid, int size, int N, int li, int lf){
	int i, j;
	int lin, col;

	// linha 0 			-> guarda vizinho de cima
	// linha 1...size 	-> conteudo da subtabela
	// linha size+1 	-> guardda vizinho de baixo

	// Inicializa com zeros
	for(i=1; i <= size; i++)
		for(j=0; j<N; j++)
			grid[i][j] = DEAD;

	//GLIDER
	lin = 1, col = 1;
	insertCell(grid, lin  , col+1, li, lf);
	insertCell(grid, lin+1, col+2, li, lf);
	insertCell(grid, lin+2, col  , li, lf);
	insertCell(grid, lin+2, col+1, li, lf);
	insertCell(grid, lin+2, col+2, li, lf);

	//R-pentomino
	lin =10; col = 30;
	insertCell(grid, lin  , col+1, li, lf);
	insertCell(grid, lin  , col+2, li, lf);
	insertCell(grid, lin+1, col  , li, lf);
	insertCell(grid, lin+1, col+1, li, lf);
	insertCell(grid, lin+2, col+1, li, lf);
}

int getNeighbors(int** grid, int i, int j, int N){
	int up, down, left, right, count;

	// Define posicoes vizinhas respeitando os
	// limites do tabuleiro
	up = i == 0 ? N-1 : i-1;
	down = i == N-1 ? 0 : i + 1;
	left = j == 0 ? N - 1: j - 1;
	right = j == N - 1 ? 0 : j+1;

	count = 0;
	count += grid[up][j];
	count += grid[up][right];
	count += grid[i][right];
	count += grid[down][right];
	count += grid[down][j];
	count += grid[down][left];
	count += grid[i][left];
	count += grid[up][left];

	return count;
}

void simulate(int** grid, int** newgrid, int size, int N){
	int i, j, neighbors;

	// percorre apenas a parte pertencente ao processo
	// extremidades superior e inferior contem informacoes de vizinhos
	// usadas apenas na contagem das celulas internas
	for(i = 1; i <= size; i++){
		for(j = 0; j < N; j++){
			// conta vizinhos
			neighbors = getNeighbors(grid, i, j, N);

			// 1) Celula viva com 2 ou 3 vizinhos deve sobreviver
			if(grid[i][j] && (neighbors == 2 || neighbors == 3))
				newgrid[i][j] = ALIVE;

			// 2) Celula morta com 3 vizinhos torna-se viva
			else if(!grid[i][j] && neighbors == 3)
				newgrid[i][j] = ALIVE;

			// 3) Qualquer outro caso
			else{
				// celulas vivas devem morrer
				// celulas mortas continuam mortas
				newgrid[i][j] = DEAD;
			}
		}
	}
}

int countAlive(int** grid, int size, int N){
	int i, j, count;

	count = 0;

	for(i = 1; i <= size; i++)
		for(j = 0; j < N; j++)
			count += grid[i][j];

	return count;
}

int main(int argc, char** argv){
	int i, j;
	// Dimensao do tabuleiro e
	// Numero de geracoes
	int N, G;
	// Numero de processos
	int numProc;
	// ID do processo
	int pid;
	// intervalo de linhas do processo
	int li, lf, size;
	// estruturas do grid
	int **grid, **newgrid;
	// id do processo mestre
	int master;
	// contagem de celulas vivas
	int soma, alive;
	// contagem de tempo
	double start, finish;
	// variaveis de controle do MPI
	MPI_Request req;
	MPI_Status status;

	// Tabuleiro 2048x2048
	N = 2048;
	// 2000 geracoes
	G = 2000;

	// Inscreve os processos na compuitacao do MPI
	MPI_Init(&argc, &argv);

	// obtem numero total de processos e
	// identificador do processo
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	master = numProc - 1;
	numProc--;
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	// o processo com o rank mais alto nao calcula o jogo da vida
	// e serve apenas para reunir os resultados

	if(pid < master){
		// divisao do tabuleiro por processos
		size = N/numProc;		// numero de linhas por processo
		li = pid*size;			// inicio do intervalo
		lf = li + size - 1;		// fim do intervalo
		if(pid == numProc - 1){	// ultimo processo pega o resto
			lf += N % numProc;
			size += N % numProc;
		}

		// matrizes <size+2> linhas e <N> colunas
		// aloca duas linhas a mais para receber valores dos vizinhos (de cima e de baixo)
		// ----
		// linha 0:			vizinho de cima
		// linhas 1...size:	parte atual
		// linha size+1:	vizinho de baixo
		// ----
		grid = (int**) malloc((size+2) * sizeof(int*));		// aloca linhas
		newgrid = (int**) malloc((size+2) * sizeof(int*));
		for(i = 0; i < size+2; i++){						// aloca colunas
			grid[i] = (int*) malloc(N * sizeof(int));
			newgrid[i] = (int*) malloc(N * sizeof(int));
		}

		// inicializa o grid
		startGrid(grid, size, N, li, lf);
	}

	// espera todos os processos criarem e inicializarem as matrizes
	MPI_Barrier(MPI_COMM_WORLD);

	// inicia a contagem do tempo
	if(pid == master)
		start = MPI_Wtime();

	// Realiza a simulacao para G geracoes
	// Alterna a posicao de grid e newgrid nas chamadas
	// (G impar -> resposta final em newgrid)
	// (G par -> resposta final em grid)
	for(i = 0; i < G; i++){
		if(pid < master){
			// processos vizinhos compartilham informacoes das bordas
			// alterna entre grid e newgrid
			if(i % 2 == 0){
				// envia borda inferior do grid para o vizinho de baixo (tag = 2)
				MPI_Isend(grid[size], N, MPI_INT, (pid+1)%numProc, 2, MPI_COMM_WORLD, &req);
				// envia borda superior do grid para o vizinho de cima (tag = 3)
				if(pid == 0)
					MPI_Isend(grid[1], N, MPI_INT, numProc-1, 3, MPI_COMM_WORLD, &req);
				else
					MPI_Isend(grid[1], N, MPI_INT, pid-1, 3, MPI_COMM_WORLD, &req);

				// recebe borda inferior do grid vizinho de cima (tag = 2)
				if(pid == 0)
					MPI_Recv(grid[0], N, MPI_INT, numProc-1, 2, MPI_COMM_WORLD, &status);
				else
					MPI_Recv(grid[0], N, MPI_INT, pid-1, 2, MPI_COMM_WORLD, &status);
				// recebe borda superior do vizinho de baixo (tag = 3)
				MPI_Recv(grid[size+1], N, MPI_INT, (pid+1)%numProc, 3, MPI_COMM_WORLD, &status);

				// calcula a proxima geracao
				simulate(grid, newgrid, size, N);

				// conta as celulas vivas na subtabela
				alive = countAlive(grid, size, N);
			}
			else {
				// envia borda inferior do grid para o vizinho de baixo (tag = 2)
				MPI_Isend(newgrid[size], N, MPI_INT, (pid+1)%numProc, 2, MPI_COMM_WORLD, &req);
				// envia borda superior do grid para o vizinho de cima (tag = 3)
				if(pid == 0)
					MPI_Isend(newgrid[1], N, MPI_INT, numProc-1, 3, MPI_COMM_WORLD, &req);
				else
					MPI_Isend(newgrid[1], N, MPI_INT, pid-1, 3, MPI_COMM_WORLD, &req);

				// recebe borda inferior do grid vizinho de cima (tag = 2)
				if(pid == 0)
					MPI_Recv(newgrid[0], N, MPI_INT, numProc-1, 2, MPI_COMM_WORLD, &status);
				else
					MPI_Recv(newgrid[0], N, MPI_INT, pid-1, 2, MPI_COMM_WORLD, &status);
				// recebe borda superior do vizinho de baixo (tag = 3)
				MPI_Recv(newgrid[size+1], N, MPI_INT, (pid+1)%numProc, 3, MPI_COMM_WORLD, &status);

				// calcula a proxima geracao
				simulate(newgrid, grid, size, N);

				// conta as celulas vivas na subtabela
				alive = countAlive(newgrid, size, N);
			}
		}

		// espera todos os processos terminarem o calculo
		MPI_Barrier(MPI_COMM_WORLD);

		if(pid < master){
			// envia o numero de vivos para o mestre (tag = 4)
			MPI_Isend(&alive, 1, MPI_INT, master, 4, MPI_COMM_WORLD, &req);
		}
		else{
			// mestre recebe e conta (tag = 4)
			soma = 0;
			for(j = 0; j <= master - 1; j ++){
				MPI_Recv(&alive, 1, MPI_INT, j, 4, MPI_COMM_WORLD, &status);
				soma += alive;
			}
			printf("Geracao [%d]\t %d celulas vivas\n", i, soma);
		}

		// espera todos os processos terminarem a contagem
		MPI_Barrier(MPI_COMM_WORLD);
	}

	// termina a contagem do tempo
	if(pid == master)
		finish = MPI_Wtime();

	if(pid < master){
		// libera as matrizes
		for(i = 0; i < size + 2 ; i++){
			free(grid[i]);
			free(newgrid[i]);
		}
		free(grid);
		free(newgrid);
		grid = NULL;
		newgrid = NULL;
	}

	if(pid == master)
		printf("\nTempo total = %.14lf segundos.\n", finish-start);

	// Finaliza os processos
	MPI_Finalize();
	return 0;
}