#ifndef __REDIS_H__
#define __REDIS_H__

#include "hiredis/hiredis.h"

class redis_t {
public:
    redis_t();
    ~redis_t();

public:
    int Connect(const char* host, unsigned int port, unsigned short timeout = 0);
    int Get(const char* key, char* value, unsigned int len);
    int Set(const char* key, const char* value);
    int Expire(const char* key, int expire);

	int Enqueue(const char* queue, const char* value);
	int Dequeue(const char* queue, char* value, unsigned int len);

	int S_IsMember(const char* key, const char* value);
	int S_IsMember(const char* key, const int value);
	
    int HSet(const char* key, const int field, const char* value);
    int HGet(const char* key, const int field, char* value, unsigned int len);
    int Del(const char* key);
    int HDel(const char* key, const int field);
    
	bool IsActived();
private:
    redisContext*	m_redis;	
    redisReply*		m_reply;	
};

#endif
