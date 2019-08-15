#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

#define MAX_SIZE 1024


void serviceClient(int);

int main(int argc, char *argv[]){  
  int sd, client, portNumber;
  struct sockaddr_in servAdd; 
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

 printf("server started\n");
  while(1){
    client = accept(sd, NULL, NULL);
    printf("client is successfully connected\n");

    if(!fork())
	{
      serviceClient(client);
	 
	}

    //close(client);
  }
}

void serviceClient(int sd){
	char message[255], buffer[MAX_SIZE];
	int string_input_size,n;
	int size,status;
	char *token,*copy;
	char delim[40],*command[40],*args[40];
	dup2(sd, 1);
	while(1)
	{
		int count=0;
		 printf("Waiting for new command\n");
		if(string_input_size=read(sd, message, 255))
		{
			strcpy(buffer,message);
			buffer[string_input_size]='\0';
			copy=strdup(buffer);
			token=strtok(buffer,";&|\n");
			while(token!=NULL)
			{
				delim[count]=copy[token - buffer + strlen(token)];
				command[count]=token;
				count++;
				token=strtok(NULL,";&|\n");
			}
			command[count]=NULL;
			status=-1;
			for(int k=0;k<count;k++)
			{
				if(status > -1)
				{
					if(delim[k-1]=='&')
					{
						if(status!=0)
							break;
					}
					else if(delim[k-1]=='|')
					{
						if(status==0)
							break;
					}
				}
				token=strtok(command[k]," ");
				int total=0;
				while(token!=NULL)
				{
					args[total]=token;
					total++;
					token=strtok(NULL," ");
				}
				args[total]=NULL;
				if(strcmp(args[0],"exit") == 0)
					exit(0);
				pid_t pid = fork();
				if(!pid)
				{
					execvp(args[0],args);
					fprintf(stderr,"execvp erroe\n");
					exit(1);
				}
				else if(pid<0)
				{
					 fprintf(stderr,"Fork error\n");
					 exit(1);
				}
				else
				{
					n=wait(&status);
					if(WIFEXITED(status))
						fprintf(stderr,"err",pid,WEXITSTATUS(status));
				}

			}
			 printf("\n");
			 printf("--\n");
		}
		//break;
	}
	
}