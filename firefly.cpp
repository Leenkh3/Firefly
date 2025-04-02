#include <iostream>

int main()
{
    std::cout << "Firefly Project - Main Execution" << std::endl;

    try
    {
        ASCReader reader("Resources/sedov_coarse.asc_mesh");

        if (reader.readFile())
        {
            const auto &coords = reader.getCoordinates();
            std::cout << "Successfully read " << reader.getCoordinatesCount()
                      << " coordinates:" << std::endl;
        }
        else
        {
            std::cout << "Failed to read coordinates file" << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
