#include"..\include\headers.h"
typedef struct Pair_def
{
	void * key;
	void * value;
}Pair;
void releasePair(Pair *pair);
Pair * getPair_str_str(char *,char *);
Pair * getPair_int_str(int arg1,char * arg2);

void releasePair(Pair *pair)
{
	free(pair->key);
	free(pair->value);
}

Pair * getPair_int_str(int arg1,char * arg2)
{
	int * key=(int *) malloc(sizeof(int));
	char * value=(char *) malloc(strlen(arg2)+1);
	*key=arg1;
	strcpy(value,arg2);
	Pair * pair=(Pair *) malloc(sizeof(Pair));
	pair->key=(void *) key;
	pair->value=(void *) value;
return pair;
}

Pair * getPair_str_str(char * arg1,char * arg2)
{
char * str1=(char *) malloc(strlen(arg1)+1);
char * str2=(char *) malloc(strlen(arg2)+1);
strcpy(str1,arg1);
strcpy(str2,arg2);
Pair * pair=(Pair *) malloc(sizeof(Pair));
pair->key=(void *) str1;
pair->value=(void *) str2;
return pair;
}