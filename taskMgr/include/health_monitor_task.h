
#ifndef HEALTH_MONITOR_TASK_H
#define HEALTH_MONITOR_TASK_H

// Function to initialize the health monitor task
void health_monitor_init(void);

// Function to start the health monitor task
void health_monitor_main(void* args);

// Function to deinitialize the health monitor task
void health_monitor_deinit(void);


#endif // HEALTH_MONITOR_TASK_H