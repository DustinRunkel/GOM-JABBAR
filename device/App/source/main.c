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

/* Standard includes. */
#include <stdio.h>

/*pico sdk includes*/
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

/*-----------------------------------------------------------*/

void ledTask( void * parameters ) __attribute__( ( noreturn ) );

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
/*-----------------------------------------------------------*/

int main( void )
{
    TaskHandle_t ledTaskTCB;

    ( void ) printf( "GOM-JABBAR demo application pre-alpha build: use at own risk\n" );

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

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