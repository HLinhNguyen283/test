// ==========================================================================
// Include files

#include <iostream>
#include <array>
#include <thread>
#include <queue>
#include <fstream>
#include <random>
#include <chrono>
#include <atomic>
#include "MySemaphore.hpp" 

// ==========================================================================
// Local definitions

#define DEF_NUM_SENSORS             10
#define DEF_NUM_VALUE               10
#define DEF_SIMULATION_MAX_COUNT    100
#define DEF_MAX_THREAD              3
#define DEF_EXTENSION_FILE_LOG      ".log"
#define DEF_SENSOR_NAME             "sensor_monitor"

// ==========================================================================
// Private variables 
struct SensorMessage {
    std::string sensorName;
    int sensorValue;
};

std::mutex mutex;
std::queue<SensorMessage> messageQueue;
std::condition_variable queueCondition;
std::atomic<bool> shouldTerminate(false); // Use std::atomic for thread safety

// Create an instance of the Semaphore class
MySemaphore threadLimitSemaphore(DEF_MAX_THREAD);

// ==========================================================================
// Private function prototypes
void logSensorMessageToFile(const SensorMessage& data);

void handleSensorMessage();

void simulateAndSendSensorMessage();

// ==========================================================================
// Private user code 

/**
 * @brief Main function.
 * @retval 0 The program completed successfully.
**/
int main() 
{
    std::vector<std::thread> threads;

    // Start the threads to handle sensor data
    for (int i = 0; i < DEF_NUM_SENSORS; ++i) {
        threads.emplace_back(handleSensorMessage);
    }

    // Start the thread to simulate and send sensor data
    std::thread simulationThread(simulateAndSendSensorMessage);

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Wait for the simulation thread to finish
    simulationThread.join();

    return 0;
}

/**
 * @brief Log sensor data to the appropriate log file.
 * @param[in] data The sensor data to be logged.
 * @retval None
**/
void logSensorMessageToFile(const SensorMessage& data)
{
    std::string fileName = data.sensorName + DEF_EXTENSION_FILE_LOG; 
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file to log sensor data: " << fileName << std::endl;
        return;
    }

    bool isFileEmpty = true;
    {
        std::ifstream file(fileName);
        isFileEmpty = file.peek() == std::ifstream::traits_type::eof();
    }

    if(!isFileEmpty) {
        file << ", " << data.sensorValue;
    } else {
        file << data.sensorValue;
    }
}

/**
 * @brief Handle sensor data in separate threads.
 * @retval None
**/
void handleSensorMessage()
{
    while (!(shouldTerminate && messageQueue.empty())) 
    {
        threadLimitSemaphore.acquire();

        std::unique_lock<std::mutex> lock(mutex);
        // Wait until the message queue is not empty or the simulation is completed
        queueCondition.wait(lock, [] { return !messageQueue.empty() || shouldTerminate.load(); });

        // Get the front element of the message queue
        SensorMessage data = messageQueue.front();
        messageQueue.pop();

        lock.unlock();
        queueCondition.notify_all();

        // Log the sensor data to the appropriate log file
        logSensorMessageToFile(data);

        // Signal that a thread has finished processing
        threadLimitSemaphore.release();
    }
}

/**
 * @brief  Simulate and send sensor data.
 * @retval None
**/
void simulateAndSendSensorMessage()
{
    std::random_device randomValue;
    std::mt19937 generator(randomValue());
    std::uniform_int_distribution<int> sensorDist(1, DEF_NUM_SENSORS);
    std::uniform_int_distribution<int> valueDist(1, DEF_NUM_VALUE);

    for (int count = 0; count < DEF_SIMULATION_MAX_COUNT; ++count)
    {
        // For each simulation of 10 values, sleep 1s.
        if((count+1)%10 ==  0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Simulate sensor data with random name and value
        SensorMessage data;
        data.sensorName = DEF_SENSOR_NAME + std::to_string(sensorDist(generator));
        data.sensorValue = valueDist(generator);

        {
            std::unique_lock<std::mutex> lock(mutex);
            // Check if the simulation should terminate
            if (shouldTerminate) {
                break;
            }
            // Add the sensor data to the message queue
            messageQueue.push(data);
            lock.unlock();
            queueCondition.notify_all();
        }
    }

    // Set the flag to terminate the handling threads
    shouldTerminate.store(true);
    queueCondition.notify_all();
}