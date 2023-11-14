#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <mysql.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	char nombre [20];
	int socket;
}Conectado;
typedef struct{
	Conectado conectados[10];
	int num;
}ListaConectados;

ListaConectados milista;

int Afegir(ListaConectados *lista, char nombre[20], int socket){
	if (lista->num==10){
		return -1;
	}else{
		strcpy(lista->conectados[lista->num].nombre,nombre);
		lista->conectados[lista->num].socket=socket;
		lista->num++;
		return 0;
	}
}
int Dame_Posicion(ListaConectados *lista, char nombre[20]){
	int encontrado=0,i=0;
	while(!encontrado){
		if(strcmp(lista->conectados[i].nombre,nombre)==0){
			encontrado=1;
		}else i++;
	}
	if(encontrado){
		return i;
	}else return -1;
}
int Eliminar_Conectado(ListaConectados *lista, char nombre[20]){
	int pos = Dame_Posicion(lista,nombre);
	if(pos==-1){
		return -1;
	}else{
		int i;
		for(i=pos;i<lista->num-1;i++){
			lista->conectados[i]=lista->conectados[i+1];
			//lista->conectados[i].socket=lista->conectados[i+1].socket;
		}
		lista->num--;
		return 0;
	}
}
int Dame_Socket(ListaConectados *lista, char nombre[20]){
	int i=0;
	int encontrado=0;
	while((i<lista->num) && !encontrado){
		if(strcmp(lista->conectados[i].nombre,nombre)==0){
			encontrado=1;
		}else
		   i++;
	}
	if(encontrado){
		return lista->conectados[i].socket;
	}else return -1;
}



