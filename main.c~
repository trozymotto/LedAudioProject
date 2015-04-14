//#include <pololu/orangutan.h>

#include "led_controller.h"
#include "timer.h"
#include "menu.h"

//Gives us uintX_t (e.g. uint32_t - unsigned 32 bit int)
//On the ATMega128 int is actually 16 bits, so it is better to use
//  the int32_t or int16_t so you know exactly what is going on
#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void init_adc(void);

// GLOBALS
volatile uint32_t G_yellow_ticks = 0;
volatile uint32_t G_ms_ticks = 0;

volatile uint16_t G_red_period = 500;
volatile uint16_t G_green_period = 500;
volatile uint16_t G_yellow_period = 500;

volatile uint16_t G_release_red = 0;

volatile uint32_t G_red_toggles = 0;
volatile uint32_t G_green_toggles = 0;
volatile uint32_t G_yellow_toggles = 0;

volatile uint8_t G_flag = 0; // generic flag for debugging
char send_buffer[32];

// ADC stuff
#define ARRAY_LEN 100
unsigned int dataArray[ARRAY_LEN];
unsigned char samples = 0;

int main(void) 
{
	// Initialization here.
	lcd_init_printf();	// required if we want to use printf() for LCD printing
	init_timers();
	init_menu();	// this is initialization of serial comm through USB
	init_adc();
	clear();	// clear the LCD

	//enable interrupts
	sei();
	
	start_analog_conversion(CHANNEL_A);  // start initial conversion
	while (1) 
	{
	    if (!analog_is_converting())     // if conversion is done...
        {
          dataArray[samples] = analog_conversion_result();  // get result
          start_analog_conversion(CHANNEL_A);   // start next conversion
          if (++samples >= ARRAY_LEN)           // if 20 samples have been taken...
          {
            print_adc_vals();
            samples = 0;
          }
        }
        serial_check();
		check_for_new_bytes_received();
	} //end while loop
} //end main

void print_adc_vals(void)
{

    // Used to pass to USB_COMM for serial communication
    volatile int i = 0;
	int length;
	char tempBuffer[64];
    wait_for_sending_to_finish();

    // Print the header
    //length = sprintf( tempBuffer, "Adc vals\r\n");
    //print_usb(tempBuffer, length);
    //wait_for_sending_to_finish();
    
    // View the current values in the array
    for(i = 0; i < ARRAY_LEN; i+=5)
    { 
        length = snprintf( tempBuffer, 64, "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n", 
                      dataArray[i], dataArray[i+1], dataArray[i+2], 
                      dataArray[i+3], dataArray[i+4]);
        print_usb(tempBuffer, length);
        wait_for_sending_to_finish();
    }

}


void init_adc(void)
{
    set_analog_mode(MODE_10_BIT);    // 8-bit analog-to-digital conversions
}
