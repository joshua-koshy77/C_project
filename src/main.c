#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "appliance.h"
#include "log.h"
#include "utils.h"

#define TEMP_THRESHOLD 30

/* --- Total simulated minutes (for logging) --- */
int totalSimulatedMinutes = 0;

/* --- Automation Logic --- */
void applyAutomation(Appliance appliances[], int presence, int ldrDark, int temp) {
    // Light control
    if (presence && ldrDark)
        setApplianceState(appliances, 0, 1);
    else
        setApplianceState(appliances, 0, 0);

    // Fan control
    if (temp > TEMP_THRESHOLD)
        setApplianceState(appliances, 1, 1);
    else
        setApplianceState(appliances, 1, 0);

    // Fridge always ON
    setApplianceState(appliances, 2, 1);

    // AC control
    if (temp > 35)
        setApplianceState(appliances, 3, 1);
    else
        setApplianceState(appliances, 3, 0);

    // Update running time for all ON appliances
    updateRunningTime(appliances);
}

/* --- Menu --- */
void printMenu() {
    printf("\n=== Smart Energy Monitoring & Control System ===\n");
    printf("1 - Random Sensor Simulation (1 minute)\n");
    printf("2 - Manual Sensor Input (1 minute)\n");
    printf("3 - Show Appliance Status\n");
    printf("4 - Show Total Energy Consumption\n");
    printf("5 - Show Last N Events\n");
    printf("6 - Toggle Appliance Manually (TV / Washing Machine)\n");
    printf("7 - Reset Event Log\n");
    printf("Q - Quit\n");
    printf("Enter choice: ");
}

int main() {
    srand(time(NULL));
    Appliance appliances[MAX_APPLIANCES];
    initAppliances(appliances);

    char choice;
    while (1) {
        printMenu();
        scanf(" %c", &choice);

        if (choice == 'Q' || choice == 'q') {
            printf("\nExiting Simulation...\n");
            updateRunningTime(appliances); // final update for live ON time
            displayApplianceStatus(appliances);
            printf("Total simulated time: %d minutes\n", totalSimulatedMinutes);
            printf("Total energy consumed: %.3f Wh (%.3f kWh)\n",
                   totalEnergyConsumedWh(appliances),
                   totalEnergyConsumedWh(appliances) / 1000.0);
            freeLogs();
            break;
        }

        switch (choice) {
            case '1': {
                int presence = simulatePresence();
                int ldr = simulateLDR();
                int temp = simulateTemperature();

                printf("\n[Simulated Sensors] Presence: %s | LDR: %s | Temp: %d°C\n",
                       presence ? "Detected" : "Not Detected",
                       ldr ? "Dark" : "Bright",
                       temp);

                applyAutomation(appliances, presence, ldr, temp);
                totalSimulatedMinutes += 1;
                printf("Simulated 1 minute elapsed.\n");
                break;
            }

            case '2': {
                int presence, ldr, temp;
                printf("Enter presence (0=No, 1=Yes): ");
                scanf("%d", &presence);
                printf("Enter LDR (0=Bright, 1=Dark): ");
                scanf("%d", &ldr);
                printf("Enter Temperature (°C): ");
                scanf("%d", &temp);

                applyAutomation(appliances, presence, ldr, temp);
                totalSimulatedMinutes += 1;
                printf("Simulated 1 minute elapsed.\n");
                break;
            }

            case '3':
                updateRunningTime(appliances); // update live ON time
                displayApplianceStatus(appliances);
                break;

            case '4':
                updateRunningTime(appliances); // ensure ON time is updated
                printf("Total energy consumed = %.3f Wh (%.3f kWh)\n",
                       totalEnergyConsumedWh(appliances),
                       totalEnergyConsumedWh(appliances) / 1000.0);
                break;

            case '5': {
                int N;
                printf("Enter N (number of recent events): ");
                scanf("%d", &N);
                showLastNEvents(N);
                break;
            }

            case '6':
                manualToggleAppliance(appliances); // live timer update inside
                break;

            case '7':
                freeLogs();
                printf("Event log cleared.\n");
                break;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}

