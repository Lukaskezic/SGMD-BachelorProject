#include <string>

#pragma once

class RTCModule
{
public:
    RTCModule();
    void setupRTCModule();
	void receiveRTCModuleData();
    void saveRTCModuleData();
    std::string getFileName();
private:
    std::string filename;
};