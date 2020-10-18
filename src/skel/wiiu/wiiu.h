#ifndef _WIIU_H_
#define _WIIU_H_

#include "common.h"

typedef struct
{
    void* window;
    RwBool		fullScreen;
    RwV2d		lastMousePos;
    double      mouseWheel;
    bool        cursorIsInWindow;
    RwInt8        joy1id;
    RwInt8        joy2id;
}
psGlobalType;

#define PSGLOBAL(var) (((psGlobalType *)(RsGlobal.ps))->var)

void CapturePad(RwInt32 padID);

#endif