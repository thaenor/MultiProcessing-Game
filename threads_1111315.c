#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <ctype.h>

typedef struct
{
	int id;
	int media;
}Aluno;

//Criacao dos vectores e variáveis globais, condição e exclusão
int Vec1[250], Vec2[250], Vec3[250], Vec4[250];
Aluno alunos_buffer[50];
int BufferN, contador_global=0;

pthread_mutex_t escrever_buffer;
pthread_mutex_t fim;
pthread_cond_t condicao_buffer_cheio;


//Funcao para a leitura e calculo da media
void * media( void * arg ){
	int i, j, soma=0, avg=0; //avg é uma abreviatura de average - (o mesmo que) média

		for(i=0; i<250; ++i)
		{
			soma += Vec1[i];
			soma += Vec2[i];
			soma += Vec3[i];

			avg = soma/3;

			if (BufferN == 50)
			{
				pthread_cond_signal(&condicao_buffer_cheio);//Avisa que o buffer se encontra cheio e corre limpa_buffer
			}
			else
				{
					pthread_mutex_lock(&escrever_buffer); //Bloqueia buffer (para nao preencher mais que 1 ao mesmo tempo)
					alunos_buffer[i].id = i; //Para saber a qual aluno pertence cada media calculada
					alunos_buffer[i].media = avg;
					BufferN++;
					pthread_mutex_unlock(&escrever_buffer); //Libertar preencher (para preencher no buffer)
				}
		}
	pthread_mutex_unlock(&escrever_buffer); //para garantir que o buffer não fica bloqueado
	pthread_exit( NULL );
}


void * limpa_buffer( void * arg ) {	//limpa o buffer e guarda o respectivo valor no vector 4 Vec4
	int i, j, identificador=0;
	
	for(j=0; j<5; ++j)
	{

		for(i=contador_global; i<(contador_global+50); ++i)
		{
			pthread_mutex_lock(&escrever_buffer); //Bloqueio preencher no buffer (para nao haver escrita enquanto a informação é guardada)
			identificador = alunos_buffer[identificador].id;
			Vec4[identificador] = alunos_buffer[identificador].id;
			identificador ++;
			
			pthread_mutex_unlock(&escrever_buffer); //respectivo desbloqueio do buffer
		}
		contador_global += 50;
	}

	pthread_mutex_unlock(&fim);
	pthread_exit( NULL );
}


int main()
{
	int i, s;
	srand(getpid());
	//Gerar os valores random nos vectores 1 e 2
	for (i=0; i<250; i++)
	{
		//prints de verificação comentados
		Vec1[i]=rand()%(20);
		//printf("%d ", Vec1[i]);
		Vec2[i]=rand()%(20);
		//printf("%d ", Vec1[i]);
		Vec3[i]=rand()%(20);
		//printf("%d ", Vec1[i]);
	}

	printf("Vectores criados, a iniciar calculo da media\n");
	
//Criacao dos mutex
pthread_mutex_init(&escrever_buffer, NULL); 
pthread_cond_init(&condicao_buffer_cheio, NULL);
pthread_mutex_init(&fim, NULL);

//Criacao das threads
pthread_t T1, T2, T3;
pthread_create(&T1, NULL, media, (void *) 0);
pthread_create(&T2, NULL, media, (void *) 0);
pthread_create(&T3, NULL, limpa_buffer, (void *) 0);

//Bloquear o programa (para nao terminar antes do devido tempo)
pthread_mutex_lock(&fim);
printf("Parado!\n");
pthread_mutex_lock(&fim);

//Destroir os mutex
pthread_mutex_destroy(&escrever_buffer);
pthread_mutex_destroy(&fim);

//prints de verificação
/*printf("\n\n\n\n");
for(i=0; i<250; ++i)
{
	printf("%d ", Vec4[i]);
}*/

printf("Terminado!\n");
pthread_exit(NULL);
}