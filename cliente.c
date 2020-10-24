// Client side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include <stdint.h>
#define PORT 8080
#define DATA_LEN 1024

struct msj{
	
	int8_t numSeq;
	int CRC8;	
	char data[DATA_LEN];
	int16_t lenght;
	int8_t tipo;
}; 
typedef struct msj MSJ;
   
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

float obtenConfirmacion(){
	return  ((rand()%10000)/100.0f);//maximo 99.99
}

float obtenTemporizador(){
	return  ((rand()%6000)/100.0f);//maximo 60.0 seg
}

int obtenValor(char* errorOTiempo, char *porError, char* porTem){

	if(strcmp(errorOTiempo,"-e")==0){
		return atof(porError);
	}else if(strcmp(errorOTiempo,"-p")==0){
		return atof(porTem);
	}else{
		return 0.0;
	}

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
    
    float errorMax;
    float tempMax;
    char* error=strdup(argv[1]);
    char* temp = strdup(argv[2]);
    char* porError = strdup(argv[3]);//obtencion de los porcentajes de error y tiempo
    char* porTemp = strdup(argv[4]);
    float nuevoError;
    float nuevoTemp;
    MSJ mensaje;
    mensaje.lenght=0;
    mensaje.tipo=0;
    mensaje.numSeq=0;
    mensaje.CRC8=0;
    
    
    /*int8_t numSeq;
	int CRC8;	
	char data[DATA_LEN];
	int16_t lenght;
	int8_t tipo;*/
    
    errorMax = obtenValor(error,porError,porTemp);
    tempMax = obtenValor(temp,porError,porTemp);
    
    
    
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
    	while((read(sock, &mensaje, sizeof(mensaje))) != 0){
			fwrite(&mensaje.data, 1, mensaje.lenght, archivoCopia);
    	}
    	
	printf("El archivo %s, se a descargado exitosamente del servidor \n",direccion);
    	fclose(archivoCopia);//cierra archivo
    }

    return 0; 
} 
