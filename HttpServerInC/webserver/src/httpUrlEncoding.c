#include"..\include\headers.h"
char rfc3986[256] = {0};
char html5[256] = {0};
void storeDelims();
int decode(const char *s, char *dec);
void encode(const char *s, char *enc, char *tb);
int ishex(int x);
 
/* caller responsible for memory */
void storeDelims()
{
int i=0;
	for (i = 0; i < 256; i++)
	 {
rfc3986[i] = isalnum(i)||i == '~'||i == '-'||i == '.'||i == '_' ? i : 0;
html5[i] = isalnum(i)||i == '*'||i == '-'||i == '.'||i == '_' ? i : (i == ' ') ? '+' : 0;
	}
}

void encode(const char *s, char *enc, char *tb)
{
storeDelims();

	for (; *s; s++) {
		if (tb[*s]) sprintf(enc, "%c", tb[*s]);
		else        sprintf(enc, "%%%02X", *s);
		while (*++enc);
	}
}
int ishex(int x)
{
return	(x >= '0' && x <= '9')	|| (x >= 'a' && x <= 'f') || (x >= 'A' && x<= 'F');
}
int decode(const char *s, char *dec)
{
	char *o;
	const char *end = s + strlen(s);
	int c;
 	for (o = dec; s <= end; o++) 
	{
		c = *s++;
		if (c == '+') c = ' ';
		else if (c == '%' && (	!ishex(*s++)	|| !ishex(*s++)	|| !sscanf(s - 2, "%2x", &c))) return -1;
 		if (dec) *o = c;
	}
 
	return o - dec;
}