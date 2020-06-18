/* This code is implemented By Somin Ali */
#include<stdio.h>
#include"..\include\httpserver.h"
#include<malloc.h>
void myMethod(HttpRequest *httpRequest,HttpResponse * httpResponse)
{
char * name=httpRequest->getQueryParameter(httpRequest,"name");
//printf("name :(%s)",name);
httpResponse->setContentType(httpResponse,"text/html");
Writer * writer=getWriter();
writer->append(writer,"<!DOCTYPE html>");
writer->append(writer,"<html>");
writer->append(writer,"<title>W3.CSS Template</title>");
writer->append(writer,"<meta charset='UTF-8'>");
writer->append(writer,"<style>");
writer->append(writer,"html,body,h1,h2,h3,h4,h5,h6 {font-family: 'Roboto', sans-serif}");
writer->append(writer,"</style>");
writer->append(writer,"<body class='w3-light-grey'>");
writer->append(writer,"<div class='w3-content w3-margin-top' style='max-width:1400px;'>");
writer->append(writer,"<H1>My Method</H1>");
writer->append(writer,"</div>");
writer->append(writer,"<footer class='w3-container w3-teal w3-center w3-margin-top'>");
  writer->append(writer,"<p>Find me on social media.</p>");
  writer->append(writer,"<i class='fa fa-facebook-official w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-instagram w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-snapchat w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-pinterest-p w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-twitter w3-hover-opacity'></i>");
writer->append(writer,"<i class='fa fa-linkedin w3-hover-opacity'></i>");
writer->append(writer,"<p>Powered by <a href='https://www.w3schools.com/w3css/default.asp' >w3.css</a></p>");
writer->append(writer,"</footer>");
writer->append(writer,"</body>");
writer->append(writer,"</html>");
httpResponse->addWriter(httpResponse,writer);
}


void newMethod(HttpRequest *httpRequest,HttpResponse * httpResponse)
{
char * name=httpRequest->getQueryParameter(httpRequest,"nm");
char * sex=httpRequest->getQueryParameter(httpRequest,"sex");
char * city=httpRequest->getQueryParameter(httpRequest,"ct");

printf("\nname :(%s)",name);
printf("\nsex :(%s)",sex);
printf("\ncity :(%s)",city);

httpResponse->setContentType(httpResponse,"text/html");
Writer * writer=getWriter();
writer->append(writer,"<!DOCTYPE html>");
writer->append(writer,"<html>");
writer->append(writer,"<title>W3.CSS Template</title>");
writer->append(writer,"<meta charset='UTF-8'>");
writer->append(writer,"<style>");
writer->append(writer,"html,body,h1,h2,h3,h4,h5,h6 {font-family: 'Roboto', sans-serif}");
writer->append(writer,"</style>");
writer->append(writer,"<body class='w3-light-grey'>");
writer->append(writer,"<div class='w3-content w3-margin-top' style='max-width:1400px;'>");
writer->append(writer,"<H1>New Method</H1>");
writer->append(writer,"</div>");
writer->append(writer,"<footer class='w3-container w3-teal w3-center w3-margin-top'>");
  writer->append(writer,"<p>Find me on social media.</p>");
  writer->append(writer,"<i class='fa fa-facebook-official w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-instagram w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-snapchat w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-pinterest-p w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-twitter w3-hover-opacity'></i>");
  writer->append(writer,"<i class='fa fa-linkedin w3-hover-opacity'></i>");
  writer->append(writer,"<p>Powered by <a href='https://www.w3schools.com/w3css/default.asp' >w3.css</a></p>");
writer->append(writer,"</footer>");
writer->append(writer,"</body>");
writer->append(writer,"</html>");
httpResponse->addWriter(httpResponse,writer);
}

int main()
{
HttpServer httpServer;
init(&httpServer);
httpServer.addDynamicResource(&httpServer,"/three.com/processform",myMethod);
httpServer.addDynamicResource(&httpServer,"/three.com/newForm",newMethod);
int ports[1]={80};
httpServer.startServer(&httpServer,ports,1);
httpServer.waitingForServer(&httpServer);
return 0;
}
