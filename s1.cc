#include "fcgi_stdio.h"

#include <stdlib.h>
#include <stdio.h>

redis_t	g_redis;

static void
loop(redis_t* r)
{
	while(FCGI_Accept() >= 0) {
		printf("hello");	
	}
}

static int
redis_init()
{

}

int
main(int argc, char* argv[])
{
	if (redis_init()) {
		perror(NULL);
		exit(0);
	}
	
	loop(g_redis);
	
	return 0;
}