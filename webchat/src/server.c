#include<sys/types.h> //including data types
#include<sys/socket.h> //listening
#include<netdb.h> //provide network db operations
#include<fcntl.h> //open the connection
#include<unistd.h> //close the connection
#include<stdio.h> 
#include<string.h>

int main()
{
 char str[100]; //receiving data
 char sendline1[100];
 bzero(sendline1,100);
 int listen_fd,comm_fd;
 struct sockaddr_in servaddr; //IPv4_IPv6, IP add, PORT add
 
 //creating a socket, address family inet(using v4),use INET6 for v6
 listen_fd=socket(AF_INET,SOCK_STREAM,0);
 //SOCK_STREAM = TCP, SOCK_DGRAM = UDP
 //0 is default protocol not flag operation
 
 
 bzero(str,100); //want to make 100 bytes 0
 bzero(&servaddr,sizeof(servaddr)); //initialize all values with 0
 
 //defining server address..passing those 3 parameters
 servaddr.sin_family=AF_INET; //IPv4
 servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 //server will connect with any IP addr
 //htonl is used to convert host byte into network byte
 servaddr.sin_port=htons(21000);
 //0 to 65,535....first 1024 ports are fixed
 
 
 //binding server addr with socket
 bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
 //listening 
 listen(listen_fd,5); //5 clients can be connected to the server
 
 //to establish connection with client...like a pipeline
 comm_fd=accept(listen_fd,(struct sockaddr *)NULL,NULL);
 
 //client initializing data
 while(1){
 recv(comm_fd,str,100,0); //hardcoded 100 because we do not know length of incoming stream
 printf("Client: %s",str);
 printf("Your message: ");
 fgets(sendline1,100,stdin);
 send(comm_fd,sendline1,strlen(sendline1),0);
 bzero(sendline1,100);
 //0 is flag operation...tells abt successfull and unsuccessfull op
 }
 close(comm_fd);
}