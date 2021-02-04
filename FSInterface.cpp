//
// Created by aren on 4/2/21.
//
#include "FSInterface.h"

bool InitFS(FSInfo &fsInfo)
{
    bool status = LittleFS.begin();

    if(!LittleFS.exists(DIVE_LOG_PATH))
    {
        LittleFS.mkdir(DIVE_LOG_PATH);
    }
    LittleFS.info(fsInfo);
    return status;
}