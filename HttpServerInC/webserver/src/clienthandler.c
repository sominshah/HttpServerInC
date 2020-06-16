#include"..\include\headers.h"
#include"..\include\httpRequestParser.h"
#include"..\include\httpResponse.h"
#include"..\include\httpResponseCodes.h"
#include"..\include\filehandler.h"
#include"..\include\httpApplication.h"
#include<fcntl.h>
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
void initialize(ClientHandler * handler)
{
handler->requestStringLength=0;
if(handler->requestString!=NULL)
{
free(handler->requestString);
handler->requestString=NULL;
}
handler->sendResponse=ClientHandler_sendResponse;
handler->processRequest=ClientHandler_processRequest;
handler->readRequest=ClientHandler_readRequest;
handler->getHttpRequest=ClientHandler_getHttpRequest;
handler->dispose=ClientHandler_dispose;
}

void ClientHandler_sendResponse(ClientHandler * handler,SOCKET * socket,HttpResponse * httpResponse)
{
	printf("Sending response starts...\n");
char message[30000];
if(httpResponse!=NULL)
{
LinkedList * rHeaders=httpResponse->responseHeaders;
int numberOfHeaders=rHeaders->getSize(rHeaders);
int i=0;
Pair *p=NULL;
char * key=NULL;
char * value=NULL;


strcpy(message,httpResponse->statusLine);

int stringlen=0;
stringlen=strlen(message);
message[stringlen]='\n';
while(i<numberOfHeaders)
{
	p=(Pair *) rHeaders->get(rHeaders,i);
	key=(char *) p->key;
	strcat(message,key);
	stringlen=strlen(message);
	message[stringlen]=':';
	message[stringlen+1]=' ';
	value=(char *) p->value;
	strcat(message,value);
	message[strlen(message)]='\n';
	i++;
}
	stringlen=strlen(message);
	message[stringlen]='\n';
	message[stringlen+1]='\n';



	if(strcmp(httpResponse->contentType,"image/jpeg")==0)
	{
	printf("Request:---->(%s)\n",handler->requestString);		
	printf("\nResponse:--->(%s)\n",message);
	send(*socket,message,strlen(message)-1,0);

	FILE * file =fopen(httpResponse->resourceLocation,"rb");
	long bytesTosend=httpResponse->responseLength;
	int chunk=2000;
	long totalSent=0;
	int bytesRead=0; 
  	char file_buff[bytesTosend+1];
	fseek(file,0L,SEEK_SET);
	while(bytesTosend>0)
	{
		if(chunk>bytesTosend)chunk=bytesTosend;	
		bytesRead=fread(file_buff,1,chunk, file);
		send(*socket,file_buff,chunk,0);
		totalSent+=chunk;
		bytesTosend-=chunk;
	}

    fclose(file);
	}
	else
	{
		if(httpResponse->responseBody!=NULL )
		{
	 	strcat(message,httpResponse->responseBody);
		send(*socket,message,strlen(message),0);
		}else
		{
			send(*socket,message,strlen(message),0);
			if(httpResponse->writer!=NULL)
			{
				int i=0;
				LinkedList * list=httpResponse->writer->contentList;
				int listSize=list->getSize(list);
				int j=0;
				char * line;
				int strSize=0;
				int bufferIndex=0;
				while(i<listSize)
				{
					j=0;
					line=(char *)list->get(list,i);
				send(*socket,line,strlen(line),0);
				i++;
				}
				httpResponse->writer=NULL;
			}

		}
	
	}
	
	
}
}

