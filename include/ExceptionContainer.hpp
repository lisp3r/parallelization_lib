#ifndef CRYPTO_PLAYGROUND_V2_EXCEPTIONCONTAINER_HPP
#define CRYPTO_PLAYGROUND_V2_EXCEPTIONCONTAINER_HPP

#include "Key.hpp"
#include <mutex>
#include <map>

class ExceptionContainer {
    std::map<Key *, std::exception_ptr> container;
    std::recursive_mutex                container_guard;
public:
    void Add(Key &_key, std::exception_ptr &e_ptr) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        container.insert(std::make_pair(&_key, e_ptr));
    }

    std::exception_ptr GetException(Key &_key) {
        std::unique_lock<std::recursive_mutex> locker(container_guard);
        return container[&_key];
    }

    bool IsKey(Key &key) {
        return !(container.find(&key) == container.end());
    }
};

#endif //CRYPTO_PLAYGROUND_V2_EXCEPTIONCONTAINER_HPP
