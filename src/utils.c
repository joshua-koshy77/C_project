#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void currentTimestamp(char *out, size_t len) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(out, len, "%Y-%m-%d %H:%M:%S", tm_info);
}

int simulateLDR() { return rand() % 2; }               // 0 = bright, 1 = dark
int simulateTemperature() { return 20 + rand() % 21; } // 20–40 °C
int simulatePresence() { return rand() % 2; }          // 0 = none, 1 = detected
