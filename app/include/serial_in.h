#ifndef SERIAL_IN_H
#define SERIAL_IN_H

#define INVALID_INPUT -1
// Function to initialize the serial input
extern void serial_init(void);

// Function to deinitialize the serial input
extern void serial_deinit(void);

// Function to read a byte from the serial input
extern void input_serial_main(void* arg); 

// extern functions
extern void frequency(int* const freq_out);

#endif // SERIAL_IN_H
