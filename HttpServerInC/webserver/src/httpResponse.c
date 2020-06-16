#include"..\include\pair.h"
#include"..\include\httpWriter.h"
typedef struct HttpResponse_def
{
char * statusLine;
LinkedList * responseHeaders;
Writer * writer;
char * resourceLocation;
char * responseBody;
long responseLength;
char * contentType;
void (*addWriter)(struct HttpResponse_def *,Writer *);
void (*setContentType)(struct HttpResponse_def *, char *);
}HttpResponse;
void init_HttpResponse(HttpResponse *httpResponse);
void HttpResponse_addWriter(HttpResponse * httpResponse,Writer * Writer);
void HttpResponse_setContentType(HttpResponse * httpResponse,char * ct);

void init_HttpResponse(HttpResponse *httpResponse)
{
httpResponse->writer=NULL;
httpResponse->resourceLocation=NULL;
httpResponse->statusLine=NULL;
httpResponse->responseHeaders=NULL;
httpResponse->responseBody=NULL;
httpResponse->responseLength=0;
httpResponse->addWriter=HttpResponse_addWriter;
httpResponse->setContentType=HttpResponse_setContentType;
httpResponse->contentType=NULL;
}
void HttpResponse_addWriter(HttpResponse * httpResponse,Writer * writer)
{
httpResponse->writer=writer;
httpResponse->responseLength=writer->contentLength;
}
void HttpResponse_setContentType(HttpResponse * httpResponse,char * ct)
{
char * contentType=(char *) malloc(strlen(ct)+1);
strcpy(contentType,ct);
httpResponse->contentType=contentType;
}