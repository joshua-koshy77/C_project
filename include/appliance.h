#ifndef APPLIANCE_H_INCLUDED
#define APPLIANCE_H_INCLUDED

#include <time.h>

#define MAX_APPLIANCES 6
#define LOG_TIMESTAMP_LEN 32

typedef struct {
    char name[20];            // Appliance name
    int isOn;                 // ON/OFF state
    float powerRating;        // Power in Watts
    double totalEnergyWh;     // Total energy consumed in Wh
    int totalOnMinutes;       // Total ON time in minutes
    time_t onStartTime;       // Timestamp when last turned ON
    time_t lastUpdateTime;    // Last time running-time was updated
    char lastActive[LOG_TIMESTAMP_LEN]; // Last active timestamp as string
} Appliance;

// Initialize all appliances
void initAppliances(Appliance appliances[]);

// Set appliance ON/OFF state
void setApplianceState(Appliance appliances[], int idx, int newState);

// Update running time and energy for all ON appliances
void updateRunningTime(Appliance appliances[]);

// Display the status of all appliances
void displayApplianceStatus(Appliance appliances[]);

// Calculate total energy consumed by all appliances
double totalEnergyConsumedWh(Appliance appliances[]);

// Manual toggling of appliances
void manualToggleAppliance(Appliance appliances[]);

#endif // APPLIANCE_H_INCLUDED

