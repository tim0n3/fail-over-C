#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256
#define MAX_OUTPUT_LEN 256

const char* PRIMARY_IFACE = "eth0";
const char* SECONDARY_IFACE = "wlan0";

bool check_interface_state(const char* iface) {
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, sizeof(cmd), "ip link show %s | grep \"state UP\"", iface);

    FILE* fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return false;
    }

    char output[MAX_OUTPUT_LEN];
    if (fgets(output, sizeof(output) - 1, fp) != NULL) {
        pclose(fp);
        return true;
    }

    pclose(fp);
    return false;
}

bool check_internet_connectivity() {
    FILE* fp = popen("ping -c 1 google.com", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return false;
    }

    char output[MAX_OUTPUT_LEN];
    while (fgets(output, sizeof(output) - 1, fp) != NULL) {
        if (strstr(output, "1 packets transmitted, 1 received")) {
            pclose(fp);
            return true;
        }
    }

    pclose(fp);
    return false;
}

void set_interface_state(const char* iface, bool state) {
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, sizeof(cmd), "ip link set %s %s", iface, state ? "up" : "down");

    system(cmd);
}

int main() {
    while (true) {
        if (check_interface_state(PRIMARY_IFACE)) {
            printf("Primary interface is up\n");
            if (check_internet_connectivity()) {
                printf("Internet connectivity is good\n");
                sleep(10);
            } else {
                printf("Primary interface is down, failing over to secondary interface\n");
                set_interface_state(PRIMARY_IFACE, false);
                set_interface_state(SECONDARY_IFACE, true);
            }
        } else {
            printf("Primary interface is down\n");
            if (check_interface_state(SECONDARY_IFACE)) {
                printf("Secondary interface is up\n");
                if (check_internet_connectivity()) {
                    printf("Internet connectivity is good\n");
                    sleep(10);
                } else {
                    printf("Secondary interface is down, failing back to primary interface\n");
                    set_interface_state(SECONDARY_IFACE, false);
                    set_interface_state(PRIMARY_IFACE, true);
                }
            } else {
                printf("Secondary interface is down\n");
                sleep(10);
            }
        }
    }
    return 0;
}
