#include "fifo_buffer.h"
#include <linux/slab.h>


/*
 * return 0 if success otherwise -1
 */
int init_fifo_buffer(int size, fifoBuffer *buffer) {

	buffer->data = kmalloc(sizeof(char) *size, GFP_KERNEL);
	if(buffer->data == NULL) {
		return -1;
	}
	buffer->size=size;
	buffer->index = 0;
	buffer->count = 0;
	return 0;	
}


int write_fifo_buffer(fifoBuffer *buffer, char *fromWrite, int byte) {

	int freeSpace,bytesToWrite,x,indexToWrite;
	freeSpace = buffer->size - buffer->count;
	bytesToWrite = min(freeSpace,byte);
	
	for( x = 0; x <= bytesToWrite; x++) {
		indexToWrite = (buffer->index + buffer->count) % buffer->size;
		buffer->data[indexToWrite] = fromWrite[x];
		buffer->count++;
	}
	return bytesToWrite;
}

int read_fifo_buffer(fifoBuffer *buffer, char *toWrite, int byte) {

	int bytesToRead,x;
	bytesToRead = min(byte,buffer->count);
	
	for( x = 0; x <= bytesToRead; x++) {
		toWrite[x] = buffer->data[buffer->index];
		buffer->index = buffer->index % buffer->size;
	}
	return bytesToRead;
}

/*
 * return 0 if successfully destory -1 if buffer don't exists
 */
int destroy_fifo_buffer(fifoBuffer *buffer){
	
	if(buffer->data == NULL) {
		return -1;
	}
	kfree(buffer->data);
	buffer->count = 0;
	buffer->size = 0;
	buffer->index = 0;
	return 0;
}

/*
 * return 1 if FifoBuffer is empty otherwise 0
 */
int is_fifo_buffer_empty(fifoBuffer *buffer){
	
	if(buffer->count == 0){
		return 1;
	}
	return 0;
}

/*
 *	retrun 1 if Fifo Buffer is full otherwise 0
 */
int is_fifo_buffer_full(fifoBuffer *buffer) {
	
	if(buffer->size == buffer->count) {
		return 1;
	}
	return 0;
}





