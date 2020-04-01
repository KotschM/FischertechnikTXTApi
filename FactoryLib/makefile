INCLUDE = -I"..\deps\include" -I"include"

CXX =arm-linux-gnueabihf-g++ $(INCLUDE)
CXXFLAGS = -std=gnu++17 -O3 -Wall -c -fmessage-length=0 -Wno-psabi -shared -Wnarrowing -Wparentheses

$(shell mkdir obj)

all: libutils.so libmqtt.so libtxthighlevelapi.so libtxtlowlevelapi.so

libutils.so: include/utils.h
	$(CXX) $(CXXFLAGS) src/utils.cpp -o obj/$@

libmqtt.so: include/TxtMqttFactoryClient.h
	$(CXX) $(CXXFLAGS) src/TxtMqttFactoryClient.cpp -o obj/$@

libtxthighlevelapi.so: include/TXT_highlevel_API.h
	$(CXX) $(CXXFLAGS) src/TXT_highlevel_API.cpp -o obj/$@

libtxtlowlevelapi.so: include/TXT_lowlevel_API.h
	$(CXX) $(CXXFLAGS) src/TXT_lowlevel_API.cpp -o obj/$@