#ifndef DYNSRV_H_
#define DYNSRV_H_
#include <stdbool.h>

typedef struct cfgdata {
	char domain[64];
	char subdomain[16];
	char ip_addr[16];
} cfgdata_t;

int bindToInterface(int portno);
int clientConn(int fd, char * cliaddr);
int readData(int fd, char * domain);
int updateZone(cfgdata_t * cf, char * file);
int updateSerialNo(char * fromfile, char * newserial);
void RandomFilename(char *filename);
void stripSerialNo(char *in, char *out);
bool if_Exist(char *item, char *zfname);
int NewEntry(cfgdata_t * cf, char * file);
int apply(char * tmp_f, char * dst_f, const char * domain);
#endif
