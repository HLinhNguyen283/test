#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <random>
#include <chrono>
#include <semaphore.h>

#define MAX_THREADS 3
#define SIMULATION_COUNT 100

struct SensorData {
    std::string sensorName;
    int sensorValue;
};

std::mutex mutex;
std::queue<SensorData> messageQueue;
std::condition_variable queueCondition;
sem_t threadLimitSemaphore;
int simulatedCount = 0; // Keep track of the number of simulated sensor values

// Function to log sensor data to the appropriate log file
void logSensorData(const SensorData& data) {
    std::string fileName = data.sensorName.substr(0, data.sensorName.find_last_of('.')) + ".log";
    std::ofstream file(fileName, std::ios::app);
    if (file.is_open()) {
        file << data.sensorValue << ", ";
        file.close();
    }
}

// Function to handle sensor data in separate threads
void handleSensorData() {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        // Wait until the message queue is not empty or the simulation is completed
        queueCondition.wait(lock, [] { return !messageQueue.empty() || simulatedCount == SIMULATION_COUNT; });

        // Check if the simulation is completed
        if (simulatedCount == SIMULATION_COUNT && messageQueue.empty()) {
            // Stop the thread as the simulation is completed
            return;
        }

        // Get the front element of the message queue
        SensorData data = messageQueue.front();
        messageQueue.pop();

        lock.unlock();
        queueCondition.notify_all();

        // Log the sensor data to the appropriate log file
        logSensorData(data);

        // Signal that a thread has finished processing
        sem_post(&threadLimitSemaphore);
    }
}

// Function to simulate and send sensor data
void simulateAndSendSensorData() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, 10);

    for (int i = 0; i < SIMULATION_COUNT; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Simulate sensor data with random name and value
        SensorData data;
        data.sensorName = "sensor_monitor" + std::to_string(distribution(generator)) + ".log";
        data.sensorValue = distribution(generator);

        // Wait until a thread is available to process the data
        sem_wait(&threadLimitSemaphore);

        {
            std::unique_lock<std::mutex> lock(mutex);
            // Add the sensor data to the message queue
            messageQueue.push(data);
            lock.unlock();
            queueCondition.notify_all();
        }

        // Increase the simulated count
        ++simulatedCount;
    }
}

int main() {
    // Initialize the semaphore to limit the number of threads
    sem_init(&threadLimitSemaphore, 0, MAX_THREADS);

    std::vector<std::thread> threads;

    // Start the threads to handle sensor data
    for (int i = 0; i < MAX_THREADS; ++i) {
        threads.emplace_back(handleSensorData);
    }

    // Start the thread to simulate and send sensor data
    std::thread simulationThread(simulateAndSendSensorData);

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Wait for the simulation thread to finish
    simulationThread.join();

    // Destroy the semaphore
    sem_destroy(&threadLimitSemaphore);

    return 0;
}
