
#ifndef ASC_READER_H
#define ASC_READER_H

#include <string>
#include <vector>

class ASCReader
{
public:
    struct Coordinate
    {
        double x;
        double y;
        double z;

        Coordinate(double x_val = 0.0, double y_val = 0.0, double z_val = 0.0);
    };

    struct Connectivity
    {
        int x;
        int y;
        int z;
        int a;
        int b;
        int c;

        Connectivity(int x_val = 0, int y_val = 0, int z_val = 0, int a_val = 0, int b_val = 0, int c_val = 0);
    };

    explicit ASCReader(const std::string &file_path);

    bool readFile();
    const std::vector<Coordinate> &getCoordinates() const;
    int getCoordinatesCount() const;
    void clearCoordinates();
    const std::vector<Connectivity> &getConnections() const;
    int getConnectionsCount() const;
    void clearConnections();

private:
    std::string filename;
    std::vector<Coordinate> coordinates;
    std::vector<Connectivity> connections;

    bool parseLine(const std::string &line, Coordinate &coord);
};

#endif