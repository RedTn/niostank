/*
 * timing.h
 *
 *  Created on: 2013-01-29
 *      Author: Takdeer Kaur
 */

#ifndef TIMING_H_
#define TIMING_H_

#include <stdio.h>
#include "global.h"
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "alt_types.h"
#include <stddef.h>
#include "audio.h"


alt_u32 my_alarm_callback(void* context);
void audio_isr(void * context, alt_u32 irq_id);
void start_clock(int time);

#endif /* TIMING_H_ */
