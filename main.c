#include <pololu/orangutan.h>

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
#include <stdbool.h>

#include "Adafruit_GFX.h"
#include "RGBmatrixPanel.h"
#include "test_led.h"

//FFT library
#include "ffft.h"
#include "fix_fft.h"

#define FIX_FFT

typedef enum {
    BANK_NULL,
    BANK1,
    BANK2
}tCaptureBank;

// Function prototypes
void init_adc(void);
void print_adc_vals();
int8_t abs_int8_t(int8_t data);

// GLOBALS
char send_buffer[32];
volatile int printAdc = 0;
bool display_color_wheel = 0;
bool display_shapes = 0;
//Interrupt counters
volatile long global_adc_counter = 0;

// ADC stuff
//#define ARRAY_LEN 100
//unsigned int dataArray1[ARRAY_LEN];
tCaptureBank  mBank = BANK1;
unsigned char samples = 0;
int16_t sample;

// spectrum downsample data
#define FFT_BINS 8
#define PAST_NUM 10
uint16_t nowSpectrum[FFT_BINS];
uint16_t pastSpectrum[FFT_BINS][PAST_NUM];
uint16_t avgSpectrum[FFT_BINS];

#ifndef FIX_FFT
int16_t       capture1[FFT_N];
int16_t       capture2[FFT_N];
complex_t     bfly_buff[FFT_N];  // FFT "butterfly" buffer
uint16_t      spectrum[FFT_N/2]; // Spectrum output buffer

#else
int8_t       capture1[FFT_N];
int8_t       capture2[FFT_N];
int8_t       bfly_buff[FFT_N];  // FFT "butterfly" buffer
int8_t       spectrum[FFT_N/2]; // Spectrum output buffer
#endif

// Pinouts for LED Matrix
#define LED_CLK 5
#define LED_OE  1
#define LED_LAT 0
#define LED_A   1
#define LED_B   0
#define LED_C   2
#define LED_D   3

