#ifndef HDEVICE_H
#define HDEVICE_H

#include <string>
#include <vector>
using std::vector;

struct HDevice{
    char name[256];
    //...
};

#include <cstddef>
using std::size_t;
const size_t HDEVICE_MAX_DEVICE_NAME_LEN = sizeof((new struct HDevice)->name);

vector<HDevice> getVideoDevices();
vector<HDevice> getAudioDevices();

#endif // HDEVICE_H
