#include <mixer.h>

#include <sndfile.h>

typedef struct sndfile {
	SNDFILE* sf;
	SF_INFO info;
} sndfile_t;

static void destroy(mixer_audio_t* audio){
	sndfile_t* s = audio->opaque;

	sf_close(s->sf);
}

static int read_frame(mixer_audio_t* audio, double* buffer, int frames){
	sndfile_t* s = audio->opaque;
	
	return sf_read_double(s->sf, buffer, frames * 2) / 2;
}

mixer_audio_t* mixer_open_file(mixer_t* mixer, const char* filename, int loop){
	mixer_audio_t* a;
	sndfile_t* s = malloc(sizeof(*s));

	s->sf = sf_open(filename, SFM_READ, &s->info);
	if(s->sf == NULL){
		free(s);
		return NULL;
	}

	a = mixer_new_audio(mixer);
	
	mixer_mutex_lock(mixer->mutex);
	a->opaque = s;
	a->destroy = destroy;
	a->read_frame = read_frame;
	a->loop = loop;
	a->stopped = 0;
	mixer_mutex_unlock(mixer->mutex);

	return a;
}
