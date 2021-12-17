#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <filesystem>
#include <iomanip>
#include "direct.h"

#include "cPackage.h"

int main(int argc, char *argv[])
{
    cAllPackages thePacks;
    

    if (argc == 1)
    {
        std::cout << thePacks.list();
        return 0;
    }

    auto P = thePacks.find(argv[1]);
    if (P.userName().empty())
        throw std::runtime_error(
            std::string("Cannot find ") + argv[1]);
    P.clone();
    P.publish(thePacks.packFolder() + "/pub/src");
    std::cout << P.userName() << " published to "
              << thePacks.packFolder() << "/pub/src";
}