#include <mixer.h>

#include <stdlib.h>
#include <string.h>

mixer_t* mixer_open_device(void){
	mixer_t* m = malloc(sizeof(*m));
	
	memset(m, 0, sizeof(*m));

	if((m->driver = mixerdrv_open_device(m)) == NULL){
		free(m);
		return NULL;
	}

	return m;
}

void mixer_close_device(mixer_t* handle){
	mixerdrv_close_device(handle);
}
