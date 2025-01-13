//
//  main.cpp
//  METALSolarSystem
//
//  Created by Jacky Lei on 1/1/25.
//

#include <iostream>
#include "MetalEngine/app_delegate.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate( &del );
    pSharedApplication->run();

    pAutoreleasePool->release();

    return 0;
}
