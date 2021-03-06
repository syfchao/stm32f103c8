#include "ringbuffer.h"
#include <string.h>
#include <stdio.h>


#define Min(x, y) ((x) < (y) ? (x) : (y))

cycle_buffer* buffer;

RingBuffer::RingBuffer()
{
    buffer = (cycle_buffer *)malloc(sizeof(cycle_buffer));
    if (!buffer) return; 
    memset(buffer, 0, sizeof(RingBuffer)); 

    buffer->size = DEFAULT_BUF_SIZE;  
    buffer->in   = 0;
    buffer->out  = 0;  

    buffer->buf = (unsigned char *)malloc(buffer->size);  
    if (!buffer->buf)
    {
        free(buffer);
        return;
    }
    memset(buffer->buf, 0, DEFAULT_BUF_SIZE);
}

RingBuffer::~RingBuffer()
{
    if(buffer) {
        free(buffer->buf);
        buffer->buf = NULL;
        free(buffer);
        buffer = NULL;
    }
}


int RingBuffer::Reset(cycle_buffer *buffer)
{
    if (buffer == NULL)
    {
        return -1;
    }
     
    buffer->in   = 0;
    buffer->out  = 0;
    memset(buffer->buf, 0, buffer->size);

    return 0;
}

int RingBuffer::empty(cycle_buffer *buffer)
{
    return buffer->in == buffer->out;
}

int RingBuffer::write(cycle_buffer *buffer,char *data,unsigned int length)
{
    unsigned int len = 0;

    length = Min(length, buffer->size - buffer->in + buffer->out);  
    len    = Min(length, buffer->size - (buffer->in & (buffer->size - 1)));

 
    memcpy(buffer->buf + (buffer->in & (buffer->size - 1)), data, len);
    memcpy(buffer->buf, data + len, length - len);
 
    buffer->in += length;
 
    return length;
}

int RingBuffer::read(cycle_buffer *buffer,char *target,unsigned int amount)
{
    unsigned int len = 0;  

    amount = Min(amount, buffer->in - buffer->out);
    len    = Min(amount, buffer->size - (buffer->out & (buffer->size - 1)));
 
    memcpy(target, buffer->buf + (buffer->out & (buffer->size - 1)), len);
    memcpy(target + len, buffer->buf, amount - len);
 
    buffer->out += amount;
 
    return amount;
}

