#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

bool ReadCFG(config_t * cfg, char * filename) {
	FILE * cfgfile;
	char opt[50];
	char val[50];
	char buf[256];

	cfgfile = fopen(filename, "r");
	if(cfgfile == NULL)
		return false;
	while(fgets(buf, sizeof(buf), cfgfile) != NULL) {
		if(buf[0] == '#')
			continue;
		if(strstr(buf, "server ="))
			strcpy(cfg->client.host, getVal(buf));
		if(strstr(buf, "port ="))
			cfg->port = atoi(getVal(buf));
		if(strstr(buf, "domain ="))
			strcpy(cfg->client.domain, getVal(buf));
		if(strstr(buf, "zones ="))
			strcpy(cfg->server.zonedir, getVal(buf));
		if(strstr(buf, "log ="))
			strcpy(cfg->logfile, getVal(buf));
		if(strstr(buf, "interval ="))
			cfg->client.interval = atoi(getVal(buf));
		if(strstr(buf, "login ="))
			strcpy(cfg->client.username, getVal(buf));
		if(strstr(buf, "pass ="))
			strcpy(cfg->client.password, getVal(buf));
	}
	fclose(cfgfile);
	return true;
}
char * getVal(char * str) {
	char * newstr = str;

	while(*str) {
		newstr = ++str;
		if(isspace(*str) && (isalpha(*(str+1)) || isdigit(*(str+1)) || *(str+1) == '/'))
			break;
	}
	str = newstr;
	*(str+(strlen(str)-1)) = '\0';
	return ++str;
}