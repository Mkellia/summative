#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For sleep()

// Log entry structure
typedef struct Log {
    char timestamp[20];
    float temperature;
    float humidity;
    float pressure;
    struct Log* prev;
    struct Log* next;
} Log;

Log* head = NULL;
Log* tail = NULL;
Log* current = NULL;
int playing = 0;

// Function to create and append a log
void addLog(const char* timestamp, float temp, float hum, float pres) {
    Log* newLog = (Log*)malloc(sizeof(Log));
    strcpy(newLog->timestamp, timestamp);
    newLog->temperature = temp;
    newLog->humidity = hum;
    newLog->pressure = pres;
    newLog->prev = newLog->next = NULL;

    if (!head) {
        head = tail = newLog;
    } else {
        tail->next = newLog;
        newLog->prev = tail;
        tail = newLog;
    }
}

// Display current log
void displayLog(Log* log) {
    if (log) {
        printf("\nTimestamp: %s\nTemp: %.2f°C, Humidity: %.2f%%, Pressure: %.2f hPa\n",
               log->timestamp, log->temperature, log->humidity, log->pressure);
    } else {
        printf("No log available.\n");
    }
}

// Simulated log playback
void playLogs() {
    playing = 1;
    while (playing && current) {
        displayLog(current);
        if (current->next) {
            current = current->next;
            sleep(2); // Simulated delay
        } else {
            printf("End of log reached.\n");
            break;
        }
    }
}

// Stop and reset
void stopSystem() {
    playing = 0;
    current = head;
    printf("\nSystem stopped and reset to first log.\n");
}

int main() {
    // Simulated logs (in real use, logs would be loaded from EEPROM or sensors)
    addLog("2025-06-01 10:00", 22.5, 45.2, 1013.1);
    addLog("2025-06-01 11:00", 23.1, 46.0, 1012.9);
    addLog("2025-06-01 12:00", 24.0, 44.8, 1012.7);
    addLog("2025-06-01 13:00", 25.3, 43.5, 1012.4);

    current = head;
    char command[10];

    printf("Embedded Log System Started.\n");
    displayLog(current);

    while (1) {
        printf("\nCommands: n (next), p (prev), play, pause, stop, exit\n> ");
        scanf("%s", command);

        if (strcmp(command, "n") == 0) {
            if (current->next) {
                current = current->next;
                displayLog(current);
            } else {
                printf("Already at latest log.\n");
            }
        } else if (strcmp(command, "p") == 0) {
            if (current->prev) {
                current = current->prev;
                displayLog(current);
            } else {
                printf("Already at oldest log.\n");
            }
        } else if (strcmp(command, "play") == 0) {
            playLogs();
        } else if (strcmp(command, "pause") == 0) {
            playing = 0;
            printf("Playback paused.\n");
        } else if (strcmp(command, "stop") == 0) {
            stopSystem();
            displayLog(current);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting system.\n");
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    // Cleanup
    Log* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
