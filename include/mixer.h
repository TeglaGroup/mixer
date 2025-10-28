#ifndef __MIXER_H__
#define __MIXER_H__

/**
 * NOTE!!!
 *
 * Mixer always expects stereo 16-bits
 */
#define MIXER_RATE 48000

typedef struct mixer {
	void* driver; /* driver specific area */
	void* mutex;
} mixer_t;

/* mixer.c */
mixer_t* mixer_open_device(void);
void mixer_close_device(mixer_t* handle);

/* mutex.c */
void* mixer_mutex_create(void);
void mixer_mutex_lock(void* handle);
void mixer_mutex_unlock(void* handle);
void mixer_mutex_destroy(void* handle);

/* driver */
void* mixerdrv_open_device(mixer_t* handle);
void mixerdrv_close_device(mixer_t* handle);

#endif
