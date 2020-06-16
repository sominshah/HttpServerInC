#ifndef AppDeployerDefination
#define AppDeployerDefination
#include"headers.h"
#include"httpApplication.h"
typedef struct ApplicationDeployer_def
{
LinkedList * applications;	
int numberOfApplications;
int (*deploy)();
Resource * (*getResource) (struct ApplicationDeployer_def *,char *);
void (*addDynamicResource)(struct ApplicationDeployer_def *,const char *,void (*)(HttpRequest *,HttpResponse *));
}ApplicationDeployer;
Resource * ApplicationDeployer_getResource(ApplicationDeployer *,char * path);
void ApplicationDeployer_addDynamicResource(ApplicationDeployer *,const char *,void (*sourceInjector)(HttpRequest *,HttpResponse *));
int ApplicationDeployer_Deploy();
int ApplicationDeployer_isDirectory(const char *path);
ApplicationDeployer * getApplicationDeployer();

#endif