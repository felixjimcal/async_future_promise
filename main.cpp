#include <iostream>
#include <future>
#include <thread>
int square(unsigned int i) {
    return i * i;
}

int main() {
    // https://stackoverflow.com/questions/11004273/what-is-stdpromise/12335206#12335206

    // * ASYNC
    // Deals with creating a thread and creating a future for us
    // std::async returns a std::future that holds the return value that will be calculated by the function
    // std::async works without a launch policy, so std::async(square, 5); compiles.
    // When you do that the system gets to decide if it wants to create a thread or not.
    // The idea was that the system chooses to make a thread unless it is already running more threads than it can run efficiently.
    // override that behavior with std::launch::async which forces the system to create a thread
    auto f = std::async(std::launch::async, square, 8); // forcing a thread
    std::cout << "square currently running\n"; // do something method is running
    std::cout << "result is " << f.get() << '\n'; // getting the result

    // * FUTURE (yes, they look practically equal)
    // A future is an object that can retrieve a value from some provider object or function,
    // Valid" futures are future objects associated to a shared state, and are constructed by calling one of the following functions:
    // async
    // promise::get_future
    // packaged_task::get_future
    std::future<int> fut = std::async (square, 444444443); // creating our own future
    std::cout << "isPrime currently running\n"; // do something method is running
    std::cout << "result is " << fut.get() << '\n';

    // * FUTURE AND PROMISE
    // A PROMISE allows us to return state from a thread.
    // Whereas a FUTURE is for reading that returned state.
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    promise.set_value(square(5));
    std::cout << "result is " << future.get() << '\n';

    return EXIT_SUCCESS;
}
