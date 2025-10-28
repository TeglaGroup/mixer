#include <mixer.h>

mixer_t* mixer_open_device(void){
	mixer_t* m = malloc(sizeof(*m));
	
	memset(m, 0, sizeof(*m));

	m->stop = 0;
	m->mutex = mixer_mutex_create();

	if((m->driver = mixerdrv_open_device(m)) == NULL){
		free(m);
		return NULL;
	}

	return m;
}

void mixer_close_device(mixer_t* handle){
	mixer_mutex_lock(handle->mutex);
	handle->stop = 1;
	mixer_mutex_unlock(handle->mutex);

	mixerdrv_close_device(handle);
	mixer_mutex_destroy(handle->mutex);
}

mixer_audio_t* mixer_new_audio(mixer_t* handle){
	mixer_audio_t* a = malloc(sizeof(*a));

	memset(a, 0, sizeof(*a));

	a->stopped = 1;
	a->loop = 0;
	a->volume = 1;

	mixer_mutex_lock(handle->mutex);
	if(handle->audios == NULL){
		handle->audios = a;
	}else{
		mixer_audio_t* l = handle->audios;
		while(l->next != NULL) l = l->next;
		l->next = a;
	}
	mixer_mutex_unlock(handle->mutex);

	return a;
}

void mixer_destroy_audio(mixer_t* handle, mixer_audio_t* audio){
	mixer_audio_t* l = handle->audios;
	
	mixer_mutex_lock(handle->mutex);
	if(l == audio){
		l = l->next;
	}else{
		while(l->next != audio){
			l = l->next;
		}
		l->next = l->next->next;
	}
	mixer_mutex_unlock(handle->mutex);

	if(audio->destroy != NULL) audio->destroy(audio);
	free(audio);
}

void mixer_mix(mixer_t* handle, void* buffer, int frames){
	mixer_audio_t* l;
	double* r = malloc(sizeof(*r) * frames * 2);
	short* sr = buffer;
	int i;

	for(i = 0; i < frames * 2; i++){
		r[i] = 0;
	}

	memset(buffer, 0, frames * 2 * 2);

	mixer_mutex_lock(handle->mutex);
	l = handle->audios;
	while(l != NULL){
		if(!l->stopped){
			if(l->read_frame != NULL){
				double* t = malloc(frames * sizeof(*t) * 2);

				if(l->read_frame(l, t, frames) < frames){
					if(l->loop){
						if(l->reset != NULL) l->reset(l);
					}else{
						l->stopped = 1;
					}
				}

				for(i = 0; i < frames * 2; i++){
					r[i] += t[i] * l->volume;
				}

				free(t);
			}
		}
		l = l->next;
	}
	mixer_mutex_unlock(handle->mutex);

	for(i = 0; i < frames * 2; i++){
		if(r[i] <= -1.25){
			r[i] = -0.984375;
		}else if(r[i] >= 1.25){
			r[i] = 0.984375;
		}else{
			r[i] = 1.1 * r[i] - 0.2 * r[i] * r[i] * r[i];
		}
		sr[i] = r[i] * 32767;
	}

	free(r);
}

void mixer_audio_start(mixer_t* handle, mixer_audio_t* audio){
	mixer_mutex_lock(handle->mutex);
	audio->stopped = 0;
	mixer_mutex_unlock(handle->mutex);
}

void mixer_audio_set_loop(mixer_t* handle, mixer_audio_t* audio, int toggle){
	mixer_mutex_lock(handle->mutex);
	audio->loop = toggle;
	mixer_mutex_unlock(handle->mutex);
}

void mixer_audio_set_volume(mixer_t* handle, mixer_audio_t* audio, double volume){
	mixer_mutex_lock(handle->mutex);
	audio->volume = volume;
	mixer_mutex_unlock(handle->mutex);
}
