//
//  main.cpp
//  VulkanTest
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "HelloTriangleApp.h"

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<HelloTriangle>();
    
    application->create("Hello Triangle");
    application->run();
    
    return 0;
}

