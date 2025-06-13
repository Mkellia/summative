#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BST Node
typedef struct Node {
    char name[50];
    struct Node* left;
    struct Node* right;
} Node;

// Create a new BST node
Node* insert(Node* root, const char* name) {
    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        strcpy(newNode->name, name);
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (strcmp(name, root->name) < 0)
        root->left = insert(root->left, name);
    else if (strcmp(name, root->name) > 0)
        root->right = insert(root->right, name);

    return root;
}

// Search for exact match
int search(Node* root, const char* name) {
    if (root == NULL) return 0;
    int cmp = strcmp(name, root->name);
    if (cmp == 0) return 1;
    else if (cmp < 0) return search(root->left, name);
    else return search(root->right, name);
}

// Minimum of three numbers
int min(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    else if (b <= a && b <= c) return b;
    return c;
}

// Levenshtein Distance
int levenshtein(const char* s, const char* t) {
    int len_s = strlen(s);
    int len_t = strlen(t);
    int d[len_s+1][len_t+1];

    for (int i = 0; i <= len_s; i++) d[i][0] = i;
    for (int j = 0; j <= len_t; j++) d[0][j] = j;

    for (int i = 1; i <= len_s; i++) {
        for (int j = 1; j <= len_t; j++) {
            int cost = (s[i-1] == t[j-1]) ? 0 : 1;
            d[i][j] = min(
                d[i-1][j] + 1,
                d[i][j-1] + 1,
                d[i-1][j-1] + cost
            );
        }
    }
    return d[len_s][len_t];
}

// Find closest match
void findClosest(Node* root, const char* input, char* bestMatch, int* minDistance) {
    if (!root) return;
    int distance = levenshtein(input, root->name);
    if (distance < *minDistance) {
        *minDistance = distance;
        strcpy(bestMatch, root->name);
    }
    findClosest(root->left, input, bestMatch, minDistance);
    findClosest(root->right, input, bestMatch, minDistance);
}

// Handle access logic
void handleAccess(Node* root, const char* name) {
    if (search(root, name)) {
        printf("\n✅ Access Granted. Welcome %s!\n", name);
        return;
    }

    char suggestion[50];
    int minDist = 999;
    findClosest(root, name, suggestion, &minDist);

    if (minDist <= 2) {
        printf("\n❗Name not found. Did you mean: %s?\n", suggestion);
    } else {
        printf("\n❌ Name '%s' not recognized. Logged for review.\n", name);
        FILE* logFile = fopen("unrecognized_names.log", "a");
        if (logFile) {
            fprintf(logFile, "%s\n", name);
            fclose(logFile);
        }
    }
}

// Load names from file
Node* loadAuthorizedNames(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        return NULL;
    }

    Node* root = NULL;
    char name[50];
    while (fgets(name, sizeof(name), file)) {
        name[strcspn(name, "\n")] = 0; // Remove newline
        root = insert(root, name);
    }
    fclose(file);
    return root;
}

int main() {
    Node* bst = loadAuthorizedNames("authorized_names.txt");
    if (!bst) return 1;

    char input[50];
    while (1) {
        printf("\nEnter your name for access (or 'exit'): ");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) break;

        handleAccess(bst, input);
    }

    return 0;
}
