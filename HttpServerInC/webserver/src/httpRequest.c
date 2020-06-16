#include"..\include\pair.h"
typedef struct HttpRequest_def
{
char * method;
char * uri;
char * protocol;
char * dynamicPageURI;
int isQueryExist;
LinkedList * queryParameters;
LinkedList * requestHeaders;
LinkedList * (*getQueryParameters)(struct HttpRequest_def *);
char * (*getQueryParameter)(struct HttpRequest_def *, char * key);
}HttpRequest;

LinkedList * HttpRequest_getQueryParameters(HttpRequest *);
char * HttpRequest_getQueryParameter(HttpRequest * httpRequest, char * key);
int init_HttPRequest(HttpRequest * httpRequest);

int init_HttPRequest(HttpRequest * httpRequest)
{
httpRequest->method=NULL;
httpRequest->uri=NULL;
httpRequest->protocol=NULL;
httpRequest->dynamicPageURI=NULL;
httpRequest->isQueryExist=0;
httpRequest->queryParameters=NULL;
httpRequest->requestHeaders=NULL;
httpRequest->getQueryParameters=HttpRequest_getQueryParameters;
httpRequest->getQueryParameter=HttpRequest_getQueryParameter;
}
LinkedList * HttpRequest_getQueryParameters(HttpRequest * httpRequest)
{
return httpRequest->queryParameters;
}
char * HttpRequest_getQueryParameter(HttpRequest * httpRequest, char * key)
{
LinkedList * list=httpRequest->queryParameters;
int listSize=list->getSize(list);
int i=0;
Pair * pair;
char * pairKey=NULL;
char * pairValue=NULL;
while(i<listSize)
{
pair=list->get(list,i);
pairKey=(char *) pair->key;
pairValue=(char *) pair->value;
if(strcmp(key,pairKey)==0)
{
break;
}
i++;
}
return pairValue;
}