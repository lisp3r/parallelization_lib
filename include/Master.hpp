#ifndef CRYPTO_PLAYGROUND_V2_MASTER_HPP
#define CRYPTO_PLAYGROUND_V2_MASTER_HPP

#include <future>
#include <vector>
#include <map>
#include "Key.hpp"
#include "FutureContainer.hpp"
#include "FutureContainerElement.hpp"
#include <queue>
#include <functional>
#include "ExceptionContainer.hpp"

class Master {
private:
    std::atomic_bool stop;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> task_queue;
    std::mutex queue_guard;
    std::mutex print_guard;
    std::condition_variable cv;

    size_t num_threads;
    std::atomic_size_t num_busy_threads;
    int state;

    FutureContainer futures;
    ExceptionContainer exceptions;

private:
    void AddToQueue(std::function<void()> function) {
        std::unique_lock<std::mutex> locker(queue_guard);
        task_queue.push(function);
    }

public:
    Master(size_t threads = 1) : stop(false), num_busy_threads(0), state(0) {
        if (threads == 0)
            threads = 1;
        num_threads = threads;

        for (size_t i = 0; i < num_threads; i++)
            workers.emplace_back([this]() {
                while (1) {
                    std::function<void()> task;
                    // for releasing mutex behind the scope
                    {
                        std::unique_lock<std::mutex> locker(queue_guard);
                        // conditions for stop waiting and get come work: stop == false and task_queue is not empty. Also state must be 1 for some reason.
                        // a thread is waiting for predicate return true.
                        cv.wait(locker, [&]() {
                            return (state == 1) && (!task_queue.empty() || stop);
                        });

                        if (stop && task_queue.empty())
                            return;

                        num_busy_threads++;
                        task = task_queue.front();
                        task_queue.pop();
                    }
                    // {
                    //   std::unique_lock<std::mutex> locker(print_guard);
                    //   std::cout << std::this_thread::get_id() << ": I get it!" << std::endl;
                    // }
                    task();
                    // {
                    //   std::unique_lock<std::mutex> locker(print_guard);
                    //   std::cout << std::this_thread::get_id() << ": I pop it" << std::endl;
                    // }
                    num_busy_threads--;
                    stop = true;
                }
            });
    }

    size_t GetNumThreads() { return num_threads; }

    size_t GetNumTasksInQueue() {
      // std::unique_lock<std::mutex> locker(queue_guard);
      return task_queue.size();
    }

    size_t GetNumBusyThreads() { return num_busy_threads; }

    void Start() {
        state = 1;
        cv.notify_all();
    }

    void Wait() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 0;
    }

    void Stop() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        state = 0;
        std::unique_lock<std::mutex> locker(queue_guard);
        while (!task_queue.empty())
            task_queue.pop();
    }

    ~Master() {
        for (auto &a : workers) {
            a.join();
            stop = true;
        }
    }

    bool IsReady(Key &k) {
        return futures.IsDataReady(k);
    }

    template<class _R>
    _R GetData(Key &key) {
        if (IsReady(key)) {
            auto res = futures.GetData<_R>(key);
            return res;
        }
        std::cerr << "Data is not ready yet!" << std::endl;
        _R sorry = 0;
        return sorry;
    }

    std::exception_ptr GetException(Key &key) {
        if (IsReady(key)) {
            auto res = exceptions.GetException(key);
            return res;
        }
    }

    Master operator=(Master master) = delete;

    template<class _FN, class... _ARGS>
    Key Run(_FN &&_fn, _ARGS &&..._args) {
        AddToQueue(std::bind(std::forward<_FN>(_fn), std::forward<_ARGS>(_args)...));

    }

    template<class _R, class _FN, class... _ARGS>
    Key Run(_FN &&_fn, _ARGS &&... _args) {
        std::function<_R()> task = std::bind(std::forward<_FN>(_fn), std::forward<_ARGS>(_args)...);

        auto result_promise = std::make_shared<std::promise<_R>>();
        std::shared_future<_R> future = result_promise->get_future();

        Key main_key;

        std::function<void()> function = [this, result_promise = std::move(result_promise), f = std::move(task), kop = &main_key]() {
            try {
                _R g = f();
                result_promise->set_value(g);
            } catch (...) {

                //result_promise.set_exception(std::current_exception());

                std::exception_ptr eptr;
                eptr = std::current_exception();
                exceptions.Add(*kop, eptr);
            }

//            _R g = f();
//            result_promise->set_value(g);
        };

        AddToQueue(function);

        FutureContainerElement<_R> *new_element = new FutureContainerElement<_R>(std::move(future));
        futures.Add(main_key, new_element);

        return main_key;
    }

    void DefaultHandler(std::exception_ptr eptr) {
        try {
            if (eptr) {
                std::rethrow_exception(eptr);
            }
        } catch (const std::exception &e) {
            std::cout << "Caught exception \"" << e.what() << "\"\n";
        }
    }

};

#endif //CRYPTO_PLAYGROUND_V2_MASTER_HPP
