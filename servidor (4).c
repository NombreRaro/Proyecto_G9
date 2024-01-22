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

typedef struct{
	Conectado connected[4];
	int numparticipantes,numinvitados;
	int turno,CasLibres;
}Partida;



ListaConectados milista;
Partida partida;



//Obtiene como par√°metros el nombre y el socket del cliente conectado
//Devuelve -1 si hay un error
//Devuelve 0 si ha podido a√±adir correctamente el cliente a la lista de conectados
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
//Obtiene como par√°metros nombre y socket de los clientes conectados
//Devuelve -1 si no se puede a√±adir/invitar a nadie m√°s a la partida
//Devuelve 0 si ha podido crear la partida correctamente
int AfegirPartida(Partida *partida, char nombre[20], int socket){
	if (partida->numparticipantes==4){
		return -1;
	}else{
		strcpy(partida->connected[partida->numparticipantes].nombre,nombre);
		partida->connected[partida->numparticipantes].socket=socket;
		partida->numparticipantes++;
		return 0;
	}
}
//Busca un usuario conectado, usando nombre como par√°metro
//Devuelve -1 si hay un error
//Devuelve 0 si el proceso es correcto
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
//Recibre nombre como par√°metro
//Devuelve -1 si hay un error
//Devuelve 0 si ha podido eliminar a un usuario de la lista de conectados
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
//Recibe nombre como par√°metro
//Devuelve -1 si hay un error
//Devuelve 0 si encuentra el socket correctamente del cliente conectado
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


