#ifndef HMEDIA_H
#define HMEDIA_H

#include <string>

enum MEDIA_TYPE{
    MEDIA_TYPE_NETWORK = 0,
	MEDIA_TYPE_FILE,
    MEDIA_TYPE_CAPTURE,
    MEDIA_TYPE_NONE
};

#define DEFAULT_MEDIA_TYPE  MEDIA_TYPE_NETWORK
const char DEFAULT_MEDIA_SRC[]="rtsp://192.168.1.10/user=admin&password=&channel=1&stream=0.sdp?real_stream";

typedef struct media_s{
    MEDIA_TYPE  type;
    int         index; // for camera index
    std::string src;
    std::string descr;

    media_s(){
        type = DEFAULT_MEDIA_TYPE;
        index = 0;
        src.assign(DEFAULT_MEDIA_SRC);
    }
}HMedia;

#endif // HMEDIA_H
