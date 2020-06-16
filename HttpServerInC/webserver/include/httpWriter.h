#ifndef WriterDefination
#define WriterDefination
typedef struct Writer_def
{
LinkedList * contentList;
long contentLength;
void (*append)(struct Writer_def *,char *);
}Writer;
void Writer_append(Writer * writer,char * data);
Writer * getWriter();
#endif