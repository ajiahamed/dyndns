#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "dynsrv.h"
#include "auth.h"

static void splitDomain(char *userdomain, cfgdata_t * cfg);

int ddserv(char * zonedir, int logfd, int sockfd) {
	int cli_fd;
	int authstatus;
	char * source_addr;
	char * client_domain;
	char zonepath[64];
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];
	cfgdata_t cf;
	struct iovec authdata[2];

	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

	strcpy(zonepath, zonedir);
	if ((cli_fd = clientConn(sockfd, source_addr)) < 0) {
		sprintf(logmsg, "%s ERROR: Connection failed from: %s\n", timestamp(t_stamp), source_addr);
		write(logfd, logmsg, strlen(logmsg));
		exit(-1);
	}
	else {
		sprintf(logmsg, "%s INFO: Client: %s connected\n", timestamp(t_stamp), source_addr);
		write(logfd, logmsg, strlen(logmsg));
		strcpy(cf.ip_addr, source_addr);
		if(readAuthData(cli_fd, authdata) != 0) {
			sprintf(logmsg, "%s ERROR: Read authdata failed from: %s\n", timestamp(t_stamp), source_addr);
			write(logfd, logmsg, strlen(logmsg));
			exit(-1);
		}
	}
	authstatus = userauth((char *) authdata[0].iov_base, (char *) authdata[1].iov_base);
	if(authstatus == 0) {
		write(cli_fd, "authok", 7);
		if (readData(cli_fd, client_domain) > 0) {
			splitDomain(client_domain, &cf);
			strcat(zonepath, cf.domain);
		}
		else {
				sprintf(logmsg, "%s ERROR: Read data failed from: %s\n", timestamp(t_stamp), source_addr);
				write(logfd, logmsg, strlen(logmsg));
				exit(-1);
		}
		if(if_Exist(cf.subdomain, zonepath) == true) {
			if(if_Exist(cf.ip_addr, zonepath) == false) {
				if(updateZone(&cf, zonepath)) {
					sprintf(logmsg, "%s INFO: %s IP Address updated\n", timestamp(t_stamp), cf.subdomain);
					write(logfd, logmsg, strlen(logmsg));
				}
				else {
					sprintf(logmsg, "%s ERROR: %s update failed\n", timestamp(t_stamp), cf.subdomain);
					write(logfd, logmsg, strlen(logmsg));
				}
			}
			else {
				NewEntry(&cf, zonepath);
				sprintf(logmsg, "%s INFO: New host added: %s.%s\n", timestamp(t_stamp), cf.subdomain, cf.domain);
				write(logfd, logmsg, strlen(logmsg));
			}
		}
	}
	else if(authstatus == 1) {
		sprintf(logmsg, "%s ERROR: Unknown user: %s\n", timestamp(t_stamp), (char *) authdata[0].iov_base);
		write(logfd, logmsg, strlen(logmsg));
	}
	else if(authstatus == 2) {
		sprintf(logmsg, "%s ERROR: Incorrect password for user: %s\n", timestamp(t_stamp), (char *) authdata[0].iov_base);
		write(logfd, logmsg, strlen(logmsg));
	}
	close(cli_fd);
	free(source_addr);
	free(client_domain);
	exit(1);
}
static void splitDomain(char *userdomain, cfgdata_t * cfg) {
    char * dom;
    char * subdom;

    dom = cfg->domain;
    subdom = cfg->subdomain;

    while(*userdomain) {
            *subdom++ = *userdomain++;
            if(*userdomain == '.') {
                    *subdom = '\0';
                    break;
            }
    }
    while(*++userdomain)
            *dom++ = *userdomain;
    *dom = '\0';
}
