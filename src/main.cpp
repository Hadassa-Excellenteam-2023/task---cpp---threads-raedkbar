#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <cmath>

std::mutex mutex;  // Mutex to synchronize access to shared data
std::condition_variable cv;  // Condition variable to signal consumer
bool allProducersFinished = false;  // Flag to indicate if all producers finished
int activeProducers = 0;  // Counter for active producers

/**
 * @brief Function executed by each producer thread.
 */
void producerTask()
{
    double value = static_cast<double>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    bool lastMessage = false;

    {
        std::unique_lock<std::mutex> lock(mutex);
        std::cout << "Producer: " << value << " (Thread ID: " << std::this_thread::get_id() << ") sent." << std::endl;
        activeProducers++;

        if (activeProducers == 2)
            lastMessage = true;
    }

    while (!lastMessage && !allProducersFinished)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            value /= 10.0;
            std::cout << "Producer: " << std::fixed << std::setprecision(9) << value << " (Thread ID: " << std::this_thread::get_id() << ") sent." << std::endl;
        }

        if (std::fabs(value) < 0.000000001)
            lastMessage = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    {
        std::unique_lock<std::mutex> lock(mutex);
        std::cout << "Producer (Thread ID: " << std::this_thread::get_id() << ") finished." << std::endl;
        activeProducers--;

        if (activeProducers == 0)
            allProducersFinished = true;
    }

    cv.notify_one();  // Notify consumer thread
}

/**
 * @brief Function executed by the consumer thread.
 */
void consumerTask()
{
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [] { return allProducersFinished; });  // Wait until all producers finish

    std::cout << "Consumer finished." << std::endl;
}

int main()
{
    std::thread consumerThread(consumerTask);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Sleep to allow consumer thread to wait

    std::vector<std::thread> producerThreads;
    producerThreads.emplace_back(producerTask);  // Create first producer thread
    producerThreads.emplace_back(producerTask);  // Create second producer thread

    for (auto& thread : producerThreads) {
        try {
            thread.join();  // Wait for each producer thread to finish
        } catch (const std::exception& e) {
            std::cout << "Exception caught in producer thread: " << e.what() << std::endl;
        }
    }

    consumerThread.join();  // Wait for consumer thread to finish

    return 0;
}
