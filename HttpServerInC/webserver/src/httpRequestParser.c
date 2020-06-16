#include"..\include\headers.h"
#include"..\include\pair.h"
#include"..\include\httpRequest.h"
#include"..\include\httpUrlEncoding.h"
LinkedList * parseQueryString(char * queryString);
void parse(char * httpRequestString,int requestLength,HttpRequest * httpRequest)
{
	printf("Parsing starts...\n");
if(requestLength==0) 
{
httpRequest=NULL;
return;	
}

int i=0;
int c=0;
int k=0;
int p=0;
int m=0;
Boolean isFirstLine=True;
Boolean isMethodType=True;
Boolean isPath=True;
Boolean isProtocol=True;
Boolean isHeaderKey=True;
Boolean isHeaderValue=True;
Boolean isQueryString=False;
int queryStringStartFrom=0;

LinkedList *pairsList= (LinkedList *) malloc(sizeof(LinkedList));
createSinglyList(pairsList);

while(i<requestLength)
{
	if(httpRequestString[i]==13 && httpRequestString[i+1]==10)
	{

		if(isFirstLine)
		{
			k=0;
			while(k<c)
			{

				if(httpRequestString[k]==' ' || (httpRequestString[k]==13 && httpRequestString[k+1]==10))
				{
					if(isMethodType)
					{
					char * methodName=(char*) malloc(k+1);
					p=0;
						while(p<k)
						{
						methodName[p]=httpRequestString[p];
						p++;
						}
					methodName[p]='\0';
					Pair * pair =(Pair *) malloc(sizeof(Pair));
					char * method=(char *) malloc(7);
					strcpy(method,"Method");
					pair->key=method;
					pair->value=methodName;
					httpRequest->method=methodName;
					pairsList->add(pairsList,pair);
					isMethodType=False;
					}
					else
					{

						if(isPath)
						{
						int uriIndex=0;
						char * uri= (char*) malloc((k-p)+2);
						char URLchar;
						p=p+1;
							while(p<k)
							{
							URLchar=httpRequestString[p];
							uri[uriIndex]=URLchar;
							uriIndex++;
							if(URLchar=='?')
							{
								queryStringStartFrom=uriIndex;
								isQueryString=True;
							}	
							p++;
							}
						uri[uriIndex]='\0';
						Pair * pair =(Pair *) malloc(sizeof(Pair));
						char * uriStr=(char *) malloc(4);
						strcpy(uriStr,"Uri");
						pair->key=uriStr;
						pair->value=uri;
						httpRequest->uri=uri;
						pairsList->add(pairsList,pair);
						isPath=False;
						}
						if(isProtocol)
						{
							int proIndex=0;
							char * protocol= (char*) malloc(c-k);
							p=p+1;
							while(p<c)
							{
								protocol[proIndex]=httpRequestString[p];
								proIndex++;
								p++;
							}
							protocol[proIndex]='\0';
							Pair *pair =(Pair *) malloc(sizeof(Pair));
							char * protocolStr=(char *) malloc(9);
							strcpy(protocolStr,"Protocol");
							pair->key=protocolStr;
							pair->value=protocol;
							httpRequest->protocol=protocol;
							pairsList->add(pairsList,pair);
							isProtocol=False;
						}
					
					}

				}
			k++;
			}

		isFirstLine=False;
		}else
			{

			p=k=i-c;
			c=i;
			char * headerKey=NULL;
			char * headerValue=NULL;
				while(k<c)
				{
					if(httpRequestString[k]==':' || (httpRequestString[k]==13 && httpRequestString[k+1]==10))
					{

						if(isHeaderKey)
						{
							headerKey=(char * ) malloc((k-p)+1);
							m=0;
							p=p+1;						
							while(p<k)
							{
								headerKey[m]=httpRequestString[p];
								m++;
								p++;
							}
							headerKey[m]='\0';
							isHeaderKey=False;
						}
						if(isHeaderValue)
						{
							headerValue=(char * ) malloc((c-k));
							m=0;
							k=c;
							p=p+2;						
							while(p<k)
							{
								headerValue[m]=httpRequestString[p];
								m++;
								p++;
							}
							headerValue[m]='\0';
							isHeaderValue=False;

						}
						if(isHeaderValue ==False && isHeaderKey==False)
						{

							Pair *pair =(Pair *) malloc(sizeof(Pair));
							pair->key=headerKey;
							pair->value=headerValue;
							pairsList->add(pairsList,pair);
							isHeaderKey=True;
							isHeaderValue=True;							
						}
					
					}
				k++;
				}
			
			}

	i+=1;
	c=0;
	}else
	{
		c++;
		i++;		
	}
}
httpRequest->requestHeaders=pairsList;
httpRequest->isQueryExist=0;
httpRequest->queryParameters=NULL;
if(isQueryString)
{

		i=0;
		char * dynamicPageURIBuffer=(char *) malloc(queryStringStartFrom+1);
		while(i<queryStringStartFrom-1)
		{
			dynamicPageURIBuffer[i]=httpRequest->uri[i];
			i++;
		}
		dynamicPageURIBuffer[i]='\0';
		httpRequest->dynamicPageURI=dynamicPageURIBuffer;
		//printf("DynamicPageURI: %s\n", httpRequest->dynamicPageURI);

	int uriLen=strlen(httpRequest->uri);
	char querybuffer[uriLen+1];
	int buffInd=0;
		while(queryStringStartFrom<uriLen)
		{
			querybuffer[buffInd]=httpRequest->uri[queryStringStartFrom];
			queryStringStartFrom++;
			buffInd++;
		}

		
querybuffer[buffInd]='\0';  
httpRequest->queryParameters=parseQueryString(querybuffer);
httpRequest->isQueryExist=1;
}
	printf("Parsing ends...\n");
}
LinkedList * parseQueryString(char * queryString)
{
//printf("Query String : %s\n",queryString);
//printf("Parsing the Query String ...\n");

LinkedList queryParameters;
createSinglyList(&queryParameters);

char * queryParameterPair=NULL;
char * token;
const char delim[2]="&";
token=strtok(queryString,delim);
while(token!=NULL)
{
queryParameterPair= (char *) malloc(strlen(token)+1);
strcpy(queryParameterPair,token);
queryParameters.add(&queryParameters,queryParameterPair);
token=strtok(NULL,delim);
}

int parameterListSize=queryParameters.getSize(&queryParameters);
int i=0;
const char delim1[2]="=";
Pair * pair;
char * key,* value;
LinkedList * queryParametersPairs=(LinkedList *) malloc(sizeof(LinkedList));
createSinglyList(queryParametersPairs);
while(i<parameterListSize)
{
	queryParameterPair=queryParameters.get(&queryParameters,i);
	key=strtok(queryParameterPair,delim1);
	value=strtok(NULL,delim1);
	pair=getPair_str_str(key,value);
	decode(key,pair->key);
	decode(value,pair->value);
	queryParametersPairs->add(queryParametersPairs,pair);
	free(queryParameterPair);
	i++;
}
deleteSinglyList(&queryParameters);
parameterListSize=queryParametersPairs->getSize(queryParametersPairs);

i=0;
while(i<parameterListSize)
{
pair=(Pair *) queryParametersPairs->get(queryParametersPairs,i);
key=(char *) pair->key;
value=(char *) pair->value;
printf("[Key : %s ,", key);
printf("value: %s]\n", value);
i++;
}

return queryParametersPairs;
}
