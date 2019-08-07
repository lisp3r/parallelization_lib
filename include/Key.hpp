#ifndef CRYPTO_PLAYGROUND_V2_KEY_HPP
#define CRYPTO_PLAYGROUND_V2_KEY_HPP

#include <uuid/uuid.h>

class Key {
    uuid_t val;
public:
    Key() { uuid_generate(val); }

    Key(uuid_t _val) { uuid_copy(val, _val); }

    ~Key() { uuid_clear(val); }

    bool Compare(Key _uuid) { return !uuid_compare(val, _uuid.val); }

    void Clear() { uuid_clear(val); }

    Key &operator=(const Key &_key) {
        uuid_copy(val, _key.val);
    }
};


#endif //CRYPTO_PLAYGROUND_V2_KEY_HPP
