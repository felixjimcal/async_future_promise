#include <iostream>
#include <future>
#include <thread>
int square(unsigned int i) {
    return i * i;
}

int main() {
    // https://stackoverflow.com/questions/11004273/what-is-stdpromise/12335206#12335206
    // https://en.cppreference.com/w/cpp/thread/future <-- Include exception handling
    // https://cplusplus.com/reference/future/promise

    // * ASYNC **************************************************
    // The most convenient and straight-forward way to perform an asynchronous computation is 
    // via the async function template, which returns the matching future immediately:
    int result = std::async(foo, 1.5, 'x', false); // you can use auto instead of int
    
    // * FUTURE **************************************************
    // provides a mechanism to access the result of asynchronous operations.
    // An asynchronous operation (created via std::async, std::packaged_task, or std::promise) 
    // can provide a std::future object to the creator of that asynchronous operation
    // future from a packaged_task
    std::packaged_task<int()> task([]{ return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread t(std::move(task)); // launch on a thread
 
    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, []{ return 8; });
 
    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{ p.set_value_at_thread_exit(9); }).detach();
 
    std::cout << "Waiting..." << std::flush;
    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: " << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
    t.join();
    
    
    // * PROMISE **************************************************
    // A promise is an object that can store a value of type T to be retrieved by a future object 
    // (possibly in another thread), offering a synchronization point.
    void print_int (std::future<int>& fut) {
        int x = fut.get();
        std::cout << "value: " << x << '\n';
    }
    
    std::promise<int> prom; 
    std::future<int> fut = prom.get_future();    // engagement with future
    std::thread th1 (print_int, std::ref(fut));  // send future to new thread
    prom.set_value (10);                         // fulfill promise
    th1.join();                                  // (synchronizes with getting the future)

    
    return EXIT_SUCCESS;
}
