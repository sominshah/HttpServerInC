#ifndef HttpURLEncoding
#define HttpURLEncoding
void storeDelims();
void encode(const char *s, char *enc, char *tb);
int ishex(int x);
int decode(const char *s, char *dec);
#endif