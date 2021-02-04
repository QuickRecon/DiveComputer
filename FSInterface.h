//
// Created by aren on 4/2/21.
//

#ifndef DIVECOMPUTER_FSINTERFACE_H
#define DIVECOMPUTER_FSINTERFACE_H

#include "LittleFS.h"

#define DIVE_LOG_PATH "/Dives/"

bool InitFS(FSInfo &fsInfo);

#endif //DIVECOMPUTER_FSINTERFACE_H
