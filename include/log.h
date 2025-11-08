#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
typedef struct LogNode {
    char applianceName[20];
    char action[8];
    char timestamp[32];
    struct LogNode *next;
} LogNode;

extern LogNode *logHead;

void addLog(const char *applName, const char *action);
void showLastNEvents(int N);
void freeLogs();


#endif // LOG_H_INCLUDED
