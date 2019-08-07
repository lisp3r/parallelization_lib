#include <iostream>
#include "../include/Master.hpp"

int divide(int a, int b) {
    if(b == 0)
        throw std::runtime_error("divide by zero!");
    else return a/b;
}

int main() {
    Master m(4);

    // auto f1 = m.Run([](){std::cout << "I an here now\n";});

    auto key = m.Run<int>(divide , 5, 0);
    auto key2 = m.Run<char>(divide, 4, 2);
    auto key3 = m.Run<char>(divide, 8, 3);

    m.Start();

    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (m.IsReady(key)) {
        try {
            auto res1 = m.GetData<int>(key);
            std::cout << "ready: " << res1 << std::endl;
        } catch(std::logic_error &le) {
           //std::cout << le.what() << std::endl;
            auto ex = m.GetException(key);
            m.DefaultHandler(ex);
        }
    }

    if (m.IsReady(key2)) {
        try {
            auto res2 = m.GetData<int>(key2);
            std::cout << "ready: " << res2 << std::endl;
        } catch(std::logic_error &le) {
           //std::cout << le.what() << std::endl;
            auto ex = m.GetException(key2);
            m.DefaultHandler(ex);
        }
    }

    if (m.IsReady(key3)) {
        try {
            auto res3 = m.GetData<int>(key3);
            std::cout << "ready: " << res3 << std::endl;
        } catch(std::logic_error &le) {
           //std::cout << le.what() << std::endl;
            auto ex = m.GetException(key3);
            m.DefaultHandler(ex);
        }
    }

    m.Stop();

    // std::cout << "We have " << m.GetNumThreads() << "threads. " << m.GetNumBusyThreads() << " threads is busy\n";


    return 0;
}