//Funcion para checkear los usuarios conectados actualmente
//Hace un recorrido devolviendo un 'sprintf' con los nombres de los usuarios conectados
void DameConectados(ListaConectados *lista, char conectados[300]){
	sprintf(conectados,"%d", lista->num);
	int i;
	for(i=0;i<lista->num;i++){
		sprintf(conectados,"%s/%s", conectados, lista->conectados[i].nombre);
	}
}
//Funcion para checkear los usuarios que se han invitado a la partida
//Hace un recorrido devolviendo un 'sprintf' con los nombres de los usuarios invitados
void DameInvitados(Partida *partida, char invitados[300]){
	sprintf(invitados,"%d", partida->numparticipantes);
	int i;
	for(i=0;i<partida->numparticipantes;i++){
		sprintf(invitados,"%s/%s", invitados, partida->connected[i].nombre);
	}
}
//Funcion para registrar un usuario en la base de datos
//Devuelve 0 si el proceso falla
//Devuelve 1 si el registro se hace satisfactoriamente
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
//Funcion para que un usuario inicie sesi√≥n
//Devuelve 0 si el proceso falla
//Devuelve 1 si se efectua correctamente
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
//INSERTA el ultimo ganador de la partida
//Devuelve una cadena de caracteres indicando error de consulta
//AÒade el jugador de ultima partida jugada
void anyadir_victorias(int id, int pos,char respuesta[512]){
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
	sprintf(consulta,"INSERT INTO Participacion VALUES '");
	strcat (consulta, id); 
	strcat (consulta, "','");
	//concatenamos el nombre 
	strcat (consulta, pos); 
	strcat (consulta, "'");
	
	err=mysql_query(conn,consulta);
	if (err!=0) {
		printf ("Error al introducir datos la base %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado=mysql_store_result(conn);
	row=mysql_fetch_row(resultado);
	if(row==NULL){
		char noresult="noresult";
		printf("NO se han obtenido datos en la consulta\n");
	}
}
//Consulta del jugador con mas victorias
//Devuelve una cadena de caracteres indicando error de consulta
//AÒade el jugador con mas victorias
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
		sprintf(respuesta,"%s/%s",row[0],row[1]);
	}
	
}
//Consulta del jugador con menos victorias
//Devuelve una cadena de caracteres, indicando error o exito en la consulta
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
		sprintf(respuesta,"%s/%s",row[0],row[1]);
	}
	
}
//Consulta de jugador mas guapo
//Devuelve una cadena de caracteres indicando error o √©xito en la consulta
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
void EnviarConectados(char mensaje[500]){
	char conectados[512];
	char connected[512];
	pthread_mutex_lock(&mutex);
	DameConectados(&milista, conectados);
	pthread_mutex_unlock(&mutex);
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
	
	sprintf(mensaje, "3/%s", conectados);
	printf("\n%s\n", mensaje);
	return mensaje;
}
//ESTA FUNCION es la que atiende al cliente
//SE segmenta en diferentes funciones, utilizando diversos codigos para acceder a cada una de ellas
//0: Peticion de conexion
//1: Peticion mas victorias
//2: Peticion mas derrotas
//3: Peticion mas guapo
//4: Peticion ver conectados
//5: Peticion para enviar mensaje al chat
//6: Peticion para registrar un usuario
//7: Peticion donde un usuario invita a otro para jugar una partida
//8: Peticion para aceptar o declinar una invitaci√≥n
//9: Funcion 'jugar'
//10: Funcion para eliminar un usuario conectado de la lista de conectados
//11:Peticion para reenviar el movimiento de un jugador al resto de usuarios
//13: Envia a todos los invitados de la partida mensajes de texto actualizando el estado de la partida
void *AtenderCliente(void *socket){
	
	char nombre[20];
	int ret;
	int retorno,conectado=0;
	char peticion[512];
	char respuesta [512];
	char res[512];
	char usuario[20];
	int sock_conn = *((int *) socket);
	printf("Te doy el socket %d\n", sock_conn);
	
	
	int terminar = 0;
	
	while(terminar==0){
		int connected=0;
		int antesconectados=0;
		
		
		
		printf("Esperant a peticiÛ:\n");
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Recibido\n");
		peticion[ret]='\0';
		
		printf("Peticion: %s\n", peticion);
		
		char *p = strtok(peticion,"/");
		int codigo = atoi(p);
		printf("%d\n", codigo);
		
		
		
		
		
		if(codigo==0){
			p = strtok(NULL,"/");
			
			char password[20];
			strcpy(nombre, p);
			p = strtok(NULL,"/");
			strcpy(password,p);
			printf("Codigo: %d, Usuario: %s Password: %s\n", codigo,nombre,password);
			int retorno=login(nombre,password);
			sprintf(respuesta,"0/%d", retorno);
			sprintf(usuario,"%s",nombre);
			pthread_mutex_lock(&mutex);
			//int sock = Dame_Socket(&milista,nombre);
			int add=Afegir(&milista,nombre,sock_conn);
			int pos = Dame_Posicion(&milista,nombre);
			pthread_mutex_unlock(&mutex);
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
			connected++;
			EnviarConectados(peticion);
			for(int i=0;i<milista.num;i++){
				write(milista.conectados[i].socket, peticion, strlen(peticion));
			}
			
		}else if(codigo==1){
			char respuesta[512];
			char peticion[512];
			mas_victorias(respuesta);
			sprintf(peticion,"2/1/%s", respuesta);
			//Si demanem qui te mes victories enviarem al client un 10
			printf("\n%s\n", peticion);
			write(sock_conn, peticion, strlen(peticion));
		}else if(codigo==2){
			char respuesta[512];
			char peticion[512];
			menos_victorias(respuesta);
			sprintf(peticion,"2/2/%s", respuesta);
			//Si demanem qui te menys victoris enviarem al client un 11
			printf("\n%s\n", peticion);
			write(sock_conn, peticion, strlen(peticion));
		}else if(codigo==3){
			char respuesta[512];
			char peticion[512];
			mas_guapo(respuesta);
			sprintf(peticion,"2/3/%s", respuesta);
			//Si demanem qui es mes guapo enviarem al client un 12
			printf("\n%s\n", peticion);
			write(sock_conn, peticion, strlen(peticion));
		}else if(codigo==4){
			//veure conectats
			char peticion[512];
			char conectados[512];
			char connected[512];
			/*pthread_mutex_lock(&mutex);
			DameConectados(&milista, conectados);
			pthread_mutex_unlock(&mutex);
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
			
			sprintf(peticion, "3/%s", conectados);
			printf("\n%s\n", peticion);*/
			EnviarConectados(peticion);
			write(sock_conn, peticion, strlen(peticion));
		}else if(codigo==5){
			//chat
			char resp[800];
			char user[20];
			char text[512];
			printf("Chat de text rebut\n");
			p = strtok(NULL,"/");
			strcpy(user, p);
			p = strtok(NULL,"/");
			strcpy(text,p);
			printf("%s: %s\n", user, text);
			
			
			sprintf(resp,"4/%s/%s",user,text);
			printf("Enviando mensaje\n");
			printf("%s\n",resp);
			for(int i=0;i<milista.num;i++){
				printf("Envio a: %d\n", milista.conectados[i].socket); 
				write(milista.conectados[i].socket,resp,strlen(resp));
			}
		}else if(codigo==6){
			printf("Registrando a usuario:\n");
			
			char nombre[20];
			char password[20];
			p = strtok(NULL,"/");
			strcpy(nombre, p);
			
			p = strtok(NULL,"/");
			strcpy(password,p);
			printf("Codigo: %d, Usuario: %s Password: %s\n", codigo,nombre,password);
			
			pthread_mutex_lock(&mutex);
			int ret=registro(nombre,password);
			pthread_mutex_unlock(&mutex);
			
			sprintf(respuesta,"%d", ret);
			printf("%s", respuesta);
			if(ret==1){
				printf("Se ha REGISTRADO el usuario");
				sprintf(respuesta,"1/%d", ret);
				connected++;
				
			}else{
				printf("NO se ha podido inscribir al siguiente usuario");
			}
			//Si registrem a un usuari enviarem al client un 0
			EnviarConectados(peticion);
			for(int i=0;i<milista.num;i++){
				write(milista.conectados[i].socket, peticion, strlen(peticion));
			}
		}else if(codigo==7){
			//7/2/PERSONA QUE INVITA/arnau
			int number;
			int encontrado=0;
			char names[512];
			char resp[512];
			char sender[20];
			//7/Maria/Juan
			printf("Enviando peticion a:\n");
			p = strtok(NULL,"/");//numero de persones a que se li envia la peticiÛ
			number = atoi(p);
			p = strtok(NULL,"/");
			strcpy(sender,p);
			
			pthread_mutex_lock(&mutex);
			int socket_sender = Dame_Socket(&milista, sender);
			pthread_mutex_unlock(&mutex);
	
			pthread_mutex_lock(&mutex);
			int sock = AfegirPartida(&partida, sender, socket_sender);
			pthread_mutex_unlock(&mutex);
			
			for(int i=0;i<=number-1;i++){
				p = strtok(NULL,"/");
				strcpy(nombre,p);
				pthread_mutex_lock(&mutex);
				int sock = Dame_Socket(&milista, nombre);
				pthread_mutex_unlock(&mutex);
				if(sock == -1){
					printf("Jugador %s no conectado\n", nombre);
					/*strcpy(resp, "5/");
					write(sock_conn, resp, strlen(resp));*/
				}else{
					printf("Usuario %s invita a %s con socket: %d\n", sender, nombre, sock);
					sprintf(resp, "5/%s", sender);
					printf("%s\n", resp);
					write(sock, resp, strlen(resp));
				}
				
			
				
				
				
			}
				
		}else if(codigo==8){
			//8/1/PERSONA QUE ACCEPTA O DECLINA PETICI”
			int number;
			char sender[20];
			char invited[300];
			char env[512];
			p = strtok(NULL,"/");
			number = atoi(p);
			p = strtok(NULL,"/");
			strcpy(sender,p);
			if(number==0){
				//NO SE ACEPTA PETICION
				printf("El usuari %s no accepta\n", sender);
				sprintf(res, "6/0/%s",sender);
				printf("%s\n", res);
				for(int i=0;i<milista.num;i++){
					write(partida.connected[i].socket, res, strlen(res));
				}
			}else{
				//S'ACCEPTA PETICIO
				printf("El usuari %s accepta\n", sender);
				int sock = Dame_Socket(&milista,sender);
				pthread_mutex_lock(&mutex);
				int resp = AfegirPartida(&partida, sender, sock);
				pthread_mutex_unlock(&mutex);
				if(resp==-1){		
					//LLISTA PLENA
					printf("La llista de invitats a partida esta plena\n");
					sprintf(res, "6/1/%s",sender);
					printf("%s\n", res);
					write(sock_conn, res, strlen(res));
				}else{
					//ENVIEM LLISTA COMPLETA DE INVITATS:
					printf("S'afegeix correctament el usuari %s a la llista de invitats\n",sender);
					printf("Num. llista invitats: %d\n", partida.numparticipantes);
					pthread_mutex_lock(&mutex);
					DameInvitados(&partida, invited);
					pthread_mutex_unlock(&mutex);
					for(int i=0;i<milista.num;i++){
						int j=i+1;
						sprintf(env, "6/3/%d",j);
						printf("%s\n", env);
						write(partida.connected[i].socket, env, strlen(env));
					}printf("\n");
					printf("Invitados: %s\n", invited);
					sprintf(res, "6/2/%s",invited);
					printf("Respuesta: %s\n", res);
					for(int i=0;i<milista.num;i++){
						write(partida.connected[i].socket, res, strlen(res));
					}
						
				}
				
			}
		}else if(codigo == 9){
			//FUNCIO JUGAR
			strcpy(peticion,"7/1");
			for(int i=0;i<milista.num;i++){
				write(partida.connected[i].socket, peticion, strlen(peticion));
			}
		
		}else if(codigo==10){
			int eliminar = Eliminar_Conectado(&milista, usuario);
			//write(sock_conn, respuesta, strlen(respuesta));
			//Si eliminem a un usuari de conectats enviarem al client un 0. 
			if(eliminar==-1){
				printf("No esta\n");
			}else{
				printf("Usuario %s eliminado\n",usuario);
				EnviarConectados(peticion);
				for(int i=0;i<milista.num;i++){
					write(milista.conectados[i].socket, peticion, strlen(peticion));
				}
				
				connected--;
			}
			close(sock_conn);
			terminar=1;
		}else if(codigo == 11){
			int turno, contador, next_turno;
			//11/jug1,2,3/valor mogut/next_torn
			p = strtok(NULL,"/");
			turno = atoi(p);
			if(turno == 5){
				//VICTORIA
				sprintf(peticion,"8/5");
				for(int i=0;i<milista.num;i++){
					write(partida.connected[i].socket, peticion, strlen(peticion));
				}
			}else{
				p = strtok(NULL,"/");
				contador = atoi(p);
				p = strtok(NULL,"/");
				next_turno = atoi(p);
				sprintf(peticion,"8/%d/%d/%d", turno,contador, next_turno);
				printf("%s\n", peticion);
				for(int i=0;i<milista.num;i++){
					write(partida.connected[i].socket, peticion, strlen(peticion));
				}
			}
			
			
		}else if(codigo == 13){
			//13/username/text
			char mensaje[512];
			char user[20];
			p = strtok(NULL,"/");
			strcpy(user,p);
			p = strtok(NULL,"/");
			strcpy(mensaje,p);
			p = strtok(NULL,"/");
			int n = atoi(p);
			if(n==1){
				sprintf(peticion,"9/%s/%s/1", user,mensaje);
				printf("\n%s\n", peticion);
				for(int i=0;i<milista.num;i++){
					write(partida.connected[i].socket, peticion, strlen(peticion));
				}
			}else{
				
				sprintf(peticion,"9/%s/%s/0", user,mensaje);
				printf("\n%s\n", peticion);
				for(int i=0;i<milista.num;i++){
				write(partida.connected[i].socket, peticion, strlen(peticion));
				
				}
			}
		}
		else{
				 printf("NO has introducido un codigo correcto");
		}
	
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
	
	serv_adr.sin_port = htons(9051);
	if(bind(sock_listen,(struct sockaddr *) & serv_adr,sizeof(serv_adr))<0)
		printf("Error al bind\n");
	if(listen(sock_listen,3)<0)
		printf("Error Listen");
	
	int i=0;
	printf("Anem a comenÁar conexio:\n");
	
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

