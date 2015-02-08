/*
   SIMRAN KEDIA 201201024
   GURNIAMAT KAUR 201201050*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

char history[100][1000];
char complete_command[1000];
char command[100][100];
int command_count=0;
char server_send_data [1024],server_recv_data[1024];
char recv_complete_command[1024];
char recv_command[32][32];
int recv_command_count=0;

void take_input()
{
	char c;
	scanf("%[^\n]",complete_command);
	scanf("%c",&c);
	if(complete_command[0]=='I')
		strcpy(history[command_count++],complete_command);

}

int tokenise2(char comm[1000])
{

	char *pch=NULL;
	char copy[1000];
	strcpy(copy,comm);
	pch = strtok (copy," ");
	int p=0;
	if(pch!=NULL)
	{
		strcpy(recv_command[p],pch);
		p++;
		pch=strtok(NULL," ");
		if(pch!=NULL)
		{
			strcpy(recv_command[p],pch);
			p++;
			pch=strtok(NULL," ");
			if(pch!=NULL)
			{
				strcpy(recv_command[p],pch);
				p++;
				//printf("pch ---------%s\n ",pch);
				pch=strtok(NULL," ");
				if(pch!=NULL)
				{
					strcpy(recv_command[p],pch);
					p++;
					//printf("pch ---------%s\n ",pch);
					pch=strtok(NULL," ");
				}
			}
		}
	}
	recv_command[p][0]='\0';
	return p;
	/*int i;
	  for(i=0;i<p;i++)
	  printf("%s ",recv_command[i]);
	  printf("\n");
	  return p;*/
}


int tokenise1(char comm[1000])
{

	char *pch=NULL;
	char copy[1000];
	strcpy(copy,comm);
	pch = strtok (copy," ");
	int p=0;
	if(pch!=NULL)
	{
		strcpy(command[p],pch);
		p++;
		pch=strtok(NULL," ");
		if(pch!=NULL)
		{
			strcpy(command[p],pch);
			p++;
			pch=strtok(NULL," ");
			if(pch!=NULL)
			{
				strcpy(command[p],pch);
				p++;
				//printf("pch ---------%s\n ",pch);
				pch=strtok(NULL," ");
				if(pch!=NULL)
				{
					strcpy(command[p],pch);
					p++;
					//printf("pch ---------%s\n ",pch);
					pch=strtok(NULL," ");
				}
			}
		}
	}
	command[p][0]='\0';
	return p;
	/*printf("Complete command tokenised\n");
	  int i;
	  for(i=0;i<p;i++)
	  printf("%s ",command[i]);
	  printf("\n");
	  return p;*/
}

void server_function(FILE *fp,int connected)
{
	
	memset(server_send_data,0,1024);
	int sentn;
	int byter;
	while(!feof(fp))
	{       //printf("while loop count\n");
		byter=fread(server_send_data,1,1024,fp);
		server_send_data[byter]='\0';
		sentn = write(connected,server_send_data,1024);
		//printf("%s",server_send_data);
	}
	memset(server_send_data,0,1024);
	char end[1000];
	strcpy(end,"End Of File");
	strcpy(server_send_data,end);
	write(connected,server_send_data,1024);
	fclose(fp);
	//printf("NOw what?\n");
}


