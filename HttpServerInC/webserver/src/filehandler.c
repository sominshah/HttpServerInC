#include"..\include\headers.h"
#include"..\include\httpApplication.h"
typedef struct ApplicationDeployer_def
{
LinkedList * applications;	
int numberOfApplications;
int (*deploy)();
Resource * (*getResource) (struct ApplicationDeployer_def *,char *);
void (*addDynamicResource)(struct ApplicationDeployer_def *,const char *,void (*)(HttpRequest *,HttpResponse *));
}ApplicationDeployer;

ApplicationDeployer * applicationDeployer=NULL;
Resource * ApplicationDeployer_getResource(ApplicationDeployer *,char * path);
int ApplicationDeployer_Deploy();
int ApplicationDeployer_isDirectory(const char *path);
ApplicationDeployer * getApplicationDeployer();
void ApplicationDeployer_addDynamicResource(ApplicationDeployer *,const char *,void (*sourceInjector)(HttpRequest *,HttpResponse *));

void ApplicationDeployer_addDynamicResource(ApplicationDeployer * applicationDeployer,const char * uri,void (*sourceInjector)(HttpRequest *,HttpResponse *))
{
//printf("Adding dynamic Resource...\n");
char * uriBuffer=NULL;
LinkedList * applications=applicationDeployer->applications;
int nos=applications->getSize(applications);
int j=0,i=0,ctxlen=0;
Application * application=NULL;
Resource * resource=NULL;
char * cxtName=NULL;
Boolean match=True;
int pathLength=strlen(uri);
while(i<nos)
{
	match=True;
application=applications->get(applications,i);
cxtName=application->contextName;
//printf("cxt(%s)\n",cxtName);
	j=0;
	ctxlen=strlen(cxtName);
	while(j<ctxlen)
	{
		if(cxtName[j]!=uri[j])
		{
			match=False;
			break;
		}
			
	j++;
	}



		if(j==ctxlen && match==True)
		{
			if(ctxlen==pathLength)
			{
				return;
			}

			//printf("contextName has matched !!!!\n");
					resource=NULL;
					resource=(Resource *) malloc(sizeof(Resource));
					strcpy(resource->type,"dynamic");
					uriBuffer=(char *) malloc(pathLength+1);
					strcpy(uriBuffer,uri);
					resource->resourcePath=uriBuffer;
					resource->loadDynamicContents=sourceInjector;
					application->resources->add(application->resources,resource);
					application->numberOfResources++;
					//printf("Adding resource (%s) in website (%s) \n",resource->resourcePath,cxtName);
					return;

		}



i++;
}
}


Resource * ApplicationDeployer_getResource(ApplicationDeployer * appDeloyer,char * path)
{
//printf("Path validation starts\n  URI: (%s)",path);
if(strcmp(path,"/")==0)
{
path="/ROOT/index.html";
}
int pathLength=strlen(path);
char pathToCompare[pathLength+1];
strcpy(pathToCompare,path);
LinkedList * applications=appDeloyer->applications;
int nos=applications->getSize(applications);
int i=0,j=0,ctxlen=0;
LinkedList * resources=NULL;
Application * application=NULL;
Resource * resource=NULL;
char * cxtName=NULL;
Boolean match=True;
Boolean isOnlyContext=False;
while(i<nos)
{
	match=True;
	cxtName=NULL;
	application=(Application *) applications->get(applications,i);
	cxtName=application->contextName;
	j=0;
	ctxlen=strlen(cxtName);
	while(j<ctxlen)
	{
		if(cxtName[j]!=pathToCompare[j])
		{
			match=False;
			break;
		}
	j++;
	}

if(j==ctxlen && match==True)
	{
		if(ctxlen==pathLength)
		{
		return NULL;
		}
				j=0;
				resource=NULL;
				resources=NULL;
				resources=application->resources;
				//printf("\n <<<<---------resource of: (%s)------------>>>> \n",cxtName);
				while(j<application->numberOfResources)
					{
						resource=(Resource *)resources->get(resources,j); 
				//printf("\n <<<<Resource Path : (%s)------------>>>> \n",resource->resourcePath);
						if(strcmp(pathToCompare,resource->resourcePath)==0)
						{
						return resource;
						}
					j++;
					}
				return NULL;

			
	}
i++;
}
return NULL;
}


