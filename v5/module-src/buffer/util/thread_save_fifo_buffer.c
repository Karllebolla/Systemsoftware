#include "thread_save_fifo_buffer.h"
#include <linux/slab.h>


/*
 * return 0 if success otherwise -1
 */
int init_fifo_buffer(int size, fifoBuffer *buffer) {

	printk("init fifoBuffer\n");
	buffer->data = kmalloc(sizeof(char) *size, GFP_KERNEL);
	if(buffer->data == NULL) {
		return -1;
	}
	buffer->size=size;
	buffer->index = 0;
	buffer->count = 0;
	mutex_init(&buffer->buffer_mutex);
	return 0;	
}


int write_fifo_buffer(fifoBuffer *buffer, char *fromWrite, int byte) {

	int freeSpace,bytesToWrite,x,indexToWrite;
	
	mutex_lock(&buffer->buffer_mutex);
	freeSpace = buffer->size - buffer->count;
	bytesToWrite = min(freeSpace,byte);
	
	for( x = 0; x < bytesToWrite; x++) {
		indexToWrite = (buffer->index + buffer->count) % buffer->size;
		buffer->data[indexToWrite] = fromWrite[x];
		buffer->count++;
	}

	mutex_unlock(&buffer->buffer_mutex);
	return bytesToWrite;
}

int read_fifo_buffer(fifoBuffer *buffer, char *toWrite, int byte) {

	int bytesToRead,x;
	
	mutex_lock(&buffer->buffer_mutex);
	bytesToRead = min(byte,buffer->count);
	
	for( x = 0; x < bytesToRead; x++) {
		toWrite[x] = buffer->data[buffer->index];
		buffer->index = (buffer->index + 1) % buffer->size;
	}
	buffer->count -= bytesToRead;

	mutex_unlock(&buffer->buffer_mutex);
	return bytesToRead;
}

/*
 * return 0 if successfully destory -1 if buffer don't exists
 */
int destroy_fifo_buffer(fifoBuffer *buffer){
	
	mutex_lock(&buffer->buffer_mutex);
	if(buffer->data == NULL) {
		return -1;
	}
	kfree(buffer->data);
	buffer->count = 0;
	buffer->size = 0;
	buffer->index = 0;
	mutex_unlock(&buffer->buffer_mutex);
	return 0;
}

/*
 * return 1 if FifoBuffer is empty otherwise 0
 */
int is_fifo_buffer_empty(fifoBuffer *buffer){
		
	int ret = 0;
	mutex_lock(&buffer->buffer_mutex);
	if(buffer->count == 0){
		ret =  1;
	}
	mutex_unlock(&buffer->buffer_mutex);
	return ret;
}

/*
 *	retrun 1 if Fifo Buffer is full otherwise 0
 */
int is_fifo_buffer_full(fifoBuffer *buffer) {
	
	int ret = 0;
	mutex_lock(&buffer->buffer_mutex);
	if(buffer->size == buffer->count) {
		ret = 1;
	}
	mutex_unlock(&buffer->buffer_mutex);
	return ret;
}





