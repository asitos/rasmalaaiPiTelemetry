CXX = g++
CXXFLAGS = -Os -Wall

BIN_V1 = v1/telemetry-monitor
BIN_V2 = v2/telemetry-monitor

all: $(BIN_V1) $(BIN_V2)

$(BIN_V1): v1/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_V2): v2/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(BIN_V1) $(BIN_V2)