HttpResponse * ClientHandler_processRequest(ClientHandler * clientHandler,ApplicationDeployer * applicationDeployer,HttpRequest * httpRequest)
{

if(httpRequest==NULL)
{
return NULL;
}
printf("Processing starts...\n");

int i=0;
Boolean internalError=False;
Boolean protocolError=False;
Boolean methodError=False;
Boolean uriError=True;
Boolean isImage=False;
char * resourceLocation=NULL;
StatusCode statusCode=OK;
char * statusString="OK";
char * responseBody=NULL;
int responseBodySize=0;
		if(httpRequest==NULL) 
		{
			statusCode=INTERNAL_SERVER_ERROR;
			statusString="Internal Server Error";
			internalError=True;
			resourceLocation="../webapps/ROOT/internalError.html";
		}
		if(strcmp(httpRequest->protocol,"HTTP/1.1")!=0)
		{
			statusCode=INTERNAL_SERVER_ERROR;
			statusString="Internal Server Error";	
			protocolError=True;
			resourceLocation="../webapps/ROOT/internalError.html";
		}
		if(strcmp(httpRequest->method,"GET")==0 || strcmp(httpRequest->method,"POST")==0 || strcmp(httpRequest->method,"PUT")==0)
		{
		methodError=False;
		}
		else
		{
			methodError=True;
			statusString="Method Not Allowed";
			statusCode=METHOD_NOT_ALLOWED;
			resourceLocation="../webapps/ROOT/methodNotAllowed.html";	
		}
Resource * resource=NULL;
HttpResponse * httpResponse=(HttpResponse *) malloc(sizeof(HttpResponse));
init_HttpResponse(httpResponse);
if(methodError==False && protocolError==False && internalError==False)
{
	if(httpRequest->isQueryExist)
	{
		resource =applicationDeployer->getResource(applicationDeployer,httpRequest->dynamicPageURI);
		if(resource==NULL)
		{
			resourceLocation="../webapps/ROOT/notFound.html";
			statusString="Not Found";
			statusCode=NOT_FOUND;
			httpResponse->contentType="text/html";
		}else
		{
		resourceLocation=NULL;
		resource->loadDynamicContents(httpRequest,httpResponse);
		httpResponse->responseBody=NULL;
		}
		
	}
	else
	{		
		resource =applicationDeployer->getResource(applicationDeployer,httpRequest->uri);
		if(resource==NULL)
		{	
			httpResponse->contentType="text/html";
			resourceLocation="../webapps/ROOT/notFound.html";
			statusString="Not Found";
			statusCode=NOT_FOUND;
		}else
		{
			httpResponse->contentType=resource->contentType;
			resourceLocation=resource->location;			
		}

	}

}


if(resourceLocation!=NULL && httpResponse->contentType!=NULL)
{	
long fileSize=0;
int bufferSize=0;
FILE *file=NULL;	

		httpResponse->resourceLocation=(char *) malloc(strlen(resourceLocation)+1);
		strcpy(httpResponse->resourceLocation,resourceLocation);
	if(strcmp(httpResponse->contentType,"image/jpeg")==0)
	{
		isImage=True;
		file=fopen(resourceLocation,"rb");
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		responseBodySize=fileSize;
		responseBody=NULL;

	}else
	{
		isImage=False;
	file=fopen(resourceLocation,"r");
	if(file==NULL)
	{
		httpResponse->contentType="text/html";
		resourceLocation="../webapps/ROOT/notFound.html";
		statusString="Not Found";
		statusCode=NOT_FOUND;
		file=fopen(resourceLocation,"r");
	}
	fseek(file,0L,SEEK_END);
	fileSize=ftell(file);
	bufferSize=fileSize+2;
	responseBody=(char *)malloc(bufferSize);
	i=0;
	fseek(file,0L,SEEK_SET);
	while(1)
	{
		if(feof(file))break;
		responseBody[i]=fgetc(file);
		 i++;
	}
	responseBodySize=i;
	}
fclose(file);
}


if(httpResponse->responseLength==0 && httpResponse->responseBody==NULL)
{
httpResponse->responseLength=responseBodySize;
httpResponse->responseBody=responseBody;	
}

char * statusLineBuffer=(char *) malloc(100);
sprintf(statusLineBuffer,"HTTP/1.1 %d %s",statusCode,statusString);
httpResponse->statusLine=statusLineBuffer;
LinkedList * responseHeaderList= (LinkedList *) malloc(sizeof(LinkedList));
createSinglyList(responseHeaderList);



/*HTTP applications have historically allowed three different 
formats for the representation of date/time stamps:
      1)Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
      2)Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
      3)Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format

      We are using third one format.
*/


time_t responseTime;
responseTime = time(NULL);		
char * timeptr=ctime(&responseTime);

Pair * datePair=(Pair *) malloc(sizeof(Pair));
datePair->key=(char *)malloc(5);
char * timeVal=(char *)malloc(30);
strcpy(datePair->key,"Date");
i=0;
char * tp=timeVal;
while(timeptr[i]!='\0')
{
	(*timeVal)=timeptr[i];
//printf("%c",(*timeVal));
i++;
timeVal++;
}
timeVal--;
(*timeVal)='\0';
datePair->value=(void *) tp;
responseHeaderList->add(responseHeaderList,datePair);
Pair * acceptRenge=getPair_str_str("Accept-Ranges","bytes");
responseHeaderList->add(responseHeaderList,acceptRenge);
Pair * serverName=getPair_str_str("Server","C Server");
responseHeaderList->add(responseHeaderList,serverName);
char contentLengthBuffer[10];
sprintf(contentLengthBuffer,"%d",httpResponse->responseLength);
Pair * contentType=getPair_str_str("Content-Type",httpResponse->contentType);
responseHeaderList->add(responseHeaderList,contentType);
Pair * contentLength=getPair_str_str("Content-Length",contentLengthBuffer);
responseHeaderList->add(responseHeaderList,contentLength);
Pair * connection=NULL;
Pair * keepAlive=NULL;
keepAlive= getPair_str_str("Keep-Alive","timeout=10");
responseHeaderList->add(responseHeaderList,keepAlive);
connection=getPair_str_str("Connection","keep-alive");
responseHeaderList->add(responseHeaderList,connection);
httpResponse->responseHeaders=responseHeaderList;
return httpResponse;
}

