#include<stdio.h>
#include<winsock2.h>
int main(int args,char *argv[])
{
WSADATA wsa;
SOCKET s,new_socket;
struct sockaddr_in server,client;
int c;
char *message,clientRequest[2000];
int clientRequestSize;

printf("Initializing Winsock...");
if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
{
printf("Faild to load winsock library , Error Code: %d",WSAGetLastError());
return 1;
}
printf("Winsock library has been initialised !");

//create a socket

if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
{
printf("Could not create socket: %d",WSAGetLastError());
}

printf("socket has been created!");
//prepare sockaddr_in structure

server.sin_family=AF_INET;
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port=htons(8090);

//Bind

if(bind(s,(const struct socket_addr *)&server,sizeof(server))==SOCKET_ERROR)
{
printf("Bind faild with error code: %d",WSAGetLastError());
}
puts("Bind done!");

//listen to incoming connections

listen(s,3);

//Accept and incoming connection

puts("waiting for incoming connections...");
c=sizeof(struct sockaddr_in);
new_socket=accept(s,(struct sockaddr *)&client,&c);

if(new_socket==INVALID_SOCKET)
{
printf("Accept faild with Error code: %d",WSAGetLastError());
return 1;
}

puts("Connection accepted");

	if((clientRequestSize = recv(new_socket , clientRequest , 2000 , 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
clientRequest[clientRequestSize] = '\0';

puts(clientRequest);


message="Hello client I have received your connection,but I have to go for this time";
send(new_socket,message,strlen(message),0);


closesocket(s);
WSACleanup();
return 0;
}