//
//  main.cpp
//  VulkanTest
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "Application.h"

class GLFWWindow: public rtvvulfw::Application {
public:
    GLFWWindow() {
    }
};

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<GLFWWindow>();
    application->createWindow("Vulkan Window");
    application->run();
    
    return 0;
}

