// Client side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 
#define PORT 8080 
   
int getSock()
{
	int sock = 0;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        exit(EXIT_FAILURE); 
    } 
}
   
struct sockaddr_in configAddress()
{
	struct sockaddr_in serv_addr; 
	
	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    return serv_addr;
}

struct sockaddr_in configIPAddr(struct sockaddr_in serv_addr)
{// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.97", &serv_addr.sin_addr)<=0)  
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		exit(EXIT_FAILURE); 
	} 
	
	return serv_addr;
}

struct sockaddr_in setConnect(int sock, struct sockaddr_in serv_addr)
{
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        exit(EXIT_FAILURE); 
    } 
    
    return serv_addr;
}

void escribirArchivo(int sock,int valread,char newBuffer[1024]){
}
	

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in serv_addr; 
    int sock = 0, valread, leidos; 
    unsigned long long numPaquetes=0;
    FILE *archivoCopia; 
    char buffer[1024]={0}; 
    char newBuffer[1024]={0};
    char direccion[100];
    char bandera[1];
    
    
    
    printf("conectando al servidor\n");
    
    //Configurando el soocket y el address
    sock = getSock();
	serv_addr = configAddress();
	serv_addr = configIPAddr(serv_addr);
	serv_addr = setConnect(sock, serv_addr);

    printf("Ingresa el archivo para descargar\n");scanf("%s",direccion);

	//enviamos el nombre del archivo al servidor
    send(sock, direccion, strlen(direccion), 0 );
    
    //Leer si existe el archivo, es informacion que nos envia el servidor
    read( sock, bandera, 1);
     
    if(bandera[0] == '0'){//Checamos la existencia del archivo
    	printf("No existe el archivo\n");
    	return 0;
    }else{
    	archivoCopia = fopen (direccion, "wb");//abrimos el archivoCopia para escribir la informacion del servidor 

	//mientras numero de datos leidos != 0, dejamos de escribir
    	while((valread = read(sock, &newBuffer, 1024)) != 0){
			fwrite(&newBuffer, 1, valread, archivoCopia);
    	}
    	
	printf("El archivo %s, se a descargado exitosamente del servidor \n",direccion);
    	fclose(archivoCopia);//cierra archivo
    }

    return 0; 
} 
