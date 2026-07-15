#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <sys/statvfs.h> 

using namespace std;

const string CLEAR_SCREEN = "\033[2J\033[1;1H";
const string BOLD = "\033[1m";
const string GREEN = "\033[32m";
const string CYAN = "\033[36m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

class TelemetryNode {
public:
    double get_cpu_temp() {
        ifstream file("/sys/class/thermal/thermal_zone0/temp");
        double temp = 0;
        if (file.is_open()) {
            file >> temp;
            temp /= 1000.0;
        }
        return temp;
    }

    void get_memory_stats(long& total_mb, long& used_mb) {
        ifstream file("/proc/meminfo");
        string line, key, unit;
        long value;
        long total = 0, available = 0;

        while (getline(file, line)) {
            istringstream iss(line);
            iss >> key >> value >> unit;
            
            if (key == "MemTotal:") total = value;
            else if (key == "MemAvailable:") available = value;

            if (total > 0 && available > 0) break; 
        }

        total_mb = total / 1024;
        long available_mb = available / 1024;
        used_mb = total_mb - available_mb;
    }

    string get_load_avg() {
        ifstream file("/proc/loadavg");
        string load1, load5, load15;
        if (file.is_open()) {
            file >> load1 >> load5 >> load15;
            return load1 + "  " + load5 + "  " + load15;
        }
        return "N/A";
    }

    string get_uptime() {
        ifstream file("/proc/uptime");
        double uptime_sec;
        if (file.is_open()) {
            file >> uptime_sec;
            int hours = uptime_sec / 3600;
            int minutes = (static_cast<int>(uptime_sec) % 3600) / 60;
            return to_string(hours) + "h " + to_string(minutes) + "m";
        }
        return "N/A";
    }

void get_network_io(double& rx_mb, double& tx_mb) {
    std::ifstream file("/proc/net/dev");
    std::string line;
    rx_mb = 0.0; tx_mb = 0.0;
    
    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string iface;
        iss >> iface;

        /*std::cout << "Found interface: " << iface << std::endl; */

        if (iface == "wlan0:") {
            long long rx_bytes, tx_bytes, dummy;
            iss >> rx_bytes;
            for (int i = 0; i < 7; ++i) iss >> dummy;
            iss >> tx_bytes;
            
            rx_mb = static_cast<double>(rx_bytes) / 1024.0; 
            tx_mb = static_cast<double>(tx_bytes) / 1024.0;
            return; 
        }
    }
}

 

    void get_disk_space(double& total_gb, double& used_gb) {
        struct statvfs buffer;
        if (statvfs("/", &buffer) == 0) {
            double total = static_cast<double>(buffer.f_blocks * buffer.f_frsize);
            double free = static_cast<double>(buffer.f_bavail * buffer.f_frsize);
            
            total_gb = total / (1024.0 * 1024.0 * 1024.0);
            used_gb = (total - free) / (1024.0 * 1024.0 * 1024.0);
        }
    }
};

int main() {
    TelemetryNode pi_node;
    long total_ram = 0, used_ram = 0;
    double rx_mb = 0.0, tx_mb = 0.0;
    double disk_total = 1.0, disk_used = 0.0;

    while (true) {
        double temp = pi_node.get_cpu_temp();
        pi_node.get_memory_stats(total_ram, used_ram);
        string load = pi_node.get_load_avg();
        string uptime = pi_node.get_uptime();
        pi_node.get_network_io(rx_mb, tx_mb);
        pi_node.get_disk_space(disk_total, disk_used);
        
        double ram_percent = (static_cast<double>(used_ram) / total_ram) * 100.0;
        double disk_percent = (disk_used / disk_total) * 100.0;

        cout << CLEAR_SCREEN;
        cout << BOLD << CYAN << "=== [ RPi 3B+ Edge Telemetry v2.0 ] ===" << RESET << "\n\n";
        
        cout << BOLD << "Uptime        : " << RESET << uptime << "\n";
        cout << BOLD << "System Load   : " << RESET << load << "\n";
        cout << BOLD << "CPU Core Temp : " << RESET << (temp > 70 ? YELLOW : GREEN) 
             << fixed << setprecision(1) << temp << " °C" << RESET << "\n\n";
        
        cout << BOLD << "Memory Usage  : " << RESET 
             << used_ram << " MB / " << total_ram << " MB " 
             << "(" << static_cast<int>(ram_percent) << "%)\n";
             
        cout << BOLD << "Root Storage  : " << RESET 
             << fixed << setprecision(1) << disk_used << " GB / " << disk_total << " GB "
             << "(" << static_cast<int>(disk_percent) << "%)\n\n";

        cout << BOLD << "Network I/O   : " << RESET 
             << "RX: " << fixed << setprecision(2) << rx_mb << " KB | "
             << "TX: " << fixed << setprecision(2) << tx_mb << " KB\n";
                  
        cout << "\nPolling every 1000ms... (Ctrl+C to exit)\n";
        
        sleep(1);
    }
    return 0;
}

