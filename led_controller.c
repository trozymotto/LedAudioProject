#include "led_controller.h"
#include "timer.h"
#include "menu.h"
#include <avr/io.h>
#include <stdio.h>

// GLOBALS

void init_LEDs() 
{


	
}

 
// INTERRUPT Names are defined in iom1284p.h

// INTERRUPT HANDLER for green LED
ISR(TIMER1_COMPA_vect) 
{

}

