#include"..\include\headers.h"
#include"..\include\clientHandler.h"
#include"..\include\filehandler.h"
#include"..\include\httpApplication.h"
typedef struct Server_def
{
int portNumber;
HANDLE threadHandle;
DWORD threadId;
}Server;

ApplicationDeployer * appDeployer=NULL;
typedef struct HttpServer_def 
{
Server serverList[100];
int numberOfActiveServers;
int allotedPortNumbers[100];
int (*startServer) (struct HttpServer_def * httpServer,int * portNumbers,int numberOfServers);
void (*deleteServer) (struct HttpServer_def * httpServer,int portNumber);
int (*getNumberOfActiveServers) (struct HttpServer_def * httpServer);
Server * (*getAllServers) (struct HttpServer_def * httpServer);
void (*waitingForServer)(struct HttpServer_def * httpServer);
void (*addDynamicResource)(struct HttpServer_def * httpServer,const char * uri,void (*sourceInjector)(HttpRequest *,HttpResponse *));
HANDLE serverHandles[100];
}HttpServer;
void HttpServer_addDynamicResource(struct HttpServer_def * httpServer,const char * uri,void (*sourceInjector)(HttpRequest *,HttpResponse *));
void HttpServer_createServer(HttpServer *httpServer,int * portNumbers,int numberOfServers);
int HttpServer_startServer(HttpServer *httpServer,int * portNumbers,int numberOfServers);
void init(HttpServer * httpserver);
DWORD WINAPI HttpServer_startListening(void *param);
Server * HttpServer_getAllServers(HttpServer *httpServer);
void HttpServer_deleteServerByPort(HttpServer *httpServer,int portNumber);
int HttpServer_getNumberOfActiveServers(HttpServer *httpServer);
void HttpServer_waitingForServer(HttpServer *httpServer);
int HttpServer_createSocket(int portNumber);
DWORD WINAPI HttpServer_clientHandler(void * param);
void HttpServer_printClientIP(struct sockaddr_in * client);
void HttpServer_waitingForServer(HttpServer *httpServer)
{
WaitForMultipleObjects(httpServer->numberOfActiveServers, httpServer->serverHandles, TRUE, INFINITE);
CloseHandle(httpServer->serverHandles);
}



int HttpServer_startServer(HttpServer * httpServer,int * portNumbers,int numberOfServers)
{
if(httpServer->numberOfActiveServers!=0)
{
return SERVER_CREATION_FAILED;
}
HttpServer_createServer(httpServer,portNumbers,numberOfServers);
return SERVER_CREATION_SUCCEED;
}

void init(HttpServer * httpserver)
{
if(httpserver==NULL)
{
printf("httpServer is null \n");
return;
}
httpserver->startServer=HttpServer_startServer;
httpserver->deleteServer=HttpServer_deleteServerByPort;
httpserver->getNumberOfActiveServers=HttpServer_getNumberOfActiveServers;
httpserver->getAllServers=HttpServer_getAllServers;
httpserver->waitingForServer=HttpServer_waitingForServer;
httpserver->addDynamicResource=HttpServer_addDynamicResource;
httpserver->numberOfActiveServers=0;
appDeployer=getApplicationDeployer();
if(appDeployer==NULL)
{
printf("Something wrong happend during the deployment of the applications.\n");
return;
}
else
{
appDeployer->deploy();
printf("\n Total %d web applications has bee deployed.\n", appDeployer->numberOfApplications);
Application * app;
LinkedList * res;
int i=0;
int m=0;
int v=0;
Resource * resou=NULL;
printf("+++++++++++++++++++++++++++Websites++++++++++++++++++++++++++\n");
while(i<appDeployer->numberOfApplications)
{
	app=(Application *) appDeployer->applications->get(appDeployer->applications,i);
	printf("Name --> %s\n",app->name);        
	printf("contextPath --> %s\n",app->contextPath); 
	printf("contextName --> %s\n",app->contextName);
	printf("Number of resources->%d\n",app->numberOfResources); 
	
	res=app->resources;
	m=res->getSize(res);
	v=0;
	while(v<m)
	{
	resou=(Resource *) res->get(res,v);
	printf("\nResource type: (%s)",resou->type); //static or dynamic
	printf("\nResource contentType: (%s)",resou->contentType); // text/html
	printf("\nResource name: (%s)",resou->name); // index.html
	printf("\nResource Path: (%s)",resou->resourcePath); //  /xyz/index.html
	printf("\nResource size: (%d)",resou->size);
	printf("\nResource location: (%s)",resou->location); 
	v++;
		printf("\n----------------------------------------\n");
	}
		printf("\n_____________________________________\n");

	i++;
}
printf("+++++++++++++++++++++++++++Websites++++++++++++++++++++++++++\n");
}
}

