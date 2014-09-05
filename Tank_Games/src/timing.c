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
#include "timing.h"


/*
int main(void) {
	printf("test\n");

	 The alt_alarm must persist for the duration of the alarm.

	char * filenames[MAXFILES];
	av_config_setup();
	int numfiles = init_sd(filenames);
	init_wav(arr);
	time_of_sound = time_audio(arr);


	audio_isr_k = 0;
	alt_irq_register(AUDIO_IRQ, NULL, &audio_isr);
	alt_up_audio_enable_write_interrupt(audio);

	while (1) {
	}
	return 0;
}
*/


void audio_isr(void * context, alt_u32 irq_id) {
	unsigned int * input = arr[audio_isr_k];

	if (alt_up_audio_write_interrupt_pending(audio) == 1) {
		alt_up_audio_write_fifo(audio, input, 100, ALT_UP_AUDIO_LEFT);
		alt_up_audio_write_fifo(audio, input, 100, ALT_UP_AUDIO_RIGHT);
		audio_isr_k = (audio_isr_k + 1) % 4377;
	}

}

void start_clock(int time) {
	if (alt_alarm_start(&alarm, time, my_alarm_callback, NULL) < 0) {
		printf("No system clock available\n");
	}
}

alt_u32 my_alarm_callback(void* context) {
	printf("Loop\n");
	play_wav(arr);
	return time_of_sound;
}



