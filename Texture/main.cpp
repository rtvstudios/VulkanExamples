//
//  main.cpp
//
//  Created by Ranjith on 5/19/21.
//

#include <iostream>

#include "TextureApp.h"

int main(int argc, const char * argv[]) {
    auto application = std::make_shared<TextureApp>();

    application->create("Texture");
    application->run();

    return 0;
}

