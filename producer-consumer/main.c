/* Kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Library includes. */
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/multicore.h>

#define QLEN 5

/**
 * The queue used to pass messages between the two tasks
 */
static QueueHandle_t queue = NULL;


/**
 * Configures the hardware
 */
static void setup();

/**
 * Produces random numbers and enqueues them
 * @param  Parameter passed to the task: Not used
 */
static void producerTask( void *param );

/**
 * Consumes random numbers from the queue
 * @param  Parameter passed to the task: Not used
 */
static void consumerTask( void *param );


void main( void ){
	setup();
	printf("Starting SMP producer-consumer...\n");

	queue = xQueueCreate(QLEN, sizeof(uint8_t) );
	if(!queue){
		printf("FreeRTOSConfig does not support queues.");
		for( ;; );
		return; // Can't work without the queue
	}

	/* Task0 running on core 0 only */
	xTaskCreateAffinitySet(
		producerTask,              /* The function that implements the task. */
		NULL,                      /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		configMINIMAL_STACK_SIZE,  /* The size of the stack to allocate to the task. */
		NULL,                      /* The parameter passed to the task. */
		1,                         /* The priority assigned to the task. */
		0x01,                      /* The affinity mask, lockint task to run ONLY on Core 0 */
		NULL );                    /* The task handle is not required, so NULL is passed. */

	/* Task1 running on core 1 only */
	xTaskCreateAffinitySet(
		consumerTask,              /* The function that implements the task. */
		NULL,                      /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		configMINIMAL_STACK_SIZE,  /* The size of the stack to allocate to the task. */
		NULL,                      /* The parameter passed to the task. */
		1,                         /* The priority assigned to the task. */
		0x02,                      /* The affinity mask, lockint task to run ONLY on Core 1 */
		NULL );                    /* The task handle is not required, so NULL is passed. */

	vTaskStartScheduler();
	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the Idle and/or
	timer tasks to be created.  See the memory management section on the
	FreeRTOS web site for more details on the FreeRTOS heap
	http://www.freertos.org/a00111.html. */
	for( ;; );
}


static void setup(){
	stdio_init_all();
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	gpio_put(PICO_DEFAULT_LED_PIN, 0);
}


static void producerTask( void *param ){
	uint8_t num;

	vTaskDelay(pdMS_TO_TICKS(1000));
	printf("Hello from the Producer task running on core %lu\n", get_core_num());
	while(true){
		for ( uint8_t i = 0; i < QLEN; ++i ){
			num = 1 + rand() % 10;
			printf("Core %lu produced random number %d\n", get_core_num(), num);
			if(xQueueSend(queue, (void*)&num, pdMS_TO_TICKS(1000)) != pdPASS ){
				printf("Core %d failed to enqueue %d after 100ms\n", get_core_num(), num);
				break;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


static void consumerTask( void *param ){
	uint8_t num;

	vTaskDelay(pdMS_TO_TICKS(1000));
	printf("Hello from the Consumer task running on core %lu\n", get_core_num());
	while(true){
		xQueueReceive(queue, (void*)&num, portMAX_DELAY);
		printf("Core %lu got a %d from the queue\n", get_core_num(), num);
		while(num-- > 0){
			gpio_put(PICO_DEFAULT_LED_PIN, 1);
			vTaskDelay(pdMS_TO_TICKS(50));
			gpio_put(PICO_DEFAULT_LED_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(50));
		}
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}