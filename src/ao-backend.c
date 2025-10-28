#include <mixer.h>

#include <ao/ao.h>

static int ao_init = 0;

typedef struct driver {
	ao_sample_format format;
	ao_device* device;
	void* thread;
} driver_t;

#define MS 15

static void wrapper(void* arg){
	mixer_t* m = arg;

	while(1){
		driver_t* d;

		mixer_mutex_lock(m->mutex);
		d = m->driver;
		mixer_mutex_unlock(m->mutex);

		if(d != NULL){
			void* frames;
			const int frame = MIXER_RATE * MS / 1000;

			frames = malloc(frame * 2 * 2);

			mixer_mix(m, frames, frame);

			ao_play(d->device, frames, frame * 2 * 2);

			free(frames);
		}

		mixer_mutex_lock(m->mutex);
		if(m->stop){
			mixer_mutex_unlock(m->mutex);
			break;
		}
		mixer_mutex_unlock(m->mutex);
	}
}

void* mixerdrv_open_device(mixer_t* handle){
	driver_t* d = malloc(sizeof(*d));

	if(!ao_init){
		ao_initialize();
	}
	ao_init++;

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

	d->thread = mixer_thread_create(wrapper, handle);

	return d;
}

void mixerdrv_close_device(mixer_t* handle){
	driver_t* d = handle->driver;

	ao_close(d->device);
	free(d);
	
	ao_init--;
	if(ao_init == 0){
		ao_shutdown();
	}
}
