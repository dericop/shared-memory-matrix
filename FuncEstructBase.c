#include <stdlib.h>
#include <time.h>

/*
* Estructuras y métdos para matriz
*/
typedef struct
{
	int cFilas;
	int cCols;
	int **ma;
	int tamMatriz;
} Matriz;

void crearMatriz(Matriz *m){
	m->tamMatriz=m->cFilas*m->cCols;
	m->ma = (int **) malloc(sizeof(int*)*m->cFilas);
	if(m->ma != NULL){
		int i;
		for (i=0; i<m->cFilas; ++i){
			m->ma[i]=(int *) malloc(sizeof(int)*m->cCols);
			if(m->ma[i]==NULL){
				perror("No hay memoria suficiente.");
				exit(1);
			}
		}
	}
	else{
		perror("No hay memoria suficiente.");
		exit(1);
	}
}

void liberarMemoria(Matriz *m){
	int i;
	for (i=0; i<m->cFilas; ++i){
		free(m->ma[i]);
	}
	free(m->ma);
}

void llenarMatriz(Matriz *m){
	int i,j;
	for (i=0;i<m->cFilas;++i){
		for(j=0;j<m->cCols;++j){
			m->ma[i][j]=(rand()%100);
		}
	}
}

void mostrarMatriz(Matriz m){
	int i,j;
	for (i=0;i<m.cFilas;++i){
		for(j=0;j<m.cCols;++j){
			printf("m[%d][%d]=%d\n",i,j,m.ma[i][j]);
		}
	}
}

/*
* Parámetro para función del hilo
*/
typedef struct
{
	int ind;
} Param;