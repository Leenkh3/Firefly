#include <iostream>
#include "asc.h"

int quantityTest(ASCReader &reader)
{
    int coord_count = reader.getCoordinatesCount();
    int conn_count = reader.getConnectionsCount();
    if (coord_count == 0 || conn_count == 0)
    {
        std::cout << "Error: No coordinates or connections found." << std::endl;
        return 1;
    }

    std::cout << "Successfully read " << coord_count << " coordinates." << std::endl;
    std::cout << "Successfully read " << conn_count << " connections." << std::endl;
    return 0;
}

int qualityTest(ASCReader &reader)
{
    ASCReader::Coordinate first_coordinate(4.71160561e-01, 3.99486482e-01, 4.33099747e-01);
    ASCReader::Coordinate last_coordinate(3.32090229e-01, 5.67535460e-01, 8.59776497e-01);

    const std::vector<ASCReader::Coordinate> &coords = reader.getCoordinates();
    int last_index = coords.size() - 1;

    if (coords[0].x != first_coordinate.x || coords[0].y != first_coordinate.y || coords[0].z != first_coordinate.z)
    {
        std::cout << "Error: First coordinate does not match expected values." << std::endl;
        return 1;
    }
    if (coords[last_index].x != last_coordinate.x || coords[last_index].y != last_coordinate.y || coords[last_index].z != last_coordinate.z)
    {
        std::cout << "Error: Last coordinate does not match expected values." << std::endl;
        return 1;
    }
    std::cout << "First and last coordinates match expected values." << std::endl;

    ASCReader::Connectivity first_connection(1, 4, 1, 2, 3, 4);
    ASCReader::Connectivity last_connection(1, 4, 1049, 1039, 6679, 1040);

    const std::vector<ASCReader::Connectivity> &conns = reader.getConnections();
    last_index = conns.size() - 1;

    if (conns[0].x != first_connection.x || conns[0].y != first_connection.y || conns[0].z != first_connection.z ||
        conns[0].a != first_connection.a || conns[0].b != first_connection.b || conns[0].c != first_connection.c)
    {
        std::cout << "Error: First connectivity does not match expected values." << std::endl;
        return 1;
    }
    if (conns[last_index].x != last_connection.x || conns[last_index].y != last_connection.y || conns[last_index].z != last_connection.z || 
        conns[last_index].a != last_connection.a || conns[last_index].b != last_connection.b || conns[last_index].c != last_connection.c)
    {
        std::cout << "Error: Last connectivity does not match expected values." << std::endl;
        return 1;
    }
    std::cout << "First and last connectivities match expected values." << std::endl;

    return 0;
}

int main()
{
    ASCReader reader("Resources/sedov_coarse.asc_mesh");
    if (reader.readFile())
    {
        if (quantityTest(reader) != 0)
        {
            std::cout << "Quantity test failed." << std::endl;
            return 1;
        }
        if (qualityTest(reader) != 0)
        {
            std::cout << "Quality test failed." << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "Failed to read coordinates file" << std::endl;
        return 1;
    }
}