#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <freertos/ringbuf.h>
#include <string>

// namespace Kernel
// {
/**
 * @brief Ring buffer callback
 *
 */
class RingBuffer
{
private:
    RingbufHandle_t mRingBufHandle; /* ring buffer handle */

public:
    /**
     * @brief Construct a new Ring Buffer object
     *
     * @param length length of the ring buffer
     * @param type Ring buffer type
     */
    RingBuffer(size_t length, RingbufferType_t type = RINGBUF_TYPE_NOSPLIT);
    /**
     * @brief Destroy the Ring Buffer object
     *
     */
    ~RingBuffer();
    /**
     * @brief Receive data fromthe ring buffer
     *
     * @param size size of the data
     * @param wait wait duration
     * @return void* return the data
     */
    void * receive(size_t * size, TickType_t wait = portMAX_DELAY);
    /**
     * @brief Return item
     *
     * @param item item to return
     * @return void*
     */
    void * returnItem(void * item);
    /**
     * @brief Send data to ring buffer
     *
     * @param data data to send
     * @param length length of data
     * @param wait wait duration
     * @return true if sent
     * @return false if not sent
     */
    bool send(void * data, size_t length, TickType_t wait = portMAX_DELAY);
};
// }

#endif
