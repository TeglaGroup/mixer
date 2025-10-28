#include <mixer.h>

#include <unistd.h>

int main(){
	mixer_t* mixer = mixer_open_device();
	mixer_audio_t* audio = mixer_open_file(mixer, "aryx.mp3", 0);

	while(1) usleep(10);

	mixer_close_device(mixer);
}