void DameConectados(ListaConectados *lista, char conectados[300]){
	sprintf(conectados,"%d", lista->num);
	int i;
	for(i=0;i<lista->num;i++){
		sprintf(conectados,"%s/%s", conectados, lista->conectados[i].nombre);
	}
}
int registro(char nombre[20], char password[20]){
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error al creal la conexion %u %s", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	conn=mysql_real_connect(conn,"localhost","root","mysql","juegoSo", 0, NULL,0);
	if(conn==NULL){
		printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	printf("%s %s", nombre, password);
	sprintf(consulta,"INSERT INTO Jugador(username,password,ganadas) VALUES('");
	strcat(consulta,nombre);
	strcat(consulta, "','");
	strcat(consulta,password);
	strcat(consulta,"',1);");
	err=mysql_query(conn,consulta);
	printf("hola");
	if(err!=0){
		printf("\nError al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		printf("NO se han obtenido datos en la consulta\n");
		return 0;
	}else{
		printf("Se ha inscrito el siguiente usuario\nUsername: %s ,Password: %s\n", row[0],row[1]);
		return 1;
	}
}
int login(char nombre[20],char password[20]){
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error al creal la conexion %u %s", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	conn=mysql_real_connect(conn,"localhost","root","mysql","juegoSo", 0, NULL,0);
	if(conn==NULL){
		printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	sprintf(consulta,"SELECT username,password FROM Jugador WHERE username='");
	strcat(consulta,nombre);
	strcat(consulta, "'and password='");
	strcat(consulta,password);
	strcat(consulta,"'");
	err=mysql_query(conn,consulta);
	if(err!=0){
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		printf("NO se han obtenido datos en la consulta\n");
		return 0;
	}else{
		printf("Se ha encontrado el siguiente usuario\nUsername: %s ,Password: %s\n", row[0],row[1]);
		return 1;
	}
}
void mas_victorias(char respuesta[512]){
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error al creal la conexion %u %s", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	conn=mysql_real_connect(conn,"localhost","root","mysql","juegoSo", 0, NULL,0);
	if(conn==NULL){
		printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	sprintf(consulta,"SELECT username,ganadas FROM Jugador ORDER BY ganadas DESC;");
	err=mysql_query(conn,consulta);
	if(err!=0){
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		char noresult="noresult";
		printf("NO se han obtenido datos en la consulta\n");
	}else{
		printf("El Usuario %s ha ganado %s partidas\n", row[0],row[1]);
		sprintf(respuesta,"%s,%s",row[0],row[1]);
	}
	
}
void menos_victorias(char respuesta[512]){
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error al creal la conexion %u %s", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	conn=mysql_real_connect(conn,"localhost","root","mysql","juegoSo", 0, NULL,0);
	if(conn==NULL){
		printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	sprintf(consulta,"SELECT username,ganadas FROM Jugador ORDER BY ganadas ASC;");
	err=mysql_query(conn,consulta);
	if(err!=0){
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		char noresult="noresult";
		printf("NO se han obtenido datos en la consulta\n");
	}else{
		printf("El Usuario %s ha ganado %s partidas\n", row[0],row[1]);
		sprintf(respuesta,"%s,%s",row[0],row[1]);
	}
	
}
void mas_guapo(char respuesta[512]){
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error al creal la conexion %u %s", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	conn=mysql_real_connect(conn,"localhost","root","mysql","juegoSo", 0, NULL,0);
	if(conn==NULL){
		printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	sprintf(consulta,"SELECT username FROM Jugador ORDER BY RAND();");
	err=mysql_query(conn,consulta);
	if(err!=0){
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		char noresult="noresult";
		printf("NO se han obtenido datos en la consulta\n");
	}else{
		printf("El Usuario %s es el mas guapo", row[0]);
		sprintf(respuesta,"%s",row[0]);
	}
	
}
void *AtenderCliente(void *socket){
	
	
	int ret;
	int retorno,conectado=0;
	char peticion[512];
	char respuesta [512];
	char res[512];
	char usuario[20];
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn=*s;
	
	int terminar = 0;
	
	while(terminar==0){
		
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Recibido\n");
		peticion[ret]='\0';
		
		printf("Peticion: %s\n", peticion);
		
		char *p = strtok(peticion,"/");
		int codigo = atoi(p);
		printf("%d\n", codigo);
		
		if(codigo==0){
			p = strtok(NULL,"/");
			char nombre[20];
			char password[20];
			strcpy(nombre, p);
			p = strtok(NULL,"/");
			strcpy(password,p);
			printf("Codigo: %d, Usuario: %s Password: %s\n", codigo,nombre,password);
			int retorno=login(nombre,password);
			sprintf(respuesta,"%d", retorno);
			sprintf(usuario,"%s",nombre);
			
			int sock = Dame_Socket(&milista,nombre);
			int add=Afegir(&milista,nombre,sock);
			int pos = Dame_Posicion(&milista,nombre);
			
			if(add==-1){
				printf("LLista plena\n");
			}else{
				printf("Afegit\n");
				if(pos==-1){
					printf("El usuario %s no esta en la lista de conectados\n",nombre);
				}
			}
			//Si ens conectem enviarem al client un 0
			write(sock_conn, respuesta, strlen(respuesta));
			
		}else if(codigo==1){
			char respuesta[512];
			mas_victorias(respuesta);
			//Si demanem qui te mes victories enviarem al client un 10
			write(sock_conn, respuesta, strlen(respuesta));
		}else if(codigo==2){
			char respuesta[512];
			menos_victorias(respuesta);
			//Si demanem qui te menys victoris enviarem al client un 11
			write(sock_conn, respuesta, strlen(respuesta));
		}else if(codigo==3){
			char respuesta[512];
			mas_guapo(respuesta);
			//Si demanem qui es mes guapo enviarem al client un 12
			write(sock_conn, respuesta, strlen(respuesta));
		}else if(codigo==4){
			
			char conectados[512];
			char connected[512];
			DameConectados(&milista, conectados);
			sprintf(connected,"%s",conectados);
			char *pi = strtok(connected, "/");
			int n = atoi(pi);
			printf("Personas conectadas: %d\n", n);
			int i;
			for(i=0;i<n;i++){
				char nombre[20];
				pi =strtok(NULL,"/");
				strcpy(nombre,pi);
				printf("Conectado: %s\n", nombre);
			}
			//Si algun client demana actualitzar conectats enviarem un 0
			write(sock_conn, conectados, strlen(conectados));
			
		}else if(codigo==6){
			printf("Registrando a usuario:\n");
			p = strtok(NULL,"/");
			char nombre[20];
			char password[20];
			strcpy(nombre, p);
			p = strtok(NULL,"/");
			strcpy(password,p);
			printf("Codigo: %d, Usuario: %s Password: %s\n", codigo,nombre,password);
			int ret=registro(nombre,password);
			sprintf(respuesta,"%d", ret);
			printf("%s", respuesta);
			if(ret==1){
				printf("Se ha REGISTRADO el usuario");
				sprintf(respuesta,"%d", ret);
				
			}else{
				printf("NO se ha podido inscribir al siguiente usuario");
			}
			//Si registrem a un usuari enviarem al client un 0
			write(sock_conn, respuesta, strlen(respuesta));
		}else if(codigo==10){
			int eliminar = Eliminar_Conectado(&milista, usuario);
			//Si eliminem a un usuari de conectats enviarem al client un 0. 
			if(eliminar==-1){
				printf("No esta\n");
			}else{
				printf("Usuario %s eliminado\n",usuario);
			}
			close(sock_conn);
			terminar=1;
		}
		else
				 printf("NO has introducido un codigo correcto");
		
		
	}
	
	
	
	
}
int main(int argc, char *argv[]) {
	
	
	milista.num=0;
	struct sockaddr_in serv_adr;
	int sock_conn, sock_listen;
	
	if((sock_listen = socket(AF_INET, SOCK_STREAM, 0))<0)
		printf("Error creant el socket");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	serv_adr.sin_port = htons(9053);
	if(bind(sock_listen,(struct sockaddr *) & serv_adr,sizeof(serv_adr))<0)
		printf("Error al bind");
	if(listen(sock_listen,3)<0)
		printf("Error Listen");
	
	int i;
	printf("Anem a comen�ar conexio:\n");
	
	int sockets[100];
	pthread_t thread;
	
	for(;;){
		
		sock_conn = accept(sock_listen, NULL,NULL);
		printf("He recibido conexion\n");
		
		sockets[i] = sock_conn;
		pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
		i++;
	}

		
}

