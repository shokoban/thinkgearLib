#pragma once

#include "./core/common.h"
#include "./utils/utils.h"

#ifdef _WIN32
#include "./core/win/thinkgear_win.h"
#else
#include "./core/linux/thinkgear_linux.h"
#endif
