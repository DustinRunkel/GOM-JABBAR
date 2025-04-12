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
#include "GJ_UserConfig.hpp"

/* Standard includes. */
#include <stdio.h>
#include <string>

/*pico sdk includes*/
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

/*-----------------------------------------------------------*/

void ledTask( void * parameters ) __attribute__( ( noreturn ) );

/*-----------------------------------------------------------*/

void ledTask( void * )
{
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
/*-----------------------------------------------------------*/

int main( void )
{
    TaskHandle_t ledTaskTCB;
    TaskHandle_t messengerTCB;

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
    * Instantiate your GOM-JABBAR params struct
    * a helper struct to make it easy to pass objects
    * to tasks
    * We have tasks layed out in our library, but you can just use our components 
    * And make your own if you want.
    */
    GJ::TaskParams * params;
    params->config_ = new Config();
    params->msgHandler = new GJ::MessageHandler(params->config_);

    ( void ) xTaskCreate( ledTask,
                            "ledblink",
                            128,
                            NULL,
                            5,
                            &ledTaskTCB);

    /*
    * Using the tasks defined in TaskSupport, you can assign GOM-JABBAR components to threads
    * or CPU affinity as you desire.
    */
    ( void ) xTaskCreate( GJ::MessageHandlerTask, "messenger", 256, params, 5, &messengerTCB);
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
        printf("Task: %s has overflowed!\n", pcTaskName);
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */