#include "pfs/cxx/cxx98/mutex.hpp"

namespace pfs {

mutex::mutex () 
{
    pthread_mutex_init(& _mutex, 0);
}

mutex::~mutex ()
{
    pthread_mutex_destroy(& _mutex); 
}

void mutex::lock ()
{
    pthread_mutex_lock(& _mutex);
}

bool mutex::try_lock ()
{
    return ! pthread_mutex_trylock(& _mutex);
}

void mutex::unlock ()
{
    pthread_mutex_unlock(& _mutex);
}


recursive_mutex::recursive_mutex ()
{
	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(& mutex_attr);
	pthread_mutexattr_settype(& mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(& _mutex, & mutex_attr);
}

recursive_mutex::~recursive_mutex ()
{
    pthread_mutex_destroy(& _mutex);
}

void recursive_mutex::lock ()
{
    pthread_mutex_lock(& _mutex);
}

bool recursive_mutex::try_lock ()
{
    return ! pthread_mutex_trylock(& _mutex);
}
    
void recursive_mutex::unlock ()
{
    pthread_mutex_unlock(& _mutex);
}

}