#include "asc.h"
#include <iostream>
#include <fstream>
#include <sstream>

ASCReader::Coordinate::Coordinate(double x_val, double y_val, double z_val)
    : x(x_val), y(y_val), z(z_val) {}

ASCReader::Connectivity::Connectivity(int x_val, int y_val, int z_val, int a_val, int b_val, int c_val)
    : x(x_val), y(y_val), z(z_val), a(a_val), b(b_val), c(c_val) {}

ASCReader::ASCReader(const std::string &file_path) : filename(file_path) {}

bool ASCReader::parseCoordinateLine(const std::string &line, Coordinate &coord)
{
    std::istringstream iss(line);
    int index;

    if (iss >> index >> coord.x >> coord.y >> coord.z)
    {
        return true;
    }
    return false;
}

bool ASCReader::parseConnectivityLine(const std::string &line, Connectivity &conn)
{
    std::istringstream iss(line);
    int index;

    if (iss >> index >> conn.x >> conn.y >> conn.z >> conn.a >> conn.b >> conn.c)
    {
        return true;
    }
    return false;
}

bool ASCReader::readFile()
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    int line_number = 0, coordinates_count = 0, connections_count = 0;

    try
    {
        // Skip first few lines and find the start of coordinates
        while (std::getline(file, line))
        {
            line_number++;
            if (line.empty() || line[0] != '*')
            {
                throw new std::runtime_error("Invalid line in the beginning of file at line " + std::to_string(line_number) + ": " + line);
            }
            else if (line.substr(0, 6) == "*nodes")
            {
                std::istringstream iss(line);
                std::string temp;

                if (iss >> temp >> coordinates_count)
                {
                    std::cout << "Coordinates count: " << coordinates_count << std::endl;
                    break;
                }
                else
                {
                    throw new std::runtime_error("Invalid format in *nodes line: " + line);
                }
            }
        }

        // Parse coordinates
        line_number = 0;
        coordinates.reserve(coordinates_count);
        while (std::getline(file, line) && line_number < coordinates_count)
        {
            line_number++;
            if (line.empty())
                continue;

            Coordinate coord;
            if (parseCoordinateLine(line, coord))
            {
                coordinates.push_back(coord);
            }
            else
            {
                std::cerr << "Warning: Invalid format at line "
                          << line_number << ": " << line << std::endl;
            }
        }

        // Read the number of connections
        std::getline(file, line);
        std::cout << "Line: " << line << std::endl;
        std::istringstream iss(line);
        std::string temp;
        if (iss >> temp >> connections_count && temp[0] == '*')
        {
            std::cout << "Connections count: " << connections_count << std::endl;
        }
        else
        {
            throw new std::runtime_error("Invalid format in connections line: " + line);
        }

        // Parse connections
        line_number = 0;
        connections.reserve(connections_count);
        while (std::getline(file, line) && line_number < connections_count)
        {
            line_number++;
            if (line.empty())
                continue;

            Connectivity conn;
            if (parseConnectivityLine(line, conn))
            {
                connections.push_back(conn);
            }
            else
            {
                std::cerr << "Warning: Invalid format at line "
                          << line_number << ": " << line << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        file.close();
        return false;
    }

    file.close();
    coordinates.shrink_to_fit();
    connections.shrink_to_fit();
    return true;
}

const std::vector<ASCReader::Coordinate>& ASCReader::getCoordinates() const
{
    return coordinates;
}

int ASCReader::getCoordinatesCount() const
{
    return coordinates.size();
}

void ASCReader::clearCoordinates()
{
    coordinates.clear();
    coordinates.shrink_to_fit();
}

const std::vector<ASCReader::Connectivity>& ASCReader::getConnections() const
{
    return connections;
}

int ASCReader::getConnectionsCount() const
{
    return connections.size();
}

void ASCReader::clearConnections()
{
    connections.clear();
    connections.shrink_to_fit();
}