int main(void)
{
  bool led_matrix_on = 0;
  int sample_sum = 0;
  int sample_avg = 0;

    // Initialization here.
    lcd_init_printf();	// required if we want to use printf() for LCD printing
    init_timers();
    init_menu();	// this is initialization of serial comm through USB
    init_adc();
    clear();	// clear the LCD

    //enable interrupts
    sei();

    //init the LED Matrix
    Adafruit_GFX(32, 32);
  	init(16,LED_A, LED_B, LED_C, LED_D, LED_CLK, LED_LAT, LED_OE, false,32);
  	begin();

    //start_analog_conversion(CHANNEL_A);  // start initial conversion
    while (1)
    {


        if (printAdc)
        {
            printAdc = 0;
            volatile int x = 0;
            uint16_t sum;
            int i;
            int samplesPerBin;
            if(mBank == BANK2){
                fix_fft((char*)bfly_buff, (char*)capture1, 7, 0);
                sample_sum = 0;
                for(i=0;i<128;i++)
                    sample_sum += abs_int8_t(capture1[i]);
                sample_avg = sample_sum/8;
            }
            else{
                fix_fft((char*)bfly_buff, (char*)capture2, 7, 0);
                sample_sum = 0;
                for(i=0;i<128;i++)
                    sample_sum += abs_int8_t(capture2[i]);
                sample_avg = sample_sum/8;
            }
            //print_adc_vals();

            // Downsample spectrum output to 8 columns:

            for(x=0; x<FFT_BINS; x++) {
                sum = 0;
                samplesPerBin = FFT_N/FFT_BINS;
                for(i=0; i<samplesPerBin; i++)
                    sum += abs_int8_t(bfly_buff[(x*samplesPerBin)+i]);
                nowSpectrum[x] = sum;
                // Shift the data in the history buffer
                for (i=0; i<PAST_NUM-1; i++)
                {
                    pastSpectrum[x][i] = pastSpectrum[x][i+1];
                }
                pastSpectrum[x][PAST_NUM-1] = sum;
                // Average the data in the past array
                for(i=0;i<PAST_NUM;i++){
                    avgSpectrum[x] += pastSpectrum[x][i];
                }
                avgSpectrum[x] = avgSpectrum[x]/PAST_NUM;
            }
            //fft_execute(bfly_buff);          // Process complex data
            //fft_output(bfly_buff, spectrum); // Complex -> spectrum

        //print_adc_vals();
        
        fillRect(0, 0, 4, 32, Color333(0, 0, 0));
        fillRect(0, 0, 4, nowSpectrum[0], Color333(7, 0, 0));

        fillRect(4, 0, 4, 32, Color333(0, 0, 0));
        fillRect(4, 0, 4, nowSpectrum[1], Color333(7,4,0));

        fillRect(8, 0, 4, 32, Color333(0, 0, 0));
        fillRect(8, 0, 4, nowSpectrum[2], Color333(0,7,0));

        fillRect(12, 0, 4, 32, Color333(0, 0, 0));
        fillRect(12, 0, 4, nowSpectrum[3], Color333(0,7,7));

        fillRect(16, 0, 4, 32, Color333(0, 0, 0));
        fillRect(16, 0, 4, nowSpectrum[4], Color333(0,4,7));

        fillRect(20, 0, 4, 32, Color333(0, 0, 0));
        fillRect(20, 0, 4, nowSpectrum[5], Color333(0,0,7));

        fillRect(24, 0, 4, 32, Color333(0, 0, 0));
        fillRect(24, 0, 4, nowSpectrum[6], Color333(4,0,7));

        fillRect(28, 0, 4, 32, Color333(0, 0, 0));
        fillRect(28, 0, 4, nowSpectrum[7], Color333(7,0,4));


        //fillRect(0, 0, 32, 32, Color333(0, 0, 0));
        //fillRect(0, 0, 32, sample_avg, Color333(7, 0, 0));

        // Pass avgSpectrum into the LED bar update

}
        //lcd_goto_xy(0,0);
        //printf("ADC:%ld", global_adc_counter);


        if (display_shapes)
          test_shapes();

        if (display_color_wheel && !led_matrix_on)
        {
          test_color_wheel();
          led_matrix_on = 1;
        }
        else if (!display_color_wheel && led_matrix_on)
        {
          fillScreen(Color333(0, 0, 0));
          led_matrix_on = 0;
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
    //if(mBank == BANK2){
    //    length = sprintf( tempBuffer, "Capture1:\r\n");
    //}
    //else{
    //    length = sprintf( tempBuffer, "Capture2:\r\n");
    //}
    //print_usb(tempBuffer, length);
    //wait_for_sending_to_finish();

    // View the spectrum data
    length = snprintf( tempBuffer, 64, //"%d,%d,%d,%d,%d,%d,%d,%d,"
                                       "%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                  nowSpectrum[0], //avgSpectrum[0],
                  nowSpectrum[1], //avgSpectrum[1],
                  nowSpectrum[2], //avgSpectrum[2],
                  nowSpectrum[3], //avgSpectrum[3],
                  nowSpectrum[4], //avgSpectrum[4],
                  nowSpectrum[5], //avgSpectrum[5],
                  nowSpectrum[6], //avgSpectrum[6],
                  nowSpectrum[7],sample);//, //avgSpectrum[7]);
    print_usb(tempBuffer, length);
    wait_for_sending_to_finish();
/*
    // View the current values in the array
    for(i = 0; i < FFT_N; i+=5)
    {
        if(mBank == BANK2){
            length = snprintf( tempBuffer, 64, "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n",
                      capture1[i],    bfly_buff[i],
                      capture1[i+1],  bfly_buff[i+1],
                      capture1[i+2],  bfly_buff[i+2],
                      capture1[i+3],  bfly_buff[i+3],
                      capture1[i+4],  bfly_buff[i+4]);
        }
        else{
            length = snprintf( tempBuffer, 64, "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n"
                                               "%d,%d\r\n",
                      capture2[i],    bfly_buff[i],
                      capture2[i+1],  bfly_buff[i+1],
                      capture2[i+2],  bfly_buff[i+2],
                      capture2[i+3],  bfly_buff[i+3],
                      capture2[i+4],  bfly_buff[i+4]);
        }

        print_usb(tempBuffer, length);
        wait_for_sending_to_finish();
    }
*/
}


void init_adc(void)
{
    set_analog_mode(MODE_10_BIT);    // 8-bit analog-to-digital conversions


/*    ADMUX = (1 << REFS0);
    //ADSC
    //23.4 Page 237
    //A single conversion is started by writing a logical one to the ADC Start Conversion bit, ADSC. This bit stays high as long as the conversion is in progress and will be cleared by hardware when the conversion is completed.
    //23.9.2 Page 250
    //7 ADEN, 6 ADSC, 5 ADATE, 4 ADIF, 3 ADIE, 2 ADPS2, 1 ADPS1 0 ADPS0
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    ADCSRB = 0;                // Free run mode, no high MUX bit
    DIDR0 = (1 << ADC0D);
    TIMSK0 = 0;                // Timer0 off*/
}

// INTERRUPT HANDLER for reading the ADC
ISR(TIMER0_COMPA_vect)
{
    static const int16_t noiseThreshold = 4;
    sample = analog_read(0);

    if((sample > (512-noiseThreshold)) && (sample < (512+noiseThreshold)))
        sample = 0;
    else
        sample -= 512; // Sign-convert for FFT; -512 to +511

#ifdef FIX_FFT
    sample = (int8_t)(sample / 8);
#endif
    // Dump the data into the active bank
    switch(mBank){
    case BANK1:
        capture1[samples] = sample;
        break;
    case BANK2:
        capture2[samples] = sample;
        break;
    default:
        break;
    }

    // if all samples have been taken...
    // Set the flag to run the FFT
    // Flip to the other bank
    if (samples >= FFT_N)
    {
        printAdc = 1;
        samples = 0;
        if(mBank == BANK2)
            mBank = BANK1;
        else
            mBank = BANK2;
    }
    else
    {
        samples++;
    }
}

ISR(ADC_vect) {

    global_adc_counter++;

}

int8_t abs_int8_t(int8_t data)
{
    if(data < 0)
        return 0-data;
    return data;
}
