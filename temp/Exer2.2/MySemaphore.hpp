#ifndef MY_SEMAPHORE_HPP
#define MY_SEMAPHORE_HPP

// ==========================================================================
// Include files

#include <mutex>
#include <condition_variable>

// ==========================================================================

/**
 * Semaphore class
**/
class MySemaphore 
{
public:
    //method

    /**
     * @brief constructor
     * @note General process
     * 
     *		Initialize the member variables
    **/
    explicit MySemaphore(int count = 0) : count(count) {}

    /**
     * @brief destructor
     * @note General process
     * 
     *		Remove objects which created with dynamic.
    **/
    virtual ~MySemaphore() {}

    /**
     * @brief Acquire for the semaphore.
     * @note  General process:
     * 
     *      This method blocks the current thread until the semaphore's count is greater than zero.
     *      When the count is greater than zero, it decrements the count and returns.
    **/
    void acquire() {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock, [this] { return count > 0; });
        --count;
    }

    /**
     * @brief Release the semaphore.
     * @note  General process:
     * 
     *      This method increments the semaphore's count and notifies one waiting thread (if any).
    **/
    void release() {
        std::unique_lock<std::mutex> lock(mutex);
        ++count;
        lock.unlock();
        condition.notify_one();
    }

    /**
     * @brief Try to wait for the semaphore.
     * @note General process:
     * 
     *      This method attempts to decrement the semaphore's count without blocking the current thread.
     *      If the count is greater than zero, it decrements the count and returns true.
     *      If the count is zero, it does not block the thread and returns false immediately.
     * 
     * @retval true if the semaphore was successfully decremented, false otherwise.
    **/
    bool try_wait() {
        std::unique_lock<std::mutex> lock(mutex);
        if (count > 0) {
            --count;
            return true;
        }
        return false;
    }

    //event

    //member

private:
    //method

    //event

    //member

    int count;
    std::mutex mutex;
    std::condition_variable condition;
};

#endif // MY_SEMAPHORE_HPP
