#include <mixer.h>

#ifdef _WIN32
/* TODO */
#else
#include <pthread.h>

void* mixer_mutex_create(void){
	pthread_mutex_t* m = malloc(sizeof(*m));

	pthread_mutex_init(m, NULL);

	return m;
}

void mixer_mutex_lock(void* handle){
	pthread_mutex_lock(handle);
}

void mixer_mutex_unlock(void* handle){
	pthread_mutex_unlock(handle);
}

void mixer_mutex_destroy(void* handle){
	pthread_mutex_destroy(handle);
	free(handle);
}
#endif
