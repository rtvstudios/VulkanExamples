#ifndef NonCopyable_h
#define NonCopyable_h

namespace rvkfw {

class RNonCopyable {
public:
    RNonCopyable(const RNonCopyable &) = delete;
    RNonCopyable& operator=(const RNonCopyable &) = delete;
    RNonCopyable() = default;
};

}

#endif
