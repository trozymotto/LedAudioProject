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
char send_buffer[32];
volatile int printAdc = 0;
 
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
    
    //ADSC
    //23.4 Page 237
    //A single conversion is started by writing a logical one to the ADC Start Conversion bit, ADSC. This bit stays high as long as the conversion is in progress and will be cleared by hardware when the conversion is completed.
    //23.9.2 Page 250
    //7 ADEN, 6 ADSC, 5 ADATE, 4 ADIF, 3 ADIE, 2 ADPS2, 1 ADPS1 0 ADPS0
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    DIDR0 = (1 << ADCD01);
    
    
    
    //start_analog_conversion(CHANNEL_A);  // start initial conversion
    while (1)
    {
        if (printAdc)
        {
            printAdc = 0;
            print_adc_vals();
            samples = 0;
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

// INTERRUPT HANDLER for reading the ADC
ISR(TIMER0_COMPA_vect) 
{
    dataArray[samples] = analog_read(0);//analog_conversion_result();  // get result
    if (samples >= ARRAY_LEN)           // if all samples have been taken...
    {
        printAdc = 1;
    }
    else
    {
        samples++;
    }
}
