#include <mixer.h>

#ifdef _WIN32
#else
#include <pthread.h>

static void* wrapper(void* arg){
	void** args = (void**)arg;
	mixer_thread_handler_t handler = args[0];
	
	handler(args[1]);

	free(args);

	return NULL;
}

void* mixer_thread_create(mixer_thread_handler_t handler, void* arg){
	pthread_t* t = malloc(sizeof(*t));
	void** args = malloc(sizeof(*args) * 2);

	args[0] = handler;
	args[1] = arg;

	pthread_create(t, NULL, wrapper, args);

	return t;
}

void mixer_thread_destroy(void* handle){
	void* value;

	pthread_join(handle, &value);

	free(handle);
}
#endif
