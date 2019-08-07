#ifndef CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_BASE_HPP
#define CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_BASE_HPP

class FutureContainerElement_base {
public:
    virtual void *GetData() = 0;
    virtual bool IsFutureReady() = 0;
};


#endif //CRYPTO_PLAYGROUND_V2_FUTURECONTAINERELEMENT_BASE_HPP
