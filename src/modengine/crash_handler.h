#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#ifndef MODENGINE_CRASH_REPORT_URL
#define MODENGINE_CRASH_REPORT_URL "http://localhost:3000/"
#endif

namespace modengine {

void start_crash_report_uploads();
void start_crash_handler(const fs::path &install_root, const fs::path &data_root);

}
