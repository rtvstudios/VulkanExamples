//
//  main.cpp
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "TransformationApp.h"

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<TransformationApp>();

    application->create("Transformations");
    application->run();

    return 0;
}

