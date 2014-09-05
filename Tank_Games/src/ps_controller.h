/*
 * ps_controller.h
 *
 *  Created on: 2013-01-24
 *      Author: Gary
 */

//Overview: Reads the button pressed in the controller. Button information is stored in the struct
#ifndef PS_CONTROLLER_H_
#define PS_CONTROLLER_H_

#define psControllerValue (int*)0x0002500

typedef union u_controller psController;

union u_controller
{
  struct
  {
	unsigned char sel : 1;
	unsigned char start : 1;
	unsigned char up : 1;
	unsigned char down : 1;
	unsigned char left : 1;
	unsigned char l2 : 1;
	unsigned char r2:  1;
	unsigned char l1 : 1;
	unsigned char r1 : 1;
	unsigned char triangle : 1;
	unsigned char right : 1;
	unsigned char x : 1;
	unsigned char square : 1;
	unsigned char circle : 1;
	unsigned char blank : 2;
  }b;
  unsigned short status;
};

//Ensure: sets ps structure to contain the current value of the controller keys
void update_Controller_Keys(psController* ps);


#endif /* PS_CONTROLLER_H_ */
