#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100
#define INF INT_MAX

// City junctions
const char* nodes[] = {
    "Dispatch Center", "Sector A", "Sector B", "Sector D",
    "Junction C", "Sector E", "Emergency Site"
};

#define NODE_COUNT 7

int getIndex(const char* name) {
    for (int i = 0; i < NODE_COUNT; i++) {
        if (strcmp(nodes[i], name) == 0)
            return i;
    }
    return -1;
}

// Adjacency matrix with weights
int graph[NODE_COUNT][NODE_COUNT];

// Dijkstra's algorithm
void dijkstra(int start, int end) {
    int dist[NODE_COUNT];
    bool visited[NODE_COUNT];
    int prev[NODE_COUNT];

    for (int i = 0; i < NODE_COUNT; i++) {
        dist[i] = INF;
        visited[i] = false;
        prev[i] = -1;
    }

    dist[start] = 0;

    for (int i = 0; i < NODE_COUNT - 1; i++) {
        int u = -1;
        for (int j = 0; j < NODE_COUNT; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (u == -1) break; // No more reachable nodes

        visited[u] = true;

        for (int v = 0; v < NODE_COUNT; v++) {
            if (graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    if (dist[end] == INF) {
        printf("\nNo path found from %s to %s.\n", nodes[start], nodes[end]);
        return;
    }

    // Output path
    printf("\nShortest path: ");
    int path[MAX];
    int count = 0;
    for (int v = end; v != -1; v = prev[v]) {
        path[count++] = v;
    }
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", nodes[path[i]]);
        if (i != 0) printf(" -> ");
    }
    printf("\nTotal travel time: %d minutes\n", dist[end]);
}

void initGraph() {
    // Initialize all to 0
    for (int i = 0; i < NODE_COUNT; i++)
        for (int j = 0; j < NODE_COUNT; j++)
            graph[i][j] = 0;

    // Define travel times (edges)
    graph[getIndex("Dispatch Center")][getIndex("Sector A")] = 10;
    graph[getIndex("Dispatch Center")][getIndex("Sector D")] = 30;
    graph[getIndex("Sector A")][getIndex("Sector B")] = 10;
    graph[getIndex("Sector B")][getIndex("Emergency Site")] = 15;
    graph[getIndex("Sector D")][getIndex("Emergency Site")] = 5;
    graph[getIndex("Sector B")][getIndex("Junction C")] = 3;
    graph[getIndex("Junction C")][getIndex("Sector E")] = 6;
    graph[getIndex("Sector E")][getIndex("Emergency Site")] = 4;

    // Make graph bidirectional
    for (int i = 0; i < NODE_COUNT; i++) {
        for (int j = 0; j < NODE_COUNT; j++) {
            if (graph[i][j] != 0) {
                graph[j][i] = graph[i][j];
            }
        }
    }
}

int main() {
    initGraph();

    char startNode[50];
    char endNode[50];

    printf("Available nodes:\n");
    for (int i = 0; i < NODE_COUNT; i++) {
        printf(" - %s\n", nodes[i]);
    }

    while (1) {
        printf("\nEnter starting location (or 'exit'): ");
        scanf(" %[^\n]", startNode);
        if (strcmp(startNode, "exit") == 0) break;

        printf("Enter destination location (or 'exit'): ");
        scanf(" %[^\n]", endNode);
        if (strcmp(endNode, "exit") == 0) break;

        int startIndex = getIndex(startNode);
        int endIndex = getIndex(endNode);

        if (startIndex == -1) {
            printf("Starting location '%s' not found.\n", startNode);
            continue;
        }
        if (endIndex == -1) {
            printf("Destination location '%s' not found.\n", endNode);
            continue;
        }

        dijkstra(startIndex, endIndex);
    }

    printf("Program exited.\n");
    return 0;
}
