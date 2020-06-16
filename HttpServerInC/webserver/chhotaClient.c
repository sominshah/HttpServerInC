#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
	int port=atoi(argv[1]);
	

	WSADATA wsa;
	SOCKET s;
	char *message, server_reply[2000];
	int recv_size;
	  
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.");

        if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket has been created!");

        struct sockaddr_in server;                        

//Function inet_addr is a very handy function to convert an IP address to a long format.

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
 
       //Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

/*
Quick Note
The concept of "connections" apply to SOCK_STREAM/TCP type of sockets. 
Connection means a reliable "stream" of data 
such that there can be multiple such streams each having communication of its own.
Think of this as a pipe which is not interfered by other data.
Other sockets like UDP , ICMP , ARP dont have a concept of "connection". 
These are non-connection based communication. 
Which means you keep sending or receiving packets from anybody and everybody.

Sending Data
Function send will simply send data.
It needs the socket descriptor , the data to send and its size.
*/

message="Hello Server yo man how are you doin'!.";

if( send(s , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");
/*
Receiving Data
Function recv is used to receive data on a socket.*/

	if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
server_reply[recv_size] = '\0';

puts(server_reply);
/*Function closesocket is used to close the socket.
 Also WSACleanup must be called to unload the winsock library (ws2_32.dll).
*/
closesocket(s);
WSACleanup();
return 0;
}