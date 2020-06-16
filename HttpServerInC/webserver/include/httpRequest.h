#ifndef HttpRequestDefination
#define HttpRequestDefination
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
#endif