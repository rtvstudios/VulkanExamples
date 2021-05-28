//
//  main.cpp
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "RectangleApp.h"

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<RectangleApp>();

    application->create("Rectangle");
    application->run();

    return 0;
}

