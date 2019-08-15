#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>



int main(int argc, char *argv[]){
	char message[255];
	char command[255];
  int server, portNumber,count=0,size,sd;
  char str2[15];
  struct sockaddr_in servAdd;
 if(argc != 3){
    printf("%s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }
  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }
	sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
}

  
 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect failed,exit\n");
  exit(3);
 }
 while(1)
 {
	printf(">");
    fgets(command, 10, stdin); 
	//printf("string is: %s\n", command);
	sprintf(message,"%s",command);
	strcpy(str2, "quit\n");
	
	if(!strcmp(command, str2))
	{
		exit(0);
	}

	write(server, message, strlen(message)+1);
	
	int n;
	char servData[255];
	char ch;
	int i=0;
	while(1)
	{
		n=read(server, &ch, 1);
		if(!n)
		{
			printf("ERROR\n");
			exit(0);
		}
		printf("%c", ch);
		servData[i]=ch;
		i++;
		if(ch=='\n')
		{
			servData[i-1]='\0';
			i=0;
		}
		if(strcmp("--",servData)==0)
		{
			break;
		}	
	}	
 }
}