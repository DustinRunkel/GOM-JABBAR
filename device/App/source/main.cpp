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
//Flag that tells us the end of the JSON string
static bool TEMP_FLAG_JSON_END = 0;

void ledTask( void * parameters ) __attribute__( ( noreturn ) );

void listenerCallback( void * str );

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

void listenerCallback( void * str )
{
    /*
    * Params must come in as void pointer, therefore I must cast this into the correct type
    * before use in the source program. 
    */
    auto str_ = static_cast<std::string*>(str);
    /*
    * RPI SDK wires stdin through UART or USB. We must collect each char 
    * into our buffer
    * 
    * One thing to note: this callback is called every time a char is in the buffer,
    * meaning we must read json objects one byte at a time, and assemble them in memory
    * over here.
    */
    char in_char = getchar();

    if(in_char == '\0')
    {
        TEMP_FLAG_JSON_END = 1;

        //push back the null byte
        str_->push_back(in_char);

        //TODO: make a manager that stores JSON objects here I just print them back out
        yyjson_doc *doc = yyjson_read(str_->c_str(), str_->length() + 1, 0);
        yyjson_val *root = yyjson_doc_get_root(doc);

        size_t idx, max;
        yyjson_val *val;
        yyjson_arr_foreach(root, idx, max, val ) 
        {
            printf("Item%d: %s\n", idx, (char*)yyjson_get_str(val));
        }

        str_->clear();
        TEMP_FLAG_JSON_END = 0;
    }
    else
    {
        str_->push_back(in_char);
    }

}
/*-----------------------------------------------------------*/

int main( void )
{
    TaskHandle_t ledTaskTCB;

    std::string * incoming_json = new std::string();

    ( void ) printf( "GOM-JABBAR demo application pre-alpha build: use at own risk\n" );

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    stdio_usb_init();

    stdio_set_chars_available_callback(listenerCallback, incoming_json);

    ( void ) xTaskCreate( ledTask,
                            "ledblink",
                            128,
                            NULL,
                            1,
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