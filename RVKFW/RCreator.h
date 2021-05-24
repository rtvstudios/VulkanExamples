
#ifndef RCreator_h
#define RCreator_h

#include <memory>

namespace rvkfw {

class RCreator {
public:
    template <typename Impl, typename ... CreateArgs>
    static std::shared_ptr<Impl> create(CreateArgs&& ... args) {
        auto obj = std::make_shared<Impl>();
        obj->create(std::forward<CreateArgs>(args)...);
        return obj;
    }
};

}

#endif
