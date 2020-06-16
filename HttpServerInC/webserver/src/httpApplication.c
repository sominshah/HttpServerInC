#include"..\include\headers.h"
#include"..\include\httpResponse.h"
#include"..\include\httpRequest.h"
typedef struct  Application_def
{
	int numberOfResources;
	char * name;           // xyz
	char * contextPath;    // ../webapps/xyz
	char * contextName;    // /xyz;
	LinkedList * resources;  // Resource is an structure.    
	
}Application;
typedef struct Resource_def
{ 
	char type[21];
	char contentType[21]; //text/html
	char name[101]; // index.html
	char * resourcePath; //  /xyz/index.html
	char * value;
	long size;
	char * location; // ../webapps/xyz/index.html
	void (*loadDynamicContents)(HttpRequest * httpRequest ,HttpResponse * httpResponse);
}Resource;