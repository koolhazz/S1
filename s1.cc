#include "fcgi_stdio.h"
#include "redis.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


redis_t	g_redis;

static char*
__getParam(const char* qs, char** stop)
{
	char *start, *end;

	start = strchr(qs, '=');
	
	if (start) {
		end = strchr(start, '&');
		if (end == NULL) {
			end = start + strlen(start) + 1;
		}
	}

	if (start && end) {
		*stop = end;
		return start + 1;
	}

	return NULL;
}

static void
loop()
{
	char *start, *stop, *pm, *value, *end;

	start = stop = pm = end = NULL;	
	value = (char*)calloc(1, 8192);

	while(FCGI_Accept() >= 0) {
		printf("Content-type: text/html\r\n\r\n"); //这里必须添加否则会报502的错误
		printf("this is s1\n");
		printf("CT: %s\n", getenv("REQUEST_METHOD"));
		start = getenv("QUERY_STRING");
		end = start + strlen(getenv("QUERY_STRING")) + 1;

		while (start != end) {
			pm = __getParam(start, &stop);
			
			if (pm) {
				memcpy(value, pm, stop - pm);
				printf("Value: %s\n", value);
			}

			start = stop;
			pm = stop = NULL;
			
			g_redis.Enqueue("s1", value);
			
			memset(value, 0, strlen(value));
		}
	}
}

static int
redis_init()
{
	if (g_redis.Connect("192.168.100.167", 4502, 5) == 0) {
		return 0;
	}
	
	return -1;
}


int
main(int argc, char* argv[])
{
	if (redis_init() == -1) {
		perror(NULL);
		exit(-1);
	}
	
	loop();
	
	return 0;
}
