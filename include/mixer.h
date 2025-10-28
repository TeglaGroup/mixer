#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdlib.h>
#include <string.h>

/**
 * NOTE!!!
 *
 * - Mixer always expects stereo 16-bits
 * - "frames" mean the amount of s16 pair, so 5 "frames" would
 *   be 5*sizeof(*frames)*2 bytes
 */
#define MIXER_RATE 48000

typedef struct mixer mixer_t;
typedef struct mixer_audio mixer_audio_t;
typedef void(*mixer_thread_handler_t)(void*);

struct mixer {
	int stop;
	void* driver;
	void* mutex;

	mixer_audio_t* audios;
};

struct mixer_audio {
	void* opaque;
	int loop;
	int stopped;
	double volume;

	void(*destroy)(mixer_audio_t* audio);
	int(*read_frame)(mixer_audio_t* audio, double* buffer, int frames);
	void(*reset)(mixer_audio_t* audio);

	mixer_audio_t* next;
};

/* mixer.c */
mixer_t* mixer_open_device(void);
void mixer_close_device(mixer_t* handle);
void mixer_mix(mixer_t* handle, void* buffer, int frames);
mixer_audio_t* mixer_new_audio(mixer_t* handle);
void mixer_destroy_audio(mixer_t* handle, mixer_audio_t* audio);
void mixer_audio_start(mixer_t* handle, mixer_audio_t* audio);
void mixer_audio_set_loop(mixer_t* handle, mixer_audio_t* audio, int toggle);
void mixer_audio_set_volume(mixer_t* handle, mixer_audio_t* audio, double volume);

/* file.c */
mixer_audio_t* mixer_open_file(mixer_t* mixer, const char* filename);

/* mutex.c */
void* mixer_mutex_create(void);
void mixer_mutex_lock(void* handle);
void mixer_mutex_unlock(void* handle);
void mixer_mutex_destroy(void* handle);

/* thread.c */
void* mixer_thread_create(mixer_thread_handler_t handler, void* arg);
void mixer_thread_destroy(void* handle);

/* driver */
void* mixerdrv_open_device(mixer_t* handle);
void mixerdrv_close_device(mixer_t* handle);

#endif
