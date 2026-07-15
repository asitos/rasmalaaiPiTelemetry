CXX = g++
CXXFLAGS = -Os -Wall

all: telemetry-01/telemetry_monitor telemetry-02/updatedTelemetry

telemetry-01/telemetry_monitor: telemetry-01/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

telemetry-02/updatedTelemetry: telemetry-02/telemetry-02.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f telemetry-01/telemetry_monitor telemetry-02/updatedTelemetry
