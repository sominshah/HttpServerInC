#ifndef PairDefination
#define PairDefination
#include"headers.h"
typedef struct Pair_def
{
	void * key;
	void * value;
}Pair;
void releasePair(Pair *pair);
Pair * getPair_str_str(char *,char *);
Pair * getPair_int_str(int,char *);
#endif