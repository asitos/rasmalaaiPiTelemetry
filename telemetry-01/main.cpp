#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

// Function to read the Pi's CPU temperature from the system file
double get_cpu_temperature() {
    // This is a virtual file where the Linux kernel dumps real-time thermal data
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
    if (!tempFile.is_open()) {
        return -1.0;
    }
    
    double raw_temp;
    tempFile >> raw_temp;
    tempFile.close();
    
    // The kernel stores it as milli-Celsius (e.g., 45000), so divide by 1000
    return raw_temp / 1000.0;
}

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "  rasmalaaiPi Telemetry Monitoring Node  " << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // Run an infinite loop to monitor the system in real-time
    while (true) {
        double current_temp = get_cpu_temperature();
        
        // Clear the current line and print updated data
        std::cout << "\r[SYSTEM STATS] CPU Temp: " << current_temp << "°C " << std::flush;
        
        // Sleep for 1 second before reading the file again
        sleep(1);
    }
    
    return 0;
}
