#include "redis.h"
#include <string.h>

#define freeReplay(r) freeReplyObject(r);r = NULL


redis_t::redis_t()
	:m_redis(NULL),
	m_reply(NULL)
{
}

redis_t::~redis_t()
{
	if (m_reply) freeReplyObject(m_reply);
	if (m_redis) redisFree(m_redis);
}

int 
redis_t::Connect(const char* host, 
				 unsigned int port, 
				 unsigned short timeout)
{
	if(m_reply) {
		freeReplyObject(m_reply);
		m_reply = NULL;
	}

	if (m_redis) {
		redisFree(m_redis);
		m_redis = NULL;
	}

	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;

    m_redis = redisConnectWithTimeout(host, port, tv);
	
    if(m_redis->err) {
		redisFree(m_redis);
		m_redis = NULL;
		
		return -1;
    }
	
    return 0;
}

int 
redis_t::Get(const char* key, char* value, unsigned int len)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "GET %s", key));
    if(m_reply)
    {
        int result = 0;
			
        if (1 == m_reply->type) {
			snprintf(value, len, "%s", m_reply->str);
        } else if (m_reply->type == REDIS_REPLY_ERROR){
			result = -1;
		}
        
        freeReplyObject(m_reply);
		m_reply = NULL;
		
        return result;
    }

    return -1;
}

int 
redis_t::Set(const char* key, const char* value)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "SET %s %s", key, value));
    if(m_reply) {
        int result = (m_reply->type == REDIS_REPLY_NIL) ? -1 : 0;

		freeReplyObject(m_reply);
		m_reply = NULL;
		
		return result;
    }
    
	return -1;
}

int redis_t::Expire(const char* key, int expire)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "EXPIRE %s %d", key, expire));
    if(m_reply) {
        int result = (m_reply->type == REDIS_REPLY_NIL) ? -1 : 0;

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
    }
    return -1;
}

int 
redis_t::Enqueue(const char* queue, const char* value)
{
	m_reply = static_cast<redisReply*>(redisCommand(m_redis, "rpush %s %s", queue, value));

	if (m_reply) {
		int result = (m_reply->type == REDIS_REPLY_NIL) ? -1 : 0;

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
	}

	return -1;
}

int
redis_t::Dequeue(const char* queue, char* value, unsigned int len)
{
	m_reply = static_cast<redisReply*>(redisCommand(m_redis, "lpop %s", queue));

	if (m_reply) {
		int result = 0;

		if (m_reply->type == REDIS_REPLY_STRING) {
			snprintf(value, len, "%s", m_reply->str);
		} else if (m_reply->type == REDIS_REPLY_ERROR) {
			result = -1;
		}

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
	}

	return -1;
}

bool
redis_t::IsActived()
{
	if (m_redis == NULL) return false;

	m_reply = static_cast<redisReply*>(redisCommand(m_redis, "ping"));

	if (m_reply) {
		if (m_reply->type == REDIS_REPLY_STATUS) {
			if(strcmp("PONG", m_reply->str) == 0) {
				freeReplyObject(m_reply);
				m_reply = NULL;
				
				return true;
			}
		}

		freeReplyObject(m_reply);
		m_reply = NULL;
	}

	return false;
}

int
redis_t::S_IsMember(const char* key, const char* value)
{
	m_reply = static_cast<redisReply*>(redisCommand(m_redis, "sismember %s %s", key, value));

	if (m_reply) {
		int result = 0;

		if (m_reply->type == 3) {
			result = m_reply->integer == 1 ? 1 : 0;
		}

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
	}

	return -1;
}

int
redis_t::S_IsMember(const char* key, const int value)
{
	m_reply = static_cast<redisReply*>(redisCommand(m_redis, "sismember %s %d", key, value));

	if (m_reply) {
		int result = 0;

		if (m_reply->type == REDIS_REPLY_INTEGER) {
			result = m_reply->integer == 1 ? 1 : 0;
		}

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
	}

	return -1;	
}

int redis_t::HSet(const char* key,const int field, const char* value)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "HSET %s %d %s", key,field,value));

    if(m_reply) {
		freeReplyObject(m_reply);
		m_reply = NULL;
		
		return 0;
    }

    return -1;
}

int redis_t::HGet(const char* key, const int field, char* value, unsigned int len)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "HGET %s %d ", key, field));
    int result = 0;
    
    if(m_reply) {
        if(m_reply->type == REDIS_REPLY_STRING) {
			snprintf(value, len, "%s", m_reply->str);
        }

		freeReplyObject(m_reply);
		m_reply = NULL;

		return result;
    }

    return -1;
}

int redis_t::Del(const char* key)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "DEL %s ", key));
    if(m_reply) {

		freeReplyObject(m_reply);
		m_reply = NULL;
		
		return 1;
    }
    
    return 0;
}

int redis_t::HDel(const char* key,const int field)
{
    m_reply = reinterpret_cast<redisReply*>(redisCommand(m_redis, "HDEL %s %d ", key,field));
    if(m_reply) {
		freeReplyObject(m_reply);
		m_reply = NULL;
		
		return 0;
    }
    
    return -1;
}

