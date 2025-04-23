//This code converts simplified mesh data from `output.txt` (as exported by the GMSH reader) into a valid GMSH `.msh` file in ASCII version 2.2 format.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10000
#define MAX_ELEMENTS 10000

typedef struct {
    int id;
    double x, y, z;
} Node;

typedef struct {
    int id;
    int node_ids[4];  // Tetrahedrons have 4 nodes
} Element;

void write_gmsh_mesh(const char *filename, Node *nodes, int num_nodes, Element *elements, int num_elements) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Could not open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "$MeshFormat\n2.2 0 8\n$EndMeshFormat\n");

    // Write nodes
    fprintf(fp, "$Nodes\n%d\n", num_nodes);
    int i;
    for (i = 0; i < num_nodes; i++) {
        fprintf(fp, "%d %.6f %.6f %.6f\n", nodes[i].id, nodes[i].x, nodes[i].y, nodes[i].z);
    }
    fprintf(fp, "$EndNodes\n");

    // Write elements
    fprintf(fp, "$Elements\n%d\n", num_elements);
    int j;
    for (i = 0; i < num_elements; i++) {
        fprintf(fp, "%d 4 2 0 0 %d %d %d %d\n", 
                elements[i].id,
                elements[i].node_ids[0],
                elements[i].node_ids[1],
                elements[i].node_ids[2],
                elements[i].node_ids[3]);
    }
    fprintf(fp, "$EndElements\n");

    fclose(fp);
}

void process_input_data(const char *filename, Node *nodes, int *num_nodes, Element *elements, int *num_elements) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Could not open input file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *num_nodes = 0;
    *num_elements = 0;

    while (fgets(line, sizeof(line), fp)) {
        int token_count = 0;
        char temp[256];
        strcpy(temp, line);
        char *tok = strtok(temp, " \t\n");
        while (tok) {
            token_count++;
            tok = strtok(NULL, " \t\n");
        }

        if (token_count == 4) {
            // Node line
            int id;
            double x, y, z;
            if (sscanf(line, "%d %lf %lf %lf", &id, &x, &y, &z) == 4) {
                nodes[*num_nodes].id = id;
                nodes[*num_nodes].x = x;
                nodes[*num_nodes].y = y;
                nodes[*num_nodes].z = z;
                (*num_nodes)++;
            }
        } else if (token_count == 5) {
            // Element line
            int id, n1, n2, n3, n4;
            if (sscanf(line, "%d %d %d %d %d", &id, &n1, &n2, &n3, &n4) == 5) {
                elements[*num_elements].id = id;
                elements[*num_elements].node_ids[0] = n1;
                elements[*num_elements].node_ids[1] = n2;
                elements[*num_elements].node_ids[2] = n3;
                elements[*num_elements].node_ids[3] = n4;
                (*num_elements)++;
            }
        }
    }

    fclose(fp);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.txt output.msh\n", argv[0]);
        return EXIT_FAILURE;
    }

    Node nodes[MAX_NODES];
    Element elements[MAX_ELEMENTS];
    int num_nodes = 0, num_elements = 0;

    process_input_data(argv[1], nodes, &num_nodes, elements, &num_elements);
    write_gmsh_mesh(argv[2], nodes, num_nodes, elements, num_elements);

    printf("Conversion successful: %d nodes, %d elements written to %s\n", num_nodes, num_elements, argv[2]);
    return 0;
}