int server_code(int s_port_no,int type)
{
	int sock, connected, bytes_recieved ;


	struct sockaddr_in server_addr,client_addr;
	int sin_size;
	char comarr[100];
	char run[1024];

	if(type==0)
	{
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Socket");
			return 1;
		}
	}
	if(type==1)
	{
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("Socket");
			return 1;
		}
	}


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(s_port_no);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1) 
	{
		perror("Unable to bind");
		return 0;
	}

	if(type==0)
	{
		if (listen(sock, 5) == -1)
		{
			perror("Listen");
			return 0;
		}
	}
	printf("Server Waiting for client on port %d\n $ ",s_port_no);
	fflush(stdout);

	while(1)
	{

		sin_size = sizeof(struct sockaddr_in);
		socklen_t * a = (socklen_t *) &sin_size;
		if(type==0)
		{
			while((connected = accept(sock, (struct sockaddr *)&client_addr,a))<0)
			{
				printf("Connection refused\n");
				sleep(3);
			}


			printf("Connection received from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			printf("\n");
			//break;
		}
		while(1)
		{
			if(type==0)
				bytes_recieved = recv(connected,server_recv_data,1024,0);
			if(type==1)
				bytes_recieved = recvfrom(sock,server_recv_data,1024,0,(struct sockaddr *)&client_addr, a);
			server_recv_data[bytes_recieved] = '\0';
			if(bytes_recieved==0){
				printf("Connection closed\n");
				printf("$");
				close(connected);
				break;
			}
			if(strcmp(server_recv_data , "Exit") == 0 || strcmp(server_recv_data , "exit") == 0){
				printf("Connection closed\n");
				printf("$");

				close(connected);
				break;
			}
			else{
				printf("\n\n");
				printf("Received Command = %s\n" , server_recv_data);
				printf("$");
				fflush(stdout);
				int coun=0;
				coun=tokenise2(server_recv_data);
				
				if(coun<2)
				{
					printf("Invalid command. Provide more arguments\n");
					continue;
				}

				//printf("server recv data-%s\n",server_recv_data);


				//printf("recv_comman[0]  %s\n",recv_command[0]);
				//printf("recv_comman[1]  %s\n",recv_command[1]);
				
				if((strcmp(recv_command[0],"FileHash")==0) && (strcmp(recv_command[1],"CheckAll")==0))
				{
					if(coun>2)
					{
						printf("Invalid Command.Need just 2 arguments\n");
						continue;
					}


					//printf(" entered loop once\n");
					if(type==0)
					{
						system("find . -type f -exec sh -c 'printf \"%s %s \n\" \"$(ls -l --time-style=+%Y%m%d%H%M%S $1 )\" \"$(md5sum $1 | cut -d\" \" -f1)\"' '' '{}' '{}' \\; | awk '{print $7, $6, $8}' > checkallfile");

						FILE *fp=fopen("checkallfile","r+");
						if(fp==NULL)
						{
							printf("File doesnt exist\n");
							continue;
						}
						server_function(fp,connected);
						system("rm checkallfile");

					}
					else 
					{
						system("find . -type f -exec sh -c 'printf \"%s %s \n\" \"$(ls -l --time-style=+%Y%m%d%H%M%S $1 )\" \"$(md5sum $1 | cut -d\" \" -f1)\"' '' '{}' '{}' \\; | awk '{print $7, $6, $8}' > checkall");

						FILE *fp = fopen("checkall","r");
						if(fp == NULL)
						{
							printf("File doesnot exists\n");
							continue;
						}
						memset(server_send_data,0,1024);
						int byter;
						int sentn;
						while(!feof(fp))
						{
							byter = fread(server_send_data,1,1024,fp);
							server_send_data[byter] = '\0';
							sentn = sendto(sock, server_send_data, 1024, 0,(struct sockaddr *)&client_addr,a);
							//printf("%s",server_send_data);
						}
						memset(server_send_data,0,1024);
						char end[]= "End Of File";
						strcpy(server_send_data,end);
						sendto(sock, server_send_data, 1024, 0,(struct sockaddr *)&client_addr,a);
						fclose(fp);
						system("rm checkallfile");
						

						//printf("recvfrom_data after check all : %s \n",server_recv_data);
					}
				}
				else if((strcmp(recv_command[0],"FileHash")==0) && (strcmp(recv_command[1],"Verify")==0))
				{
					if(coun!=3)
					{
						printf("Invalid command. Provide more arguments\n");
						continue;
					}
					strncpy(comarr,recv_command[2],100);
					//	printf("DEBUG--%s\n",command[2]);
					//		printf("Verifying %s\n",comarr);

					strcpy(run,"openssl md5 ");
					strcat(run,comarr);
					strcat(run," | cut -d\" \" -f2 > md5");
					system(run);
					strcpy(run,"date -r ./");
					strcat(run,comarr);
					strcat(run," +%Y%m%d%H%M%S > date");
					system(run);
					system("paste md5 date > verify");
					system("rm md5 date");

					FILE *fp = fopen("verify","r+");
					if(fp == NULL)
					{
						printf("File doesnot exists\n");
						continue;
					}

					memset(server_send_data,0,1024);
					int byter,sentn;
					while(!feof(fp))
					{
						//printf("entered while loop\n");
						byter = fread(server_send_data,1,1024,fp);
						server_send_data[byter] = '\0';
						if(type==0)
							sentn = write(connected,server_send_data,1024);
						else
							sentn = sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					}
					memset(server_send_data,0,1024);
					char end[]= "End Of File";
					strcpy(server_send_data,end);
					//printf("NOw server data is %s\n",server_send_data);
					if(type==0)
						write(connected,server_send_data,1024);
					else
						sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					fclose(fp);
					system("rm verify");


				}
				else if((strcmp(recv_command[0],"IndexGet")==0) && (strcmp(recv_command[1],"LongList")==0))
				{
					if(coun!=2)
					{
						printf("Invalid command. Provide more arguments\n");
						continue;
					}

					system("find . -printf '%p %TY-%Tm-%Td %TH:%Tm %kKB \n' > longlist");

					FILE *fp = fopen("longlist","r+");
					if(fp == NULL)
					{
						printf("File doesnot exist\n");
						continue;
					}

					memset(server_send_data,0,1024);
					int byter,sentn;
					while(!feof(fp))
					{
						byter = fread(server_send_data,1,1024,fp);
						server_send_data[byter] = '\0';
						if(type==0)
							sentn = write(connected,server_send_data,1024);
						else
							sentn = sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					}
					memset(server_send_data,0,1024);
					char end[100];
					strcpy(end,"End Of File");
					strcpy(server_send_data,end);
					if(type==0)
						write(connected,server_send_data,1024);
					else
						sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					fclose(fp);
					system("rm longlist");

					//printf("recv_data after longlist : %s \n",server_send_data);
				}
				else if((strcmp(recv_command[0],"IndexGet")==0) && (strcmp(recv_command[1],"RegEx")==0))
				{
					if(coun!=3)
					{
						printf("Invalid command. Provide more arguments\n");
						continue;
					}
					char comarr[100];
					char run[100];

					strncpy(comarr,recv_command[2],100);
					strcpy(run,"ls ");
					strcat(run,comarr);
					strcat(run," > regex");
					system(run);


					FILE *fp = fopen("regex","r+");
					if(fp == NULL)
					{
						printf("File doesnot exist\n");
						continue;
					}
					memset(server_send_data,0,1024);
					int byter,sentn;
					while(!feof(fp))
					{
						byter = fread(server_send_data,1,1024,fp);
						server_send_data[byter] = '\0';
						if(type==0)
							sentn = write(connected,server_send_data,1024);
						else
							sentn=sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					}
					memset(server_send_data,0,1024);
					char end[1000];
					strcpy(end,"End Of File");
					strcpy(server_send_data,end);
					if(type==0)
						write(connected,server_send_data,1024);
					else
						sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					fclose(fp);
					system("rm regex");
				}
				else if((strcmp(recv_command[0],"IndexGet")==0) && (strcmp(recv_command[1],"ShortList")==0))
				{
					int cntr=0;
					char buff[1000],copy[1000];
					int place=0;
					if(coun!=4)
					{
						printf("Invalid command. Provide more arguments\n");
						continue;
					}
					char comarr[100];
					strncpy(comarr,command[2],100);
					system("ls -l --time-style=+%Y%m%d%H%M%S -t > listing");

					FILE *fpr;
					FILE *fpr2;

					fpr = fopen( "listing", "r+" );
					fpr2 = fopen( "shortlist", "w+" );

					char *pch;

					while ( fgets( buff, 1000, fpr ) != NULL )
					{	
						if(cntr!=0 && cntr!=1)
						{
							strcpy(copy,buff);
							place=0;
							pch = strtok (buff," ");
							while (pch != NULL)
							{
								if(place==5)
								{
									if(strcmp(pch,recv_command[2])>0 && strcmp(pch,recv_command[3])<0)
									{
										fprintf(fpr2,"%s",copy);
									}
								}
								place++;
								pch = strtok (NULL," ");
							}
						}
						cntr++;
					}
					fclose( fpr );
					fclose( fpr2 );

					FILE *fp = fopen("shortlist","r+");
					if(fp == NULL)
					{
						printf("File doesnot exist\n");
						continue;
					}
					memset(server_send_data,0,1024);
					int byter,sentn;
					while(!feof(fp))
					{
						byter = fread(server_send_data,1,1024,fp);
						server_send_data[byter] = '\0';
						if(type==0)
							sentn = write(connected,server_send_data,1024);
						else
							sentn=sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					}
					memset(server_send_data,0,1024);
					char end[1000];
					strcpy(end,"End Of File");
					strcpy(server_send_data,end);
					if(type==0)
						write(connected,server_send_data,1024);
					else
						sendto(sock,server_send_data,1024,0,(struct sockaddr *)&client_addr,a);
					fclose(fp);
					system("rm listing shortlist");
				}
				else
					printf("Invalid Command\n");
			}
		}
		fflush(stdout);
	}
	close(sock);
	return 0;
}

