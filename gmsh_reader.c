#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10000   // Adjust for larger meshes
#define MAX_ELEMENTS 10000

// Structure for nodes (grid points)
typedef struct {
    int id;
    double x, y, z;
} Node;

// Structure for elements (connectivity)
typedef struct {
    int id;
    int *node_ids;  // Dynamically allocated array for node IDs
    int num_nodes;
} Element;

// Function to read a GMSH file and extract node coordinates & connectivity
void read_gmsh(const char *filename, Node **nodes, int *num_nodes, Element **elements, int *num_elements) {
    int i, j;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *nodes = malloc(MAX_NODES * sizeof(Node));
    *elements = malloc(MAX_ELEMENTS * sizeof(Element));
    *num_nodes = 0;
    *num_elements = 0;

    if (!*nodes || !*elements) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // Read Nodes Section
        if (strncmp(line, "$Nodes", 6) == 0) {
            fgets(line, sizeof(line), file);
            sscanf(line, "%d", num_nodes);

            for (i = 0; i < *num_nodes; i++) {
                fgets(line, sizeof(line), file);
                sscanf(line, "%d %lf %lf %lf", &(*nodes)[i].id, &(*nodes)[i].x, &(*nodes)[i].y, &(*nodes)[i].z);
            }
        }

        // Read Elements Section
        if (strncmp(line, "$Elements", 9) == 0) {
            fgets(line, sizeof(line), file);
            sscanf(line, "%d", num_elements);

            for (i = 0; i < *num_elements; i++) {
                int elem_id, elem_type, num_tags, node_id;
                fgets(line, sizeof(line), file);
                char *token = strtok(line, " ");

                elem_id = atoi(token);
                token = strtok(NULL, " ");
                elem_type = atoi(token);
                token = strtok(NULL, " ");
                num_tags = atoi(token);

                for (j = 0; j < num_tags; j++)
                    token = strtok(NULL, " ");

                (*elements)[i].id = elem_id;
                (*elements)[i].num_nodes = 0;
                (*elements)[i].node_ids = malloc(8 * sizeof(int)); // Support up to 8-node elements

                if (!(*elements)[i].node_ids) {
                    perror("Memory allocation failed");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }

                while ((token = strtok(NULL, " ")) != NULL) {
                    node_id = atoi(token);
                    (*elements)[i].node_ids[(*elements)[i].num_nodes++] = node_id;
                }
            }
        }
    }

    fclose(file);
}

void free_elements(Element *elements, int num_elements) {
    int i;
    for (i = 0; i < num_elements; i++) {
        free(elements[i].node_ids);
    }
    free(elements);
}

int main() {
    Node *nodes;
    Element *elements;
    int num_nodes, num_elements, i, j;

    read_gmsh("example.msh", &nodes, &num_nodes, &elements, &num_elements);

    printf("Node Coordinates:\n");
    for (i = 0; i < num_nodes; i++) {
        printf("ID %d: (%.2lf, %.2lf, %.2lf)\n", nodes[i].id, nodes[i].x, nodes[i].y, nodes[i].z);
    }

    printf("\nElement Connectivity:\n");
    for (i = 0; i < num_elements; i++) {
        printf("Element %d: ", elements[i].id);
        for (j = 0; j < elements[i].num_nodes; j++) {
            printf("%d ", elements[i].node_ids[j]);
        }
        printf("\n");
    }

    free(nodes);
    free_elements(elements, num_elements);

    return 0;
}

