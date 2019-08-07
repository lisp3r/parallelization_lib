#ifndef CRYPTO_PLAYGROUND_V2_FUTURECONTAINER_HPP
#define CRYPTO_PLAYGROUND_V2_FUTURECONTAINER_HPP

#include <map>
#include <mutex>
#include "Key.hpp"
#include "FutureContainerElement_base.hpp"

class FutureContainer {
    std::map<Key *, FutureContainerElement_base *>  container;
    std::recursive_mutex                            container_guard;
public:
    template<class F>
    void Add(Key &k, F &sb) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        container.insert(std::make_pair(&k, sb));
    }

//    FutureContainerElement_base *GetElement(Key &key) {
//        std::unique_lock<std::recursive_mutex> locker(container_guard);
//        return container[&key];
//    }

    bool IsKey(Key &key) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        return !(container.find(&key) == container.end());
    }

    bool IsDataReady(Key &key) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        if (IsKey(key)) {
            return container[&key]->IsFutureReady();
        }
    }

    template<class F>
    F GetData(Key &key) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        void *data = nullptr;
        if (IsKey(key)) {
            data = container[&key]->GetData();
        }
        F val = *(F *) data;
        return val;
    }
};

#endif //CRYPTO_PLAYGROUND_V2_FUTURECONTAINER_HPP
