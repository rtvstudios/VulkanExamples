#ifndef NonCopyable_h
#define NonCopyable_h

namespace rtvvulfw {

class NonCopyable {
public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable& operator=(const NonCopyable &) = delete;
    NonCopyable() = default;
};

}

#endif
