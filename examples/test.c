#include <mixer.h>

#include <unistd.h>

int main(){
	mixer_t* mixer = mixer_open_device();
	mixer_audio_t* audio = mixer_open_file(mixer, "./aryx.s3m");

	mixer_audio_set_loop(mixer, audio, 1);
	mixer_audio_start(mixer, audio);

	while(1) usleep(10);

	mixer_close_device(mixer);
}
