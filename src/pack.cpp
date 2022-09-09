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

#include "cCommandParser.h"
#include "cPackage.h"

cAllPackages thePacks;

void commandParse(int ac, char **av)
{
    if (ac == 1)
    {
        std::cout << thePacks.list();
        exit(0);
    }

    raven::set::cCommandParser P;

    P.add("pack", "name of package to load");
    P.add("target", "absolute path to folder containing repo to update");

    P.parse(ac, av);

    thePacks.find(
        P.value("pack"));
    thePacks.targetFolder(
        P.value("target"));
}

int main(int argc, char *argv[])
{

    commandParse(argc, argv);

    thePacks.clone();
    thePacks.publish();
}