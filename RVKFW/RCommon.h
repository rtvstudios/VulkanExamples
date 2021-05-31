
#ifndef RCommon_h
#define RCommon_h

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <atomic>
#include <functional>

namespace rvkfw {
}

#define ASSERT_NOT_NULL_TAG(obj, tag) if (!obj) { LOG_ERROR(tag, "Could not get object " << #obj); \
                                         throw std::runtime_error("required objects " #obj " is not available!"); }

#define ASSERT_NOT_NULL(obj) ASSERT_NOT_NULL_TAG(obj, tag())

#endif

