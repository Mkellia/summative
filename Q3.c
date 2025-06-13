#include <stdio.h>
#include <string.h>

#define MAX_DEVICES 10

// Device list and adjacency matrix
const char* devices[MAX_DEVICES] = {
    "D001", "D002", "D003", "D004",
    "D005", "D006", "D007", "D008"
};

int adjMatrix[MAX_DEVICES][MAX_DEVICES] = {0};
int deviceCount = 8;

// Find index of device ID
int getDeviceIndex(const char* id) {
    for (int i = 0; i < deviceCount; i++) {
        if (strcmp(devices[i], id) == 0)
            return i;
    }
    return -1;
}

// Add edge to graph
void addCommunication(const char* from, const char* to) {
    int i = getDeviceIndex(from);
    int j = getDeviceIndex(to);
    if (i != -1 && j != -1) {
        adjMatrix[i][j] = 1;  // Directed edge from -> to
    }
}

// Print directly connected devices
void showConnectedDevices(const char* id) {
    int idx = getDeviceIndex(id);
    if (idx == -1) {
        printf("\nDevice %s not found.\n", id);
        return;
    }

    printf("\nDevices directly connected to %s:\n", id);
    for (int j = 0; j < deviceCount; j++) {
        if (adjMatrix[idx][j]) {
            printf("  %s\n", devices[j]);
        }
    }
}

// Print adjacency matrix
void printAdjMatrix() {
    printf("\nAdjacency Matrix:\n      ");
    for (int i = 0; i < deviceCount; i++) {
        printf("%s ", devices[i]);
    }
    printf("\n");
    for (int i = 0; i < deviceCount; i++) {
        printf("%s ", devices[i]);
        for (int j = 0; j < deviceCount; j++) {
            printf("   %d   ", adjMatrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Build graph based on logs
    addCommunication("D001", "D002");
    addCommunication("D001", "D003");
    addCommunication("D002", "D004");
    addCommunication("D003", "D005");
    addCommunication("D004", "D005");
    addCommunication("D004", "D006");
    addCommunication("D005", "D007");
    addCommunication("D006", "D008");

    printAdjMatrix();

    char input[10];
    while (1) {
        printf("\nEnter device ID to view connections (or 'exit'): ");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) break;

        showConnectedDevices(input);
    }

    return 0;
}
