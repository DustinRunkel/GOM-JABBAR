// ________  ________  _____ ______                             
// |\   ____\|\   __  \|\   _ \  _   \                           
// \ \  \___|\ \  \|\  \ \  \\\__\ \  \                          
//  \ \  \  __\ \  \\\  \ \  \\|__| \  \                         
//   \ \  \|\  \ \  \\\  \ \  \    \ \  \                        
//    \ \_______\ \_______\ \__\    \ \__\                       
//     \|_______|\|_______|\|__|_____\|__|________  ________     
//    |\  \|\   __  \|\   __  \|\   __  \|\   __  \|\   __  \    
//    \ \  \ \  \|\  \ \  \|\ /\ \  \|\ /\ \  \|\  \ \  \|\  \   
//  __ \ \  \ \   __  \ \   __  \ \   __  \ \   __  \ \   _  _\  
// |\  \\_\  \ \  \ \  \ \  \|\  \ \  \|\  \ \  \ \  \ \  \\  \| 
// \ \________\ \__\ \__\ \_______\ \_______\ \__\ \__\ \__\\ _\ 
//  \|________|\|__|\|__|\|_______|\|_______|\|__|\|__|\|__|\|__|
// Demonstration application entrypoint

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/*Gom-Jabbar library*/
#include "Gom-Jabbar.hpp"

/* Standard includes. */
#include <stdio.h>
#include <string>

/*pico sdk includes*/
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

/*-----------------------------------------------------------*/

void ledTask( void * parameters ) __attribute__( ( noreturn ) );

void listenerCallback( void * _params );

//void shouterTask( );

/*-----------------------------------------------------------*/

void ledTask( void * parameters )
{
    /* Unused parameters. */
    ( void ) parameters;
    static bool flag = 1;

    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);
    
    for( ; ; )
    {
        vTaskDelay(1000);
        gpio_put(0, flag);
        flag = !flag;
    }
}

void listenerCallback( void * _params )
{
    static GJ::JsonReader reader = GJ::JsonReader();
    /*
    * RPI SDK wires stdin through UART or USB. We must collect each char 
    * into our buffer
    * 
    * One thing to note: this callback is called every time a char is in the buffer,
    * meaning we must read json objects one byte at a time, and assemble them in memory
    * over here.
    * 
    * This is a possible implementation of your application using the GOM-Jabbar json reader
    * The json reader only needs an incoming string from some protocol on your board
    * Eventually, this will be cofigured via an interface
    */
    char in_char = stdio_getchar_timeout_us( 10 );
    /*
    * TODO: I cannot figure out why no input works if echo is disabled
    * We will eventually have to figure this out
    */
    printf("%c", in_char);

    /*
    * If the json object has equal open/closed brackets,
    * the object is fully read-in. Therefore we reset the
    * reader by deserializing the json object into memory
    * and adding it to our backend message queue
    */
    if( in_char == '$') //dollars for now, switch to null-byte later
    {
        if(! reader.deserialize())
        {
            printf("\n%s", reader.errStr_.c_str());
        }
    }
    else
    {
        reader.push(in_char);
    }
    stdio_flush();
#ifdef DEBUG
    printf("\nIF NO ERROR BEFORE HERE: JSON ACCEPTED\n");
#endif

}
/*-----------------------------------------------------------*/

int main( void )
{
    TaskHandle_t ledTaskTCB;

    ( void ) printf( "GOM-JABBAR demo application pre-alpha build: use at own risk\n" );

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    /*
    * initialize our USB driver and hardware items
    */
    stdio_usb_init();

    /*
    * Our SDK allows us to set a callback function when a character is written to the device
    * We can only be sure there is one character available when the callback is called, therefore
    * we have to use this seemingly painful, but reliable method of reading in the characters one-by-one
    * and constructing our json object in memory
    */
    stdio_set_chars_available_callback(listenerCallback, NULL);

    ( void ) xTaskCreate( ledTask,
                            "ledblink",
                            128,
                            NULL,
                            5,
                            &ledTaskTCB);
    /* Start the scheduler. */
    vTaskStartScheduler();
    for( ; ; )
    {
        /* Should not reach here. */
    }

    return 0;
}
/*-----------------------------------------------------------*/

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )

    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */