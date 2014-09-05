
#include <stdio.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "alt_types.h"
#include <stddef.h>
#include "audio.h"

int sound_file = 0;
alt_up_audio_dev * audio;

int size_of_sound = 0;

/*int main()
{
	unsigned int *arr[4377];

	//unsigned int *arr = (unsigned int*)malloc(sizeof(unsigned int) * 4377);

	int i,ticks;

	av_config_setup();
	numfiles = init_sd(filenames);
	init_wav(arr);

	ticks = time_audio(arr);
	printf("Audio ticks in ms: %d\n", ticks);

	printf("Playing wav.\n");
	for (i = 0; i<3; i++)
		play_wav(arr);

	clean_audio(arr);

	return 0;
}
*/

static unsigned int convert (unsigned int high, unsigned int low) {
	return (high * pow(16,4) + (low * pow(16,2)));
}
void av_config_setup() {
	bool once = true;
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev(AV_CONFIG);
	while(!alt_up_av_config_read_ready(av_config)) {
		if(once) {
			printf("Error, audio not configured.\n");
			once = false;
		}
	}
}

int init_sd(char * filenames[]){
	int i = 0;
	char thepath[] = "/";
	short int check_file_name;

	alt_up_sd_card_dev * sd_card = NULL;
	int connected = 0;
	sd_card = alt_up_sd_card_open_dev("/dev/Interface");

	if (sd_card != NULL) {
		while(1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())) {
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					check_file_name = alt_up_sd_card_find_first(thepath,filenames[i]);
					if (check_file_name == 1)
						printf("Invalid Directory.\n");
					else if (check_file_name == 2)
						printf("Invalid SD Card.\n");
					else if (check_file_name == -1)
						printf("No files found.\n");
					else {
						//	printf("First file name = %s.\n", filenames[i]);
						i++;
					}
					while(alt_up_sd_card_find_next(filenames[i]) != -1){
						//	printf("Next file name = %s.\n", filenames[i]);
						i++;
					}
					return i;
				} else {
					printf("Unknown file system.\n");
				}
				connected = 1;
			} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
				printf("Card disconnected.\n");
				connected = 0;
			}
		}
	}
	else {
		printf("Error.\n");
		return -1;
	}
}

void play_wav (unsigned int **arr)
{
	alt_up_audio_reset_audio_core(audio);
	unsigned int * input;
	int k = 0;
	input = arr[k];
	while (1) {
		if (alt_up_audio_write_interrupt_pending(audio) == 1)
		{
			alt_up_audio_write_fifo(audio, input, 100, ALT_UP_AUDIO_LEFT);
			alt_up_audio_write_fifo(audio, input, 100, ALT_UP_AUDIO_RIGHT);
			k++;
			input = arr[k];
		}
		if (k >= 4377)
			break;
	}

}



void init_wav(unsigned int *arr[])
{
	printf("Initializing wav file\n");
	int i,j,k;
	int size_of_wav = 0;
	int location = 0;
	unsigned int buffer[100] = {0};
	unsigned int * soundbuf;
	soundbuf = (unsigned int*) malloc(sizeof(char) * size_of_wav);

	audio = alt_up_audio_open_dev(AUDIO_DEV);
	alt_up_audio_reset_audio_core(audio);
	//alt_up_audio_enable_write_interrupt(audio);
	/*
		sound_file = alt_up_sd_card_fopen("WAV2.WAV", false);
		//printf("Soundfile: %d\n", sound_file);
		if (sound_file != -1) {
			printf("File opened.\n");
			while(alt_up_sd_card_read(sound_file) != -1) {
				size_of_wav++;
			}
			printf("Size of wav: %d\n", size_of_wav);

			if(alt_up_sd_card_fclose(sound_file))
				printf("File closed.\n");
			else
				printf("Error closing file.\n");
		}
	 */
	size_of_wav = 875484;
	size_of_sound = (int) size_of_wav/200;
	//printf("Size of sound: %d\n", size_of_sound);
	//printf("Size of wav: %d\n", size_of_wav);
	sound_file = alt_up_sd_card_fopen("wav2.wav", false);

	if (sound_file != -1) {
		//printf("File opened.\n");
		for(i = 0; i<size_of_wav; i++){
			soundbuf[i] = alt_up_sd_card_read(sound_file);
		}
		/*
		 * debugging WAV header
		 *
			printf("ChunkSize: 0x%02x%02x%02x%02x\n", soundbuf[7], soundbuf[6], soundbuf[5], soundbuf[4]);
			printf("SubChunkSize: 0x%02x%02x%02x%02x\n", soundbuf[19], soundbuf[18], soundbuf[17], soundbuf[16]);
			printf("Audio Format: 0x%02x%02x\n", soundbuf[21], soundbuf[20]);
			printf("Channels : 0x%02x%02x\n", soundbuf[23], soundbuf[22]);
			printf("Samplerate: 0x%02x%02x%02x%02x\n", soundbuf[27], soundbuf[26], soundbuf[25], soundbuf[24]);
			printf("Bitrate: 0x%02x%02x%02x%02x\n", soundbuf[31], soundbuf[30], soundbuf[29], soundbuf[28]);
			printf("Blockalign: 0x%02x%02x\n", soundbuf[33], soundbuf[32]);
			printf("BitsPerSample: 0x%02x%02x\n", soundbuf[35], soundbuf[34]);
			printf("Data mark: %c%c%c%c\n", soundbuf[210], soundbuf[211], soundbuf[212], soundbuf[213]);
			printf("Subchunksize2: 0x%02x%02x%02x%02x\n", soundbuf[217], soundbuf[216], soundbuf[215], soundbuf[214]);
			datasize = soundbuf[217] * pow(16,6) + soundbuf[216] * pow(16,4) + soundbuf[215] * pow(16,2) + soundbuf[214];
			printf("Datasize: %d\n", datasize);
			printf("Chunks:");
			for (i = 218; i < 318; i++) {
				printf(" %02x", soundbuf[i]);
			}
			printf("\n");
		 */
		for(i = 0; i<size_of_wav; i++){
			if (soundbuf[i] == 'd') {
				if (soundbuf[i+1] == 'a'){
					if (soundbuf[i+2] == 't'){
						if (soundbuf[i+3] == 'a') {
							location = i;
							break;
						}
					}

				}
			}
		}
		i = location;
		for (k = 0; k<size_of_sound; k++) {
			for (j = 0; j<100; j++){
				buffer[j] = convert(soundbuf[i+1], soundbuf[i]);
				i = i + 2;
			}
			arr[k] = (unsigned int*)malloc(sizeof(buffer));
			memcpy((void*)arr[k], (void*)buffer, sizeof(buffer));
		}

	}
	free(soundbuf);
}

void clean_audio(unsigned int *arr[]) {
	int k;
	for (k = 0; k < size_of_sound; k++) {
		free(arr[k]);
	}
	alt_up_sd_card_fclose(sound_file);
}

int time_audio(unsigned int **arr)
{
	double timer,freq,total;
	int f = alt_timestamp_start();
	play_wav(arr);
	timer = alt_timestamp();
	freq = alt_timestamp_freq();
	total = (int)((timer  * 1000)/freq);
	return total;
}
