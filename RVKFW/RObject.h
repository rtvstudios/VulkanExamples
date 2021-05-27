#ifndef RObject_h
#define RObject_h

#include "RCommon.h"
#include "RNonCopyable.h"
#include "RLogger.h"

namespace rvkfw {

class RObject: private RNonCopyable {
public:
    RObject() = default;
    ~RObject() = default;

    virtual const char *tag() const {
        return "RObject";
    }

    virtual void preCreate() {}
    virtual void destroy() = 0;

protected:
    std::atomic_bool mCreated{ false };
};

}

#endif
