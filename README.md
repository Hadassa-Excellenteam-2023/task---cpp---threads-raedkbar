# Threads

This is a C++ code, demonstrating a multi-threaded producer-consumer pattern using threads and synchronization mechanisms. The program creates multiple producer threads that generate and send messages, while a consumer thread waits for all producers to finish before completing its task.


## Code Overview

The code consists of the following components:

- `main.cpp`: The main source code file containing the implementation of producer and consumer tasks.

- `std::mutex`: A mutex used to synchronize access to shared data.

- `std::condition_variable`: A condition variable used to signal the consumer thread when all producers have finished.

- `producerTask()`: The function executed by each producer thread. It generates unique values and sends them to the console.

- `consumerTask()`: The function executed by the consumer thread. It waits until all producers have finished before completing its task.

- `main()`: The entry point of the program. It creates threads for producers and the consumer, and waits for them to finish.

## How It Works

1. Multiple producer threads are created, and each thread generates unique values and sends them to the console.

2. The consumer thread waits until all producer threads have finished using a condition variable.

3. Once all producers have finished, the consumer thread completes its task and prints a message.

4. The main function creates the necessary threads, waits for them to finish, and then exits.

