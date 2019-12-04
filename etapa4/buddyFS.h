struct fs;
typedef struct fs{
	int fd;
	char nombreTexto[255];
	int cantInodosLibres;
	int cantInodos;
	int idPrimerInodoNoReservado;
	int tamanioInodoEnBytes;
	int tamanioDeBloqueEnBytes;
	int idBloquePrimerBloqueDeDatos;
	int cantBloques;
	int cantBloquesLibres;
	int tamanioTotalEnBytes;

	int tablaInodos;

	int primerEntradaDirectorio;

}FS;

struct archivo;
typedef struct archivo {
	int entradaDirectorio;
	int posInodo;
	int recLen;
	int modo;
	int usuario;
	int grupo;
	int fecha;
	int nameLen;
	
	int inodoNum;
	int links;
	int tamanio;
	char nombre[255];
	char fechaTexto[32];
	char grupoTexto[32];
	char usuarioTexto[32];
	char modoTexto[11];
}Archivo;
void fechaF(int segundos, char fechaS []);
void grupoF(int gid, char grupoS []);
void imprimir1(char texto1[], char texto2[]);
void imprimir2(char texto1[], int * numero);
void imprimirBits (int num);
void leer1(int fd, int offset, int bytes, char buffer[]);
void leer2(int fd, int offset, int bytes, int *buffer);
void leerEImprimir1(int fd, int offset, int bytes, char buffer[], char texto[]);
void leerEImprimir2(int fd, int offset, int bytes, int *buffer, char texto[]);
void mostrarError(char texto[]);
void obtenerModo(int modo, char salida[11]);
void usuarioF(int uid, char usuarioS []);