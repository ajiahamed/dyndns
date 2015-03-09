#ifndef DYNDNS_H_
#define DYNDNS_H_
#include <stdbool.h>

typedef struct params {
	char host[50];
	char domain[64];
	int port;
} PARAMS;
bool ReadCFG(PARAMS * cfg, char * filename);
char * getVal(char * str);

#endif /* DYNDNS_H_ */
