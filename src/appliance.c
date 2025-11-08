#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "appliance.h"
#include "log.h"
#include "utils.h"

/* --- Initialize appliances --- */
void initAppliances(Appliance appliances[]) {
    const char *names[MAX_APPLIANCES] = {"Light", "Fan", "Fridge", "AC", "TV", "WashingMachine"};
    const float power[MAX_APPLIANCES] = {60.0, 75.0, 120.0, 1000.0, 150.0, 500.0};

    time_t now = time(NULL);

    for (int i = 0; i < MAX_APPLIANCES; i++) {
        strcpy(appliances[i].name, names[i]);
        appliances[i].powerRating = power[i];
        appliances[i].isOn = (i == 2) ? 1 : 0;  // Fridge always ON
        appliances[i].totalEnergyWh = 0.0;
        appliances[i].totalOnMinutes = 0;
        appliances[i].onStartTime = appliances[i].isOn ? now : 0;
        appliances[i].lastUpdateTime = appliances[i].isOn ? now : 0;

        if (appliances[i].isOn)
            currentTimestamp(appliances[i].lastActive, LOG_TIMESTAMP_LEN);
        else
            strcpy(appliances[i].lastActive, "Never");
    }
}

/* --- Set ON/OFF state --- */
void setApplianceState(Appliance appliances[], int idx, int newState) {
    if (idx < 0 || idx >= MAX_APPLIANCES) return;

    Appliance *a = &appliances[idx];
    time_t now = time(NULL);

    // Turning ON
    if (!a->isOn && newState) {
        a->isOn = 1;
        a->onStartTime = now;
        a->lastUpdateTime = now;
        currentTimestamp(a->lastActive, LOG_TIMESTAMP_LEN);
        addLog(a->name, "ON");
    }
    // Turning OFF
    else if (a->isOn && !newState) {
        // Update running time before turning off
        updateRunningTime(appliances);

        a->isOn = 0;
        a->onStartTime = 0;
        a->lastUpdateTime = 0;
        currentTimestamp(a->lastActive, LOG_TIMESTAMP_LEN);
        addLog(a->name, "OFF");
    }
}

/* --- Update running time and energy --- */
void updateRunningTime(Appliance appliances[]) {
    time_t now = time(NULL);

    for (int i = 0; i < MAX_APPLIANCES; i++) {
        Appliance *a = &appliances[i];

        if (a->isOn && a->onStartTime > 0) {
            double minutesElapsed = difftime(now, a->lastUpdateTime) / 60.0;
            if (minutesElapsed >= 1.0) {  // update only if >= 1 minute
                a->totalOnMinutes += (int)minutesElapsed;
                a->totalEnergyWh += a->powerRating * (minutesElapsed / 60.0);
                a->lastUpdateTime = now;
                currentTimestamp(a->lastActive, LOG_TIMESTAMP_LEN);
            }
        }
    }
}

/* --- Display appliance status --- */
void displayApplianceStatus(Appliance appliances[]) {
    updateRunningTime(appliances);  // ensure live ON-time
    printf("\n--- Appliance Status ---\n");
    for (int i = 0; i < MAX_APPLIANCES; ++i) {
        printf("%d) %-15s | State: %-3s | Power: %6.1f W | Energy: %8.3f Wh | Total ON: %3d min | Last Active: %s\n",
               i + 1,
               appliances[i].name,
               appliances[i].isOn ? "ON" : "OFF",
               appliances[i].powerRating,
               appliances[i].totalEnergyWh,
               appliances[i].totalOnMinutes,
               appliances[i].lastActive);
    }
}

/* --- Total energy consumed --- */
double totalEnergyConsumedWh(Appliance appliances[]) {
    updateRunningTime(appliances);
    double sum = 0.0;
    for (int i = 0; i < MAX_APPLIANCES; ++i)
        sum += appliances[i].totalEnergyWh;
    return sum;
}

/* --- Manual toggle --- */
void manualToggleAppliance(Appliance appliances[]) {
    displayApplianceStatus(appliances);

    int idx, state;
    printf("Enter appliance number to toggle (1-%d): ", MAX_APPLIANCES);
    if (scanf("%d", &idx) != 1) return;
    idx--;
    if (idx < 0 || idx >= MAX_APPLIANCES) return;

    printf("Enter new state (1 = ON, 0 = OFF): ");
    if (scanf("%d", &state) != 1) return;

    setApplianceState(appliances, idx, state ? 1 : 0);
    printf("%s set to %s.\n", appliances[idx].name, appliances[idx].isOn ? "ON" : "OFF");
}


