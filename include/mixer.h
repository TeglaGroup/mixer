#ifndef __MIXER_H__
#define __MIXER_H__

typedef struct mixer {
	void* opaque;
} mixer_t;

mixer_t* mixer_open_device(void);
void mixer_close_device(mixer_t* handle);

#endif
