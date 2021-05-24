//
//  main.cpp
//  VulkanTest
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "RApplication.h"

class GLFWWindow: public rtvvulfw::RApplication {
public:
    GLFWWindow() {
    }
};

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<GLFWWindow>();
    application->createWindow("First Vulkan RWindow");
    application->run();
    
    return 0;
}

