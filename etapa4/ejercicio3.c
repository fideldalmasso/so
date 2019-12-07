#include "buddyFS.h"
#include <stdio.h>
#include <stdlib.h> 


int asignar2(int tamNube,int tamArchivo, int arbol[]);
void asignar(int tamNube,int tamArchivo, int arbol[]);
int espacioSuficiente(struct ext2_inode ti [], struct ext2_dir_entry_2 tde [], int cantEntradasDirectorios, int tamNube);
void imprimir(int tamNube,int nodo, int arbol[]);
void particionar(int nodo, int arbol[]);
int place(int nodo, int arbol[]);
int potencia(int base,int exp);



void particionar(int nodo, int arbol[]){
	while(nodo!=0){
		nodo=nodo%2==0?(nodo-1)/2:nodo/2;
		arbol[nodo]=1;
	}
}

int place(int nodo, int arbol[]){
	while(nodo!=0){
		nodo=nodo%2==0?(nodo-1)/2:nodo/2;
		if(arbol[nodo]>1)
			return 0;
	}
	return 1;
}

int potencia(int base,int exp){
	int i,resp;
	if(exp==0) return 1;
	resp=base;
	for(i=1;i<exp;i++)
		resp*=base;
	return resp;
}

void imprimir(int tamNube,int nodo, int arbol[]){
	int permission=0,llimit,ulimit,tab;
	
	if(nodo==0)
		permission=1;
	else if(nodo%2==0)
		permission=arbol[(nodo-1)/2]==1?1:0;
	else
		permission=arbol[nodo/2]==1?1:0;
	
	if(permission){
		llimit=ulimit=tab=0;
		
		while(1){
			if(nodo>=llimit && nodo<=ulimit){
				break;
			}else{
				tab++;
				printf(".");
				llimit=ulimit+1;
				ulimit=2*ulimit+2;
			}
		}
		
		printf(" %d ",tamNube/potencia(2,tab));
		
		if(arbol[nodo]>1)
			printf("---> Asignado %db\n",arbol[nodo]);
		else 
			if(arbol[nodo]==1)
			printf("------> \n");
			else 
				printf("|---> Libre\n");
		
		imprimir(tamNube,2*nodo+1,arbol);
		imprimir(tamNube,2*nodo+2,arbol);
	}
}

void asignar(int tamNube,int tamArchivo, int arbol[]){
	int nivelActual=0, tamActual=tamNube, i=0;
	
	if(tamArchivo>tamNube){
		printf("Asignacion de %db FALLIDA (no hay mas espacio)\n",tamArchivo);
		return;
	}
	
	while(1){
		if(tamArchivo<=tamActual && tamArchivo>(tamActual/2)){
			break;
		}else{
			tamActual/=2;
			nivelActual++;
		}
	}
	
	for(i=potencia(2,nivelActual)-1;i<=(potencia(2,nivelActual+1)-2);i++){
		if(arbol[i]==0 && place(i,arbol)){
			arbol[i]=tamArchivo;
			particionar(i,arbol);
			printf("Asignacion de %db EXITOSA\n",tamArchivo);
			break;
		}
	}
	
	if(i==potencia(2,nivelActual+1)-1){
		printf("Asignacion de %db FALLIDA (no hay mas espacio)\n",tamArchivo);
	}
}

int asignar2(int tamNube,int tamArchivo, int arbol[]){
	int nivelActual=0, tamActual=tamNube, i=0;
	
	if(tamArchivo>tamNube){
		return 0;
	}
	
	while(1){
		if(tamArchivo<=tamActual && tamArchivo>(tamActual/2)){
			break;
		}else{
			tamActual/=2;
			nivelActual++;
		}
	}
	
	for(i=potencia(2,nivelActual)-1;i<=(potencia(2,nivelActual+1)-2);i++){
		if(arbol[i]==0 && place(i,arbol)){
			arbol[i]=tamArchivo;
			particionar(i,arbol);
			return 1;
		}
	}
	
	if(i==potencia(2,nivelActual+1)-1){
		return 0;
	}
	
	return -1;
}

int espacioSuficiente (struct ext2_inode ti [], struct ext2_dir_entry_2 tde [], int cantEntradasDirectorios, int tamNube){
	int i=0;
	int *pesoArchivos=(int*) malloc (cantEntradasDirectorios*sizeof(int));
	int *arbol=(int*) malloc (tamNube*sizeof(int));
	
	for (i=0; i<cantEntradasDirectorios; i++){
		pesoArchivos[i]=ti[tde[i].inode-1].i_size;
		if (asignar2(tamNube,pesoArchivos[i],arbol)==0){
			return tamNube=espacioSuficiente(ti,tde,cantEntradasDirectorios,tamNube*2);
		}
	}
	return tamNube;
}

void ejercicio3 (struct ext2_inode ti [], struct ext2_dir_entry_2 tde [], int cantEntradasDirectorios, int tamNube){
	int i=0, espacioOcupado=0;
	int *pesoArchivos=(int*) malloc (cantEntradasDirectorios*sizeof(int));
	int *arbol=(int*) malloc (tamNube*sizeof(int));
	
	for (i=0; i<cantEntradasDirectorios; i++){
		pesoArchivos[i]=ti[tde[i].inode-1].i_size;
		espacioOcupado+=pesoArchivos[i];
		asignar(tamNube,pesoArchivos[i],arbol);
		imprimir(tamNube,0,arbol);
		printf("\n");
	}
	
	printf("ESPACIO NECESARIO PARA REALIZAR TODAS LAS ASIGNACIONES: %d\n", espacioSuficiente(ti,tde,cantEntradasDirectorios,tamNube));
	printf("ESPACIO TOTAL OCUPADO %db\n",espacioOcupado);
	
}
