#ifndef HttpServerDefination
#define HttpServerDefination
#include"headers.h"
#include"filehandler.h"
typedef struct Server_def
{
int portNumber;
HANDLE threadHandle;
DWORD threadId;
}Server;

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

#endif