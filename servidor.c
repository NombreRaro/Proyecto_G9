#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <mysql.h>

/*void ConexionSql(){
    MYSQL *conn;
    int errno
    MYSQL *resultado;
    MYSQL_ROW row;
   
    conn = mysql_init(NULL);
    if(conn==NULL){
        printf("Error al crear la conexion: %u %s\n",mysql_errno(conn),mysql_error(conn));
        exit(1);
    }
    conn = mysql_real_connect(conn,"localhost","root","mysql", <nombre basedatos>, 0, NULL, 0);
    if(conn=NULL){
        printf("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
}std=c99 'mysql_config --cflags --libs'
*/

int main(int argc, char *argv[]) {
    int sock_conn,sock_listen,ret;
    struct sockaddr_in serv_adr;
    char peticion[512];
    char respuesta[512];
   
   
    //Obrim socket
    //ConexionSql();
    if((sock_listen = socket(AF_INET, SOCK_STREAM, 0))<0)
        printf("Error creant socket");
   
    //BInd al port
   
    memset(&serv_adr,0,sizeof(serv_adr)); //inicialitza a zero serv_addr
    serv_adr.sin_family=AF_INET;
   
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); //FICA ip localeconv
    serv_adr.sin_port =htons(9050);
   
    if(bind(sock_listen,(struct sockaddr *) & serv_adr,sizeof(serv_adr))<0)
        printf("Error al bind");
    if(listen(sock_listen,3)<0)
        printf("Error Listen");
   
    printf("Esperando las credenciales:\n");
    sock_conn=accept(sock_listen,NULL,NULL);
    ret=read(sock_conn,peticion,sizeof(peticion));
   
    printf("Recibido\n");
    peticion[ret]='\0';
    printf("Peticion: %s\n", peticion);
   
    char username[20];
    char password[20];
    char *pass;
   
    char *credentials = strtok(peticion,"/");
    strcpy(username,credentials);
    pass = strtok(NULL,"/");
    strcpy(password,pass);
   
   
   
   
    printf("%s\n",username);
    printf("%s\n",password);
   
    int i=0;
    for(i=0;i<5;i++){
        //acceptem conexio d'un client
        printf("Escuchando\n");
        sock_conn=accept(sock_listen,NULL,NULL);
       
        //SERVEI
       
        ret=read(sock_conn,peticion,sizeof(peticion));
        printf("Recibido\n");
        peticion[ret]='\0';
        printf("Peticion: %s\n", peticion);
       
       
   
    }
   
   
   
}