
#ifndef RCreator_h
#define RCreator_h

#include <memory>

namespace rvkfw {

class RCreator {
public:
    template <typename Impl, typename ... Args>
    static std::shared_ptr<Impl> create(Args&& ... args) {
        auto obj = std::make_shared<Impl>();
        obj->create(std::forward<Args>(args)...);
        return obj;
    }
};

}

#endif
