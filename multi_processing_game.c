/*
 * Modelo E-2
 */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 

int main()
{
	int dados_i[2][2],dados_o[2][2];
	pid_t pid;

    int i, players = 2, games = 5, aposta, resultado, vitorias[2][3], leitura[2], status;
    for(i = 0; i < players; i++)
    {
        pipe(dados_i[i]);
        pipe(dados_o[i]);
        vitorias[i][0]=0;//empates
        vitorias[i][1]=0;//vitoria
        vitorias[i][2]=0;//derrotas
	}

	for(i = 0; i < players; i++)
    {
        pid = fork();
        if(pid == 0)
        {
        	break;
        }
        if(pid < 0)
        {
        	perror("Erro no fork()");
        }
              
    }

    for(games; games>0; games--)
    {
    	if(pid==0)/*filho*/
    	{
    		aposta = random_bet();
    		write(dados_i[i][1], &aposta, sizeof(int));
    		read(dados_o[i][0], &resultado, sizeof(int));
    		if(resultado == 0){
    			printf("Jogador %d : Empatei!\n", i);
    		} else if(resultado == 1){
    			if(i==0){
    				printf("Jogador %d : Ganhei!\n", i);
    			} else {
    				printf("Jogador %d : Perdi!\n", i);
    			}
    		} else {
    			if(i==0){
    				printf("Jogador %d : Perdi!\n", i);
    			} else {
    				printf("Jogador %d : Ganhei!\n", i);
    			}
    		}
    		
    		
    		if(games==1)
    		{
    			exit(0);
    		}	
    	} else /*pai*/{
    		
    		int acertar = random_bet();
    		int leitura[2];

    		for(i = 0; i < players; i++)
    		{
        		read(dados_i[i][0], &(leitura[i]), sizeof(int));  
    		}

        	int ronda = result(leitura[0],leitura[1]);
			write(dados_o[0][1], &ronda, sizeof(int));
			write(dados_o[1][1], &ronda, sizeof(int));
			if(ronda==0){
				vitorias[0][0]++;
				vitorias[1][0]++;
			}     
			if(ronda==1){
				vitorias[0][1]++;
				vitorias[1][2]++;
			}   
			if(ronda==2){
				vitorias[0][2]++;
				vitorias[1][1]++;
			}        

			for(i = 0; i < players; i++)
    		{
				printf("O jogador %d tem %d vitorias, %d derrotas e %d empates\n",
					(i+1), vitorias[i][1],vitorias[i][2],vitorias[i][0]);
			}

    	}
    }

    if(pid > 0)
    {		
    	
    	for(i = 0; i < players; i++)
		{
			wait(&status);
		}
		printf("\nResutados Finais:\n");
		for(i = 0; i < players; i++)
		{
			printf("O jogador %d tem %d vitorias, %d derrotas e %d empates\n",
				(i+1), vitorias[i][1],vitorias[i][2],vitorias[i][0]);
		}
    }

    return (0);
}

int random_bet()
{
    srand(rand()+getpid()); 
    int i = (rand() % 2) + 1;
    return i;
}

int result(int bet1, int bet2)
{
  int result;
  /*1-Pedra 2-Papel 3-tesoura*/
  /*return: 0 - empate | 1 - vencedor 1 | 2 - vencedor 2*/
  if(bet1 == bet2)
  {result =  0;}

  if(bet1==1 && bet2==2)
  {result =  2;} /*pedra(1) contra papel(2) vence papel - ou seja o
jogador 2. Logo retorna 2*/

  if(bet1==1 && bet2==3)
  {result =  1;} /*pedra(1) contar tesoura(3) vence pedra - ou seja o
jogador 1. Logo retorna 1*/

  if(bet1==2 && bet2==1)
  {result =  1;}

  if(bet1==2 && bet2==3)
  {result =  2;}

  if(bet1==3 && bet2==1)
  {result =  2;}

  if(bet1==3 && bet2==2)
  {result =  1;}

  return result;
}
