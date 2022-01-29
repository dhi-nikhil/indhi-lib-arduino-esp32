#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <freertos/ringbuf.h>
#include <string>

// namespace Kernel
// {
    class RingBuffer
    {
    private:
        RingbufHandle_t mRingBufHandle;

    public:
        RingBuffer(size_t length, RingbufferType_t type = RINGBUF_TYPE_NOSPLIT);
        ~RingBuffer();
        void *receive(size_t *size, TickType_t wait = portMAX_DELAY);
        void *returnItem(void *item);
        bool send(void *data, size_t length, TickType_t wait = portMAX_DELAY);
    };
// }

#endif
