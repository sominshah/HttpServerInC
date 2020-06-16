#ifndef HttpResponseDefination
#define HttpResponseDefination
#include"pair.h"
#include"httpWriter.h"
typedef struct HttpResponse_def
{
char * statusLine;
LinkedList * responseHeaders;
Writer * writer;
char * responseBody;
char * resourceLocation;
long responseLength;
char * contentType;
void (*addWriter)(struct HttpResponse_def *,Writer *);
void (*setContentType)(struct HttpResponse_def *, char *);
}HttpResponse;
void init_HttpResponse(HttpResponse *httpResponse);
void HttpResponse_addWriter(HttpResponse * httpResponse,Writer * Writer);
void HttpResponse_setContentType(HttpResponse * httpResponse,char * ct);
#endif