//
// Created by ThisEwan on 2021/10/27.
//

#ifndef EXPORTLIB_LOG_TOOL_H
#define EXPORTLIB_LOG_TOOL_H

#endif //EXPORTLIB_LOG_TOOL_H

#include <android/log.h>

#define TAG "Ewan"

#define LOGD(...)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)