char * ClientHandler_readRequest(ClientHandler * clientHandler,SOCKET *socket)
{
	clientHandler->requestStringLength=0;
	if(clientHandler->requestString!=NULL)
	{
		free(clientHandler->requestString);
		clientHandler->requestString=NULL;
	}

	int   clientRequestSize=0;   
	char * buffer=NULL;
	int requestBufferSize=30000;
	buffer=(char *)malloc(requestBufferSize);
	memset(buffer,'\0',requestBufferSize);


	clientRequestSize = recv(*socket ,buffer, requestBufferSize , 0);
	if(clientRequestSize==0)
	{
		clientHandler->requestStringLength=0;
		return NULL;
	}
	if((clientRequestSize) == SOCKET_ERROR)
	{
		puts("recv failed");
		clientHandler->requestStringLength=0;
		return NULL;
	}


	buffer[clientRequestSize]='\0';
	clientHandler->requestString=buffer;
	clientHandler->requestStringLength=clientRequestSize;
return buffer;
}


HttpRequest * ClientHandler_getHttpRequest(ClientHandler * clientHandler)
{

if(clientHandler->requestStringLength==0)
{
return NULL;
}
HttpRequest *request=(HttpRequest *) malloc(sizeof(HttpRequest));
init_HttPRequest(request);
parse(clientHandler->requestString,clientHandler->requestStringLength,request);
return request;
}
void ClientHandler_dispose(HttpRequest * httpRequest,HttpResponse * httpResponse)
{
	LinkedList * list=NULL;
	int listSize=0,i=0;
	if(httpRequest!=NULL)
	{
		if(httpRequest->requestHeaders!=NULL)
		{
		list=httpRequest->requestHeaders;
		listSize=list->getSize(list);
		i=0;
			while(i<listSize)
			{
			Pair *pair=(Pair *) list->get(list,i);
			releasePair(pair);
			i++;
			}
		deleteSinglyList(list);
		}
		free(httpRequest);
	}
	if(httpResponse!=NULL)
	{
				if(httpResponse->responseHeaders!=NULL)
				{
				list=httpResponse->responseHeaders;
				listSize=list->getSize(list);
				i=0;
					while(i<listSize)
					{
						Pair *pair=(Pair *) list->get(list,i);
						releasePair(pair);
						i++;
					}
					deleteSinglyList(list);
				}
			free(httpResponse);
	}

}