void HttpServer_addDynamicResource(struct HttpServer_def * httpServer,const char * uri,void (*sourceInjector)(HttpRequest *,HttpResponse *))
{
if(appDeployer==NULL)
{
printf("Application deployer is not ready yet, may be you have missed to call init function.\n");
return;
}
//printf("Http Server addDynamicResourceURI: %s\n",uri );
appDeployer->addDynamicResource(appDeployer,uri,sourceInjector);
}


int HttpServer_getNumberOfActiveServers(HttpServer *httpServer)
{
return httpServer->numberOfActiveServers;
}

void HttpServer_createServer(HttpServer *httpServer,int * portNumbers,int numberOfServers)
{
//printf("createServer starts\n");
//printf("Number of Servers :%d ",numberOfServers);
int nos=0; 
while(nos<numberOfServers)
{
Server server;
server.portNumber=portNumbers[nos];
DWORD threadId;
HANDLE handle=CreateThread(NULL,0,HttpServer_startListening,(void*)&portNumbers[nos],0,&threadId);
if(handle!=NULL)
{
//printf("New Thread created , Thread ID :%d \n",threadId);
server.threadHandle=handle;
server.threadId=threadId;
httpServer->serverList[httpServer->numberOfActiveServers]=server;
httpServer->allotedPortNumbers[httpServer->numberOfActiveServers]=portNumbers[httpServer->numberOfActiveServers];
httpServer->serverHandles[httpServer->numberOfActiveServers]=handle;
httpServer->numberOfActiveServers++;
}

//printf("nos value : %d \n",nos);
nos++;
}
//printf("createServer ends\n");
}


Server * HttpServer_getAllServers(HttpServer *httpServer)
{
return &(httpServer->serverList[0]);
}

void HttpServer_deleteServerByPort(HttpServer *httpServer,int portNumber)
{
Server *server=NULL;
int i=0,e=0,p=0;
while(i<httpServer->numberOfActiveServers)
{
	server=NULL;
	server=&(httpServer->serverList[i]);
		if(server!=NULL && portNumber==server->portNumber) 
			{
				for(e=i;e<httpServer->numberOfActiveServers;e++)
				{
				httpServer->serverList[e]=httpServer->serverList[e+1];
				httpServer->allotedPortNumbers[e]=httpServer->allotedPortNumbers[e+1];
				}
				httpServer->numberOfActiveServers--;
				break;
			}
					
i++;
}
				if(server!=NULL)
				{
				DWORD result = WaitForSingleObject(server->threadHandle, 0);
				if (result != WAIT_OBJECT_0) 
					{
    					// the thread handle is not signaled - the thread is still alive
    					CloseHandle(server->threadHandle);
					}
				}

}

DWORD WINAPI HttpServer_startListening(void *param)
{
int *portNumber=(int *) param;
HttpServer_createSocket(*portNumber);
return 0;
}
int HttpServer_createSocket(int portNumber)
{
WSADATA wsa;
SOCKET s,new_socket;
struct sockaddr_in server,client;
int c;
//printf("Initializing Winsock...\n");
if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
{
printf("\nFaild to load winsock library , Error Code: %d \n",WSAGetLastError());
return 1;
}
//printf("\nWinsock library has been initialised !\n");

//create a socket

if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
{
printf("Could not create socket: %d\n",WSAGetLastError());
}

//printf("socket has been created!\n");
//prepare sockaddr_in structure

server.sin_family=AF_INET;
server.sin_addr.s_addr=INADDR_ANY;
server.sin_port=htons(portNumber);

//Bind


if(bind(s,(const struct sockaddr  *)&server,sizeof(server))==SOCKET_ERROR)
{
printf("Bind faild with error code: %d\n",WSAGetLastError());
}

//puts("\nBind done!\n");

//listen to incoming connections
int iResult=0;
int iSendResult=0;
iResult=listen(s,10);
if(iResult==SOCKET_ERROR)
{
	printf("listen faild with error: %d \n",WSAGetLastError());
	closesocket(s);
	WSACleanup();
	return 1;
}
puts("waiting for incoming connections...\n");
c=sizeof(struct sockaddr_in);


int bOptVal = 10000;
int bOptLen=sizeof(int);

int iOptVal=0;
int iOptLen=sizeof(int);
    iResult = getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &iOptVal, &iOptLen);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"getsockopt for  SO_RCVTIMEO failed with error: %u\n", WSAGetLastError());
    } else
        //wprintf(L"SO_RCVTIMEO Value: %ld\n", iOptVal);

    iResult = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &bOptVal, bOptLen);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"setsockopt for SO_RCVTIMEO failed with error: %u\n", WSAGetLastError());
    } else
        //wprintf(L"Set SO_RCVTIMEO: ON\n");

    iResult = getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &iOptVal, &iOptLen);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"getsockopt for SO_RCVTIMEO failed with error: %u\n", WSAGetLastError());
    } else
        //wprintf(L"SO_RCVTIMEO Value: %ld\n", iOptVal);



while(1)
{
	new_socket = INVALID_SOCKET;
new_socket=accept(s,(struct sockaddr *)&client,&c);
HttpServer_printClientIP(&client);
if(new_socket==INVALID_SOCKET)
{
printf("Accept faild with Error code: %d",WSAGetLastError());
continue;
}

//puts("\nConnection accepted\n");
SOCKET * sock=(SOCKET * )malloc(sizeof(SOCKET));
*sock=new_socket;
DWORD threadId;
HANDLE handle=CreateThread(NULL,0,HttpServer_clientHandler,(void *)sock,0,&threadId);
if(handle!=NULL)
{
//printf("New Thread created with thread id %d for handling the client",threadId);
}
}

closesocket(s);
WSACleanup();
return 0;
}
 
DWORD WINAPI HttpServer_clientHandler(void * param)
{ 
	printf("New Thread created \n");
int iResult=0;
SOCKET * socket=(SOCKET *) param;
ClientHandler clientHandler;
initialize(&clientHandler);
int connectionTimeOut=0;
do
{
	clientHandler.readRequest(&clientHandler,socket);
	if(clientHandler.requestStringLength==0 || clientHandler.requestStringLength<0)
	{
		break;
	}
HttpRequest * httpRequest = clientHandler.getHttpRequest(&clientHandler);
HttpResponse * httpResponse = clientHandler.processRequest(&clientHandler,appDeployer,httpRequest);
clientHandler.sendResponse(&clientHandler,socket,httpResponse);
clientHandler.requestStringLength=0;
free(clientHandler.requestString);
clientHandler.dispose(httpRequest,httpResponse);
}while(1);

printf("Now it's the time to close the socket\n" );

   iResult = shutdown(*socket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
    {
        closesocket(*socket);
        return 1;
    }
 closesocket(*socket);
 free(socket);
 return 1;
}

void HttpServer_printClientIP(struct sockaddr_in * client)
{
struct in_addr ipAddr = client->sin_addr;
char * str; 
str=inet_ntoa(ipAddr);
//printf(" \n Client portNumber : %d \n",(int)client->sin_port);
}

