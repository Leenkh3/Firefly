#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10000
#define MAX_ELEMENTS 10000

// Node structure
typedef struct {
    int id;
    double x, y, z;
} Node;

// Element structure
typedef struct {
    int id;
    int node_ids[8]; // Max 8 nodes per element
    int num_nodes;
} Element;

// Read GMSH 4.1 files
void read_gmsh(const char *filename, Node *nodes, int *num_nodes, Element *elements, int *num_elements) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *num_nodes = 0;
    *num_elements = 0;

    int reading_nodes = 0;
    int reading_elements = 0;
    int i;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "$Nodes", 6) == 0) { 
            reading_nodes = 1; 
            fgets(line, sizeof(line), file); // Skip next line with node count headers
            continue; 
        }
        if (strncmp(line, "$EndNodes", 9) == 0) { 
            reading_nodes = 0; 
            continue; 
        }
        if (strncmp(line, "$Elements", 9) == 0) { 
            reading_elements = 1; 
            fgets(line, sizeof(line), file); // Skip next line with element count headers
            continue; 
        }
        if (strncmp(line, "$EndElements", 12) == 0) { 
            reading_elements = 0; 
            continue; 
        }

        if (reading_nodes && *num_nodes < MAX_NODES) {
            int id;
            double x, y, z;
            if (sscanf(line, "%d %lf %lf %lf", &id, &x, &y, &z) == 4) {
                nodes[*num_nodes].id = id;
                nodes[*num_nodes].x = x;
                nodes[*num_nodes].y = y;
                nodes[*num_nodes].z = z;
                (*num_nodes)++;
            } else {
                fprintf(stderr, "Invalid node format: %s", line);
            }
        }

        if (reading_elements && *num_elements < MAX_ELEMENTS) {
            int id, num_tags, element_type, num_nodes;
            char *token = strtok(line, " ");
            if (token) id = atoi(token); else continue;
            token = strtok(NULL, " "); if (token) element_type = atoi(token); else continue;
            token = strtok(NULL, " "); if (token) num_tags = atoi(token); else continue;

            for (i = 0; i < num_tags; i++) {
                token = strtok(NULL, " ");
            }

            num_nodes = (element_type == 4) ? 4 : (element_type == 8) ? 8 : 0;
            elements[*num_elements].id = id;
            elements[*num_elements].num_nodes = num_nodes;

            for (i = 0; i < num_nodes; i++) {
                token = strtok(NULL, " ");
                if (token) {
                    elements[*num_elements].node_ids[i] = atoi(token);
                } else {
                    fprintf(stderr, "Error reading node for element %d\n", id);
                    elements[*num_elements].node_ids[i] = -1;
                }
            }
            (*num_elements)++;
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.msh>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Node nodes[MAX_NODES];
    Element elements[MAX_ELEMENTS];
    int num_nodes, num_elements;
    int i, j;

    read_gmsh(argv[1], nodes, &num_nodes, elements, &num_elements);

    printf("\nNode Coordinates:\n");
    i = 0;
    while (i < num_nodes) {
        printf("ID %d: (%.6lf, %.6lf, %.6lf)\n", nodes[i].id, nodes[i].x, nodes[i].y, nodes[i].z);
        i++;
    }

    printf("\nElement Connectivity:\n");
    i = 0;
    while (i < num_elements) {
        printf("Element %d: ", elements[i].id);
        j = 0;
        while (j < elements[i].num_nodes) {
            printf("%d ", elements[i].node_ids[j]);
            j++;
        }
        printf("\n");
        i++;
    }

    return 0;
}
