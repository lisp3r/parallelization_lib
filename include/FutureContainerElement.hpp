#ifndef CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_HPP
#define CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_HPP

#include <future>
#include "FutureContainerElement_base.hpp"

template<class F>
class FutureContainerElement : public FutureContainerElement_base {
    std::shared_future<F> future;
    F value;
public:
    FutureContainerElement(std::shared_future<F> _f) : future(std::move(_f)) {}

    FutureContainerElement(const FutureContainerElement &);

    FutureContainerElement &operator=(const FutureContainerElement &);

    FutureContainerElement(FutureContainerElement &&other) : value(0) {
        future = std::move(other.future);
        value = other.value;
        other.value = 0;
    }

    FutureContainerElement &operator=(FutureContainerElement &&other) {
        if (this != &other) {
            value = other.value;
            future = std::move(other.future);
            other.value = 0;
        }
        return *this;
    }

    bool IsFutureReady() {
        return (std::future_status::ready == future.wait_for(std::chrono::microseconds(0)));
    }

    void *GetData() {
        value = future.get();
        return &value;
    }
};

#endif //CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_HPP