ApplicationDeployer * getApplicationDeployer()
{
if(applicationDeployer!=NULL)
{
return applicationDeployer;
}

applicationDeployer=(ApplicationDeployer *) malloc(sizeof(ApplicationDeployer));
applicationDeployer->getResource=ApplicationDeployer_getResource;
applicationDeployer->deploy=ApplicationDeployer_Deploy;
applicationDeployer->addDynamicResource=ApplicationDeployer_addDynamicResource;
applicationDeployer->numberOfApplications=0;
applicationDeployer->applications=(LinkedList *) malloc(sizeof(LinkedList));
createSinglyList(applicationDeployer->applications);
return applicationDeployer;
}

int ApplicationDeployer_Deploy()
{
char * stack[1000];
int i=0;
int top=0;
int stackSize=0;
struct dirent * directoryEntry;  //pointer for directory entry;

stack[top]="../webapps";
stackSize++;
Boolean isWebsite=True;
Application * application=NULL;
char * dirName=NULL;
char * subDirName=NULL;
char * directory=NULL;
char * g=NULL;


Application * websiteStack[1000];
int websiteStackTop=-1;
int websiteStackSize=0;
int k=0,c=0;
Application * website=NULL;

Resource * resource=NULL;
while(stackSize>0)
{
	dirName=stack[top--];
	stackSize--;
	DIR *directoryType= opendir(dirName);
	if(directoryType==NULL)
	{
		printf("Couldn't open given directory named as %s.\n",dirName);
		continue;
	}

	if(websiteStackSize>0)
	{
		if(strcmp(dirName,websiteStack[websiteStackTop]->contextPath)==0)
		{
		website=websiteStack[websiteStackTop];
		websiteStackTop--;
		websiteStackSize--;	
		}

	}

	while((directoryEntry=readdir(directoryType))!=NULL)
	{
			subDirName=directoryEntry->d_name;
			if((strcmp(subDirName,"..")!=0) && (strcmp(subDirName,".")!=0))
			{
				directory=(char *) malloc(strlen(subDirName)+strlen(dirName)+3);
				strcpy(directory,dirName);
				strcat(directory,"/");
				strcat(directory,subDirName);
				if(ApplicationDeployer_isDirectory(directory))
				{						
					if(isWebsite==True)
					{
						application=NULL;
						g=NULL;
						application=(Application *) malloc(sizeof(Application));
						g=(char *) malloc(strlen(subDirName)+1);							
						strcpy(g,subDirName);
						application->name=g;
						application->contextPath=directory;
						g=(char *) malloc(strlen(application->name)+2);
						strcpy(g,"/");
						strcat(g,application->name);
						application->contextName=g;
						applicationDeployer->applications->add(applicationDeployer->applications,application);
						applicationDeployer->numberOfApplications++;
						application->resources=(LinkedList *) malloc(sizeof(LinkedList));
						application->numberOfResources=0;
						createSinglyList(application->resources);
						websiteStack[++websiteStackTop]=application;
						websiteStackSize++;
					}
					stack[++top]=directory;
					stackSize++;
				}else
					{
					resource=NULL;
					resource=(Resource *) malloc(sizeof(Resource));
					strcpy(resource->type,"static");
					strcpy(resource->name,subDirName);
					
					//determining file type
            			char * type=NULL;	
						if(strstr(subDirName,".txt")!=NULL || strstr(subDirName,".html")!=NULL)
						{
							type = "text/html";
						}
						else
						{
							if(strstr(subDirName,".js")!=NULL)
							{
                    		type = "text/javascript";								
                    		}else
                    		{
                    			if(strstr(subDirName,".css")!=NULL)
                    			{

									type = "text/css";
                    			}else
                    			{

                    				if(strstr(subDirName,".jpg"))
                    				{
                    				type = "image/jpeg";
                    				}else
                    				{
			                    		type = "application/octet-stream";
                    				}
                    			}
                    		
                    		}

						}
            			
					strcpy(resource->contentType,type);
					g=(char *) malloc((strlen(website->contextName)+strlen(subDirName))+2);					
					strcpy(g,website->contextName);
					strcat(g,"/");
					strcat(g,subDirName);					
					resource->resourcePath=g;
					resource->value=NULL;
					resource->size=0;
					resource->location=directory;
					website->resources->add(website->resources,resource);
					website->numberOfResources++;
					directory=NULL;
						//paths[++pathIndex]=directory;
						}

			}
	
	}
isWebsite=False;
closedir(directoryType);
}
return 0;
}

int ApplicationDeployer_isDirectory(const char *path) 
{
  struct stat statbuf;
	int flag=0;
   if (stat(path, &statbuf) != 0)
   {
       return 0;
   }
flag=S_ISDIR(statbuf.st_mode);
   return flag;
}
