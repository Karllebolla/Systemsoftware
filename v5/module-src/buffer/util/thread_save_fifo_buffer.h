#ifndef THREAD_SAVE_FIFO_BUFFER
#define THREAD_SAVE_FIFO_BUFFER

#include <linux/mutex.h>

typedef struct {
	int size;
	int index;
	char *data;
	int count;
	struct mutex buffer_mutex;
} fifoBuffer;

int init_fifo_buffer(int size, fifoBuffer *buffer);
int write_fifo_buffer(fifoBuffer *buffer, char *fromWrite, int byte);
int read_fifo_buffer(fifoBuffer *buffer, char *toWrite, int byte);
int destroy_fifo_buffer(fifoBuffer *buffer);
int is_fifo_buffer_empty(fifoBuffer *buffer);
int is_fifo_buffer_full(fifoBuffer *buffer);


#endif
