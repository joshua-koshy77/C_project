#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "utils.h"

LogNode *logHead = NULL;

void addLog(const char *applName, const char *action) {
    LogNode *node = malloc(sizeof(LogNode));
    if (!node) return;
    strncpy(node->applianceName, applName, sizeof(node->applianceName)-1);
    strncpy(node->action, action, sizeof(node->action)-1);
    currentTimestamp(node->timestamp, sizeof(node->timestamp));
    node->next = logHead;
    logHead = node;
}

void showLastNEvents(int N) {
    if (!logHead) {
        printf("\nNo events logged yet.\n");
        return;
    }
    LogNode *cur = logHead;
    int count = 0;
    printf("\n--- Last %d Events ---\n", N);
    while (cur && count < N) {
        printf("[%d] %s %s at %s\n", count + 1, cur->applianceName, cur->action, cur->timestamp);
        cur = cur->next;
        count++;
    }
}

void freeLogs() {
    LogNode *cur = logHead;
    while (cur) {
        LogNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    logHead = NULL;
}
