#ifndef FIFO_BUFFER
#define FIFO_BUFFER

typedef struct {
	int size;
	int index;
	char *data;
	int count;
} fifoBuffer;

int init_fifo_buffer(int size, fifoBuffer *buffer);
int write_fifo_buffer(fifoBuffer *buffer, char *fromWrite, int byte);
int read_fifo_buffer(fifoBuffer *buffer, char *toWrite, int byte);
int destroy_fifo_buffer(fifoBuffer *buffer);
int is_fifo_buffer_empty(fifoBuffer *buffer);
int is_fifo_buffer_full(fifoBuffer *buffer);


#endif
