/* Kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Library includes. */
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>

/**
 * Hello world task
 * @param  Parameter passed to the task: task number
 */
static void coreTask( void *param );


void main( void ){
	stdio_init_all();
	printf("Starting hello-world...\n");

	/* Task0 running on core 0 only */
	xTaskCreateAffinitySet(
		coreTask,                  /* The function that implements the task. */
		NULL,                      /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		configMINIMAL_STACK_SIZE,  /* The size of the stack to allocate to the task. */
		(void*)0,                  /* The parameter passed to the task. */
		1,                         /* The priority assigned to the task. */
		0x01,                      /* The affinity mask, lockint task to run ONLY on Core 0 */
		NULL );                    /* The task handle is not required, so NULL is passed. */

	/* Task1 running on core 1 only */
	xTaskCreateAffinitySet(
		coreTask,                  /* The function that implements the task. */
		NULL,                      /* The text name assigned to the task - for debug only as it is not used by the kernel. */
		configMINIMAL_STACK_SIZE,  /* The size of the stack to allocate to the task. */
		(void*)1,                  /* The parameter passed to the task. */
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

static void coreTask( void *param ){
	while(true){
		printf("Hello from task %lu on core %lu\n", (size_t)param, get_core_num());
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
