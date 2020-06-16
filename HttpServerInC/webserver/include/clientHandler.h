#ifndef ClientHandlerDefination
#define ClientHandlerDefination
#include"headers.h"
#include"httpRequestParser.h"
#include"httpRequest.h"
#include"httpResponse.h"
#include"filehandler.h"
typedef struct ClientHandler_def
{
	int requestStringLength;
	char * requestString;
	void (*sendResponse)(struct ClientHandler_def * handler,SOCKET * socket,HttpResponse * httpResponse);
	HttpResponse * (*processRequest)(struct ClientHandler_def * clientHandler,ApplicationDeployer *,HttpRequest * httpRequest);	
	char * (*readRequest)(struct ClientHandler_def * clientHandler,SOCKET *socket);
	HttpRequest * (*getHttpRequest) (struct ClientHandler_def * clientHandler);
	void (*dispose) (HttpRequest *,HttpResponse *);
}ClientHandler;

void initialize(ClientHandler * handler);
HttpRequest * ClientHandler_getHttpRequest(ClientHandler * clientHandler);
char * ClientHandler_readRequest(ClientHandler * clientHandler,SOCKET *socket);
HttpResponse * ClientHandler_processRequest(ClientHandler * clientHandler,ApplicationDeployer *,HttpRequest * httpRequest);
void ClientHandler_sendResponse(ClientHandler * handler,SOCKET * socket,HttpResponse * httpResponse);
void ClientHandler_dispose(HttpRequest *,HttpResponse *);

#endif