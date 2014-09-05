/*
 * ps_controller.c
 *
 *  Created on: 2013-01-24
 *      Author: Gary
 */

#include "ps_controller.h"


void update_Controller_Keys(psController *ps)
{
	ps->status = *psControllerValue;
}
