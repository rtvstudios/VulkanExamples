
#ifndef RCreator_h
#define RCreator_h

#include <memory>

#include "RObject.h"

namespace rvkfw {

template<typename Impl>
class RCreator {
public:
    template <typename ... ConstructorArgs>
    RCreator(ConstructorArgs&& ... args) {
        mObject = std::make_shared<Impl>(std::forward<ConstructorArgs>(args)...);
    }

    template <typename ... CreateArgs>
    std::shared_ptr<Impl> create(CreateArgs&& ... args) {
        mObject->create(std::forward<CreateArgs>(args)...);
        return mObject;
    }

    std::shared_ptr<Impl> mObject;

};

}

#endif
