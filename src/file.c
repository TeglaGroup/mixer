#include <mixer.h>

#include <sndfile.h>
#include <xmp.h>

typedef struct fileaudio {
	int use_xmp;

	SNDFILE* sf;
	SF_INFO info;

	xmp_context xmp;
} fileaudio_t;

static void destroy(mixer_audio_t* audio){
	fileaudio_t* s = audio->opaque;

	if(s->use_xmp){
		xmp_end_player(s->xmp);
		xmp_release_module(s->xmp);
		xmp_free_context(s->xmp);
	}else{
		sf_close(s->sf);
	}
}

static int read_frame(mixer_audio_t* audio, double* buffer, int frames){
	fileaudio_t* s = audio->opaque;
	if(s->use_xmp){
		short* r = malloc(frames * sizeof(*r) * 2);
		int i;

		if(xmp_play_buffer(s->xmp, r, frames * 2 * 2, 0) < 0){
			free(r);
			return 0;
		}else{

			for(i = 0; i < 2 * frames; i++){
				buffer[i] = r[i] / 32767.0;
			}

			free(r);

			return frames;
		}
	}else{
		double* b = malloc(frames * sizeof(*b) * 2);
		int samp = sf_read_double(s->sf, b, frames * s->info.samplerate / MIXER_RATE * 2) / 2;
		int after = samp * MIXER_RATE / s->info.samplerate;
		int i;

		for(i = 0; i < after; i++){
			buffer[2 * i + 0] = b[2 * (i * s->info.samplerate / MIXER_RATE) + 0];
			buffer[2 * i + 1] = b[2 * (i * s->info.samplerate / MIXER_RATE) + 1];
		}
		free(b);

		return after;
	}
}

static void reset(mixer_audio_t* audio){
	fileaudio_t* s = audio->opaque;

	if(s->use_xmp){
		xmp_seek_time(s->xmp, 0);
	}else{
		sf_seek(s->sf, 0, SEEK_SET);
	}
}

mixer_audio_t* mixer_open_file(mixer_t* mixer, const char* filename){
	mixer_audio_t* a;
	fileaudio_t* s = malloc(sizeof(*s));

	if (access(filename, F_OK) != 0) {
		fprintf(stderr, "[TeglaMixer ERROR] file not found: %s\n", filename);
		free(s);
		return NULL;
	}

	s->use_xmp = 0;
	s->xmp = xmp_create_context();
	if(xmp_load_module(s->xmp, filename) < 0){
		xmp_free_context(s->xmp);

		s->sf = sf_open(filename, SFM_READ, &s->info);
		if(s->sf == NULL){
			free(s);
			return NULL;
		}
	}else{
		s->use_xmp = 1;
		xmp_start_player(s->xmp, MIXER_RATE, 0);
	}

	a = mixer_new_audio(mixer);
	
	a->opaque = s;
	a->destroy = destroy;
	a->read_frame = read_frame;
	a->reset = reset;

	return a;
}
