#include <mixer.h>

#include <stdlib.h>
#include <ao/ao.h>

static int ao_init = 0;

typedef struct driver {
	ao_sample_format format;
	ao_device* device;
} driver_t;

void* mixerdrv_open_device(mixer_t* handle){
	driver_t* d = malloc(sizeof(*d));

	if(!ao_init){
		ao_init = 1;
		ao_initialize();
	}

	d->format.bits = 16;
	d->format.rate = MIXER_RATE;
	d->format.channels = 2;
	d->format.byte_format = AO_FMT_NATIVE;
	d->format.matrix = NULL;

	d->device = ao_open_live(ao_default_driver_id(), &d->format, NULL);
	if(d->device == NULL){
		free(d);
		return NULL;
	}

	return d;
}

void mixerdrv_close_device(mixer_t* handle){
	driver_t* d = handle->driver;

	ao_close(d->device);
	free(d);
}
