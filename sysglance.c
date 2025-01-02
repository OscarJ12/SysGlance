#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <time.h>

// ANSI escape codes for color output
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

// Display system information such as hostname, OS version, current time, and uptime
void display_header() {
    struct sysinfo sys_info;
    char buffer[128];
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char hostname[128];

    if (sysinfo(&sys_info) != 0) {
        perror("sysinfo");
        return;
    }

    gethostname(hostname, sizeof(hostname)); // Get the hostname

    FILE *os_file = fopen("/etc/os-release", "r"); // Read OS details from /etc/os-release
    char os_name[128] = "Unknown OS";
    if (os_file) {
        while (fgets(buffer, sizeof(buffer), os_file)) {
            if (strncmp(buffer, "PRETTY_NAME=", 12) == 0) {
                buffer[strcspn(buffer, "\"")] = 0; // Remove trailing quote
                strncpy(os_name, buffer + 13, sizeof(os_name) - 1);
                os_name[sizeof(os_name) - 1] = '\0';
                break;
            }
        }
        fclose(os_file);
    }

    // Print header information
    printf(BLUE "+-----------------------------------+\n" RESET);
    printf(BLUE "| System Monitor                   |\n" RESET);
    printf(BLUE "+-----------------------------------+\n" RESET);
    printf(CYAN "| Hostname: %-20s\n" RESET, hostname);
    printf(CYAN "| OS: %-26s\n" RESET, os_name);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf(CYAN "| Time: %-26s\n" RESET, buffer);
    printf(CYAN "| Uptime: %ld days, %02ld:%02ld:%02ld\n" RESET,
           sys_info.uptime / 86400, (sys_info.uptime / 3600) % 24,
           (sys_info.uptime / 60) % 60, sys_info.uptime % 60);
    printf(BLUE "+-----------------------------------+\n\n" RESET);
}

// Display CPU usage as a percentage and a bar graph
void display_cpu_usage() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("/proc/stat");
        return;
    }

    char line[128];
    fgets(line, sizeof(line), file);

    unsigned long long user, nice, system, idle;
    sscanf(line, "cpu  %llu %llu %llu %llu", &user, &nice, &system, &idle);
    fclose(file);

    unsigned long long total = user + nice + system + idle;
    double usage = 100.0 * (user + nice + system) / total;

    // Print CPU usage
    printf(GREEN "+ CPU Usage -------------------------+\n" RESET);
    printf(GREEN "| Usage: %-24.2f%%\n" RESET, usage);

    // Graphing the CPU usage
    printf(GREEN "| [");
    int bars = usage / 5; // Scale usage to 20 bars
    for (int i = 0; i < 20; ++i) {
        if (i < bars) printf("#");
        else printf(" ");
    }
    printf("]\n" RESET);

    printf(GREEN "+-----------------------------------+\n\n" RESET);
}

// Display memory usage: total, used, and free
void display_memory_usage() {
    struct sysinfo sys_info;

    if (sysinfo(&sys_info) != 0) {
        perror("sysinfo");
        return;
    }

    long total_mem = sys_info.totalram / 1024 / 1024;
    long free_mem = sys_info.freeram / 1024 / 1024;
    long used_mem = total_mem - free_mem;

    // Print memory usage
    printf(YELLOW "+ Memory Usage ----------------------+\n" RESET);
    printf(YELLOW "| Total: %-22ld MB\n" RESET, total_mem);
    printf(YELLOW "| Used: %-23ld MB\n" RESET, used_mem);
    printf(YELLOW "| Free: %-23ld MB\n" RESET, free_mem);
    printf(YELLOW "+-----------------------------------+\n\n" RESET);
}

// Display disk usage using the 'df' command
void display_disk_usage() {
    FILE *file = popen("df -h --output=source,size,used,avail", "r");
    if (!file) {
        perror("df command");
        return;
    }

    char line[128];
    printf(CYAN "+ Disk Usage ------------------------+\n" RESET);
    fgets(line, sizeof(line), file); // Print headers once
    printf(CYAN "| %-20s %-8s %-8s %-8s\n" RESET, "Device", "Size", "Used", "Avail");
    printf(CYAN "|-----------------------------------\n" RESET);

    while (fgets(line, sizeof(line), file)) {
        char device[64], size[16], used[16], avail[16];
        sscanf(line, "%63s %15s %15s %15s", device, size, used, avail);
        printf(CYAN "| %-20s %-8s %-8s %-8s\n" RESET, device, size, used, avail);
    }

    printf(CYAN "+-----------------------------------+\n\n" RESET);
    pclose(file);
}

// Display network activity: received and transmitted bytes per interface
void display_network_activity() {
    FILE *file = fopen("/proc/net/dev", "r");
    if (!file) {
        perror("/proc/net/dev");
        return;
    }

    char line[128];
    printf(YELLOW "+ Network Activity -----------------+\n" RESET);
    fgets(line, sizeof(line), file); // Skip headers
    fgets(line, sizeof(line), file);
    printf(YELLOW "| Interface   RX (bytes) TX (bytes)\n" RESET);
    printf(YELLOW "| -------------------------------\n" RESET);
    while (fgets(line, sizeof(line), file)) {
        char interface[16];
        unsigned long long rx, tx;
        sscanf(line, " %15[^:]: %llu 0 0 0 0 0 0 %llu", interface, &rx, &tx);
        printf(YELLOW "| %-10s %-10llu %-10llu\n" RESET, interface, rx, tx);
    }
    printf(YELLOW "+-----------------------------------+\n\n" RESET);
    fclose(file);
}

// Display user information (currently logged-in user)
void display_user_info() {
    char *user = getenv("USER");
    if (!user) {
        user = "Unknown";
    }

    printf(RED "+ User Info ------------------------+\n" RESET);
    printf(RED "| User: %-26s\n" RESET, user);
    printf(RED "+-----------------------------------+\n\n" RESET);
}

// Main function to continuously display system monitoring data
int main() {
    while (1) {
        system("clear"); // Clear the terminal

        display_header();
        display_cpu_usage();
        display_memory_usage();
        display_disk_usage();
        display_network_activity();
        display_user_info();

        printf("[Ctrl+C to exit]\n");
        sleep(2); // Update every 2 seconds
    }

    return 0;
}

