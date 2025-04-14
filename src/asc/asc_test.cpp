#include <iostream>
#include "asc.h"

int test()
{
    ASCReader reader("Resources/sedov_coarse.asc_mesh");

    if (reader.readFile())
    {
        const auto &coords = reader.getCoordinates();
        std::cout << "Successfully read " << reader.getCoordinatesCount()
                  << " coordinates:" << std::endl;

        const auto &conns = reader.getConnections();
        std::cout << "Successfully read " << reader.getConnectionsCount()
                << " connections:" << std::endl;

        return 0;
    }
    else
    {
        std::cout << "Failed to read coordinates file" << std::endl;
        return 1;
    }
}

int main()
{
    return test();
}