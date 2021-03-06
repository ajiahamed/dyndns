#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_
#include <mysql.h>
#include <netinet/in.h>

typedef struct remotedata {
	char * login;
	char * pass;
	char * subdomain;
	struct in_addr client_ip_addr;
} REMOTEDATA_t;
typedef struct db_userdata {
	int id;
	char * login;
	char * md5;
	char * subdomain;
	char * email;
	char * serial;
	char * domstatus;
	int active;
} DB_USERDATA_t;
struct conn_st {
	int fd;
	struct in_addr client_ip;
};
struct subdomain_st {
	char * sub;
	char * dom;
	int len;
};
MYSQL_RES * queryUserData(MYSQL * dbh, char * login, int logger);
REMOTEDATA_t * readCLientData(int sockfd, int logger);
struct conn_st * clientConn(int sock);
int bindToInterface(int portno);
int existZoneFile(char * filepath);
struct subdomain_st * explodeDomain(char * clientDomain);
int existEntry(char * what, char * where);
char * stripSerialNo(char * input);
char * newSerialNo(char * serial);
int updateZone(char * subdomain, char * ipaddr, char * serial_from_db, char * file, int logger);
void InitConnData(REMOTEDATA_t * conn);
void InitDBData(DB_USERDATA_t * db);
int dbUpdate(MYSQL * dbh, DB_USERDATA_t * data, struct subdomain_st * domain, char * ipaddr, char * timestamp_s);
int appendDomain(char * zonepath, char * subdomain, char * ipaddr);
#endif