int client_code(int c_port_no,int type)
{
	int sock, bytes_received;
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;

	host = gethostbyname("127.0.0.1");

	if(type==0)
	{
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Socket");
			return 1;
		}
	}
	if(type==1)
	{
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			perror("Socket");
			return 1;
		}
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(c_port_no);
	memcpy(&server_addr.sin_addr,host->h_addr,host->h_length);
	bzero(&(server_addr.sin_zero),8);
	int con;

	int sin_size = sizeof(struct sockaddr_in);
	socklen_t * a = (socklen_t *) &sin_size;

	if(type==0)
	{
		while((con=connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr))) == -1)
		{
			printf("Connection failed\n");
			sleep(3);
		}
		printf("\n Client Connected to Port No %d\n",c_port_no);
	}
	char c;
	scanf("%c",&c);
	printf("$\n");
	take_input();
	//printf("%s",complete_command);
	while(strcmp(complete_command,"Exit")!=0 && strcmp(complete_command,"q")!=0 && strcmp(complete_command,"Q")!=0 && strcmp(complete_command,"exit")!=0)
	{

		if(type==0)
			send(sock,complete_command,strlen(complete_command),0);
		else
			sendto(sock,complete_command,strlen(complete_command),0,(struct sockaddr *)&server_addr,a);

		//printf("HIIIIIIIII\n");
		//	printf("complete command ---%s\n",complete_command);

		if(strcmp(complete_command,"")==0)
			printf("INVALID COMMAND\n",complete_command);

		int co=0;
		co=tokenise1(complete_command);
		if(co<2)
		{
			printf("Invalid command. Provide more arguments\n");
			printf("\n$");
			take_input();
			continue;
		}

		//printf("command[0] %s\n",command[0]);
		//printf("command[1] %s\n",command[1]);

		if((strcmp(command[0],"FileHash")==0) && (strcmp(command[1],"Verify")==0))
		{
			if(co!=3)
			{
				printf("Invalid command. Provide more arguments\n");
				printf("\n$");
				take_input();
				continue;
			}
			while(1)
			{
				//printf("ENTERED again\n");
				if(type==0)
					bytes_received=recv(sock, recv_data,1024,0);
				else
					bytes_received=recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr,a);
				recv_data[bytes_received]='\0';
				//printf("recieved\n",recv_data);
				if(strcmp(recv_data,"End Of File")==0)
					break;
				fwrite(recv_data,1,bytes_received,stdout);
			}
		}
		else if((strcmp(command[0],"FileHash")==0) && (strcmp(command[1],"CheckAll")==0))
		{
			if(co!=2)
			{
				printf("Invalid command. Provide more arguments\n");
				printf("\n$");
				take_input();
				continue;
			}
			while(1)
			{
				if(type==0)
					bytes_received=recv(sock, recv_data,1024,0);
				else
					bytes_received=recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr,a);
				recv_data[bytes_received]='\0';
				if(strcmp(recv_data,"End Of File")==0)
					break;
				fwrite(recv_data,1,bytes_received,stdout);
			}
		}
		else if((strcmp(command[0],"IndexGet")==0) && (strcmp(command[1],"LongList")==0))
		{
			if(co!=2)
			{
				printf("Invalid command. Provide more arguments\n");
				printf("\n$");
				take_input();
				continue;
			}
			while(1)
			{
				if(type==0)
					bytes_received=recv(sock, recv_data,1024,0);
				else
					bytes_received=recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr,a);
				recv_data[bytes_received]='\0';
				if(strcmp(recv_data,"End Of File")==0)
					break;
				fwrite(recv_data,1,bytes_received,stdout);
			}
		}
		else if((strcmp(command[0],"IndexGet")==0) && (strcmp(command[1],"RegEx")==0))
		{
			if(co!=3)
			{
				printf("Invalid command. Provide more arguments\n");
				printf("\n$");
				take_input();
				continue;
			}
			while(1)
			{
				if(type==0)
					bytes_received=recv(sock, recv_data,1024,0);
				else
					bytes_received=recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr,a);
				recv_data[bytes_received]='\0';
				if(strcmp(recv_data,"End Of File")==0)
					break;
				fwrite(recv_data,1,bytes_received,stdout);
			}
		}
		else if((strcmp(command[0],"IndexGet")==0) && (strcmp(command[1],"ShortList")==0))
		{
			if(co!=4)
			{
				printf("Invalid command. Provide more arguments\n");
				printf("\n$");
				take_input();
				continue;
			}
			while(1)
			{
				if(type==0)
					bytes_received=recv(sock, recv_data,1024,0);
				else
					bytes_received=recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&server_addr,a);
				recv_data[bytes_received]='\0';
				if(strcmp(recv_data,"End Of File")==0)
					break;
				fwrite(recv_data,1,bytes_received,stdout);
			}
		}
		else
			printf("Invalid Command\n");

		printf("\n$");
		take_input();
		//printf("%s\n",complete_command);

	}
	return 0;

}

int main()
{
	int s_port_no;
	int c_port_no;
	int type;
	printf("Specify your port no : ");
	scanf("%d",&s_port_no);
	printf("Specify the port no you want to connect to : ");
	scanf("%d",&c_port_no);
	printf("Type of transfer protocol :  0-tcp & 1-udp: ");
	scanf("%d",&type);
	pid_t pid;
	pid=fork();
	if(pid==-1)
	{
		printf("Error in creating Fork\n");
		exit(0);
	}
	if (pid==0)
	{
		server_code(s_port_no,type);
	}
	else
	{
		client_code(c_port_no,type);

	}
	kill(pid,SIGQUIT);
	return 0;
}


