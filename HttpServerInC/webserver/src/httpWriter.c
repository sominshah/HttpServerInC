#include"..\include\headers.h"
typedef struct Writer_def
{
LinkedList * contentList;
long contentLength;
void (*append)(struct Writer_def *,char *);
}Writer;
void Writer_append(Writer * writer,char * data);
Writer * getWriter();
Writer * getWriter()
{
Writer * writer=(Writer *) malloc(sizeof(Writer));
writer->append=Writer_append;
writer->contentList=(LinkedList *) malloc(sizeof(LinkedList));
createSinglyList(writer->contentList);
writer->contentLength=0;
return writer;
}
void Writer_append(Writer * writer,char * data)
{
int len=strlen(data);
char * str=(char *) malloc(len+1);
strcpy(str,data);
writer->contentList->add(writer->contentList,str);
writer->contentLength+=len;
}
