#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //Hilos
/*Semáforos*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
/**********/
#include "FuncEstructBase.c"

void ingresarDatos();
void multiplicar(Param *);
void crearSemaforo(char const*);

Matriz mA;
Matriz mB;
Matriz mC;
int semid;
struct sembuf operacion;

int main(int argc, char const *argv[]){
	ingresarDatos();
	if(mA.cCols!=mB.cFilas){
		printf("No se puede efectuar la operación sobre las matrices.\n");
	}
	else{
		mC.cFilas=mA.cFilas;
		mC.cCols=mB.cCols;
		crearMatriz(&mA);
		crearMatriz(&mB);
		crearMatriz(&mC);

		printf("Matriz A:\n");
		llenarMatriz(&mA);
		mostrarMatriz(mA);

		printf("\nMatriz B:\n");
		llenarMatriz(&mB);
		mostrarMatriz(mB);

		crearSemaforo(argv[0]);

		pthread_t arHilos[mC.cFilas];
		Param p[mC.cFilas];
		int i;
		printf("\nRealizando operación...\n");
		for(i=0; i<mC.cFilas; ++i){
			p[i].ind=i;
			if(pthread_create(&arHilos[i], NULL, (void *)&multiplicar, (void *)&p[i])<0){
				perror("pthread_create");
				exit(-1);
			}
		}

		for(i=0;i<mC.cFilas;++i){
			pthread_join(arHilos[i], NULL);
		}

		printf("\nMatriz C:\n");
		mostrarMatriz(mC);

		liberarMemoria(&mA);
		liberarMemoria(&mB);
		liberarMemoria(&mC);
		semctl(semid, 0, IPC_RMID, NULL);
	}
	return 0;
}

void ingresarDatos(){
	printf("Cantidad de filas(---) de la matriz A:");
	scanf("%d", &mA.cFilas);
	printf("Cantidad de columnas(|||) de la matriz A:");
	scanf("%d", &mA.cCols);
	printf("Cantidad de filas(---) de la matriz B:");
	scanf("%d", &mB.cFilas);
	printf("Cantidad de columnas(|||) de la matriz B:");
	scanf("%d", &mB.cCols);

	printf("Tamaño matriz A: %d*%d\n", mA.cFilas, mA.cCols);
	printf("Tamaño matriz B: %d*%d\n", mB.cFilas, mB.cCols);

	srand(time(NULL));
}

void multiplicar(Param *p){
	printf("Hilo %d: esperando semáforo\n", p->ind);
	operacion.sem_op=-1;
	operacion.sem_num=0;
	operacion.sem_flg=0;
	semop(semid, &operacion, 1);
	printf("Hilo %d: Semáforo obtendido.\n", p->ind);

	int n,j;
	for (j=0; j<mC.cCols; ++j){
		for(n=0; n<mA.cCols; ++n){
			mC.ma[p->ind][j]+=mA.ma[p->ind][n]*mB.ma[n][j];
		}
	}

	sleep(2);
	operacion.sem_op=1;
	operacion.sem_num=0;
	operacion.sem_flg=0;
	semop(semid, &operacion, 1);
}

void crearSemaforo(char const *ruta){
	if((semid=semget(ftok(ruta,'K'), 1, IPC_CREAT | 0600))==-1){
		perror("semget");
		exit(-1);
	}
	semctl(semid, 0, SETVAL, 5);
}