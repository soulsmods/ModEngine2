#include "crash_handler.h"
#include "modengine/version.h"

#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>
#include <memory>

#include <spdlog/spdlog.h>

using namespace spdlog;

namespace modengine {

std::unique_ptr<crashpad::CrashReportDatabase> database;

void start_crash_report_uploads()
{
    database->GetSettings()->SetUploadsEnabled(true);
}

void start_crash_handler(const fs::path &install_root, const fs::path &data_root)
{
    const auto crashpad_path = install_root / "crashpad";
    const auto crashdump_path = data_root / "crashdumps";

    base::FilePath db_path(crashdump_path);
    base::FilePath handler_path(crashpad_path / "crashpad_handler.exe");
    base::FilePath metrics_path(crashpad_path / "crash_metrics");

    database = crashpad::CrashReportDatabase::Initialize(db_path);

    if (database == nullptr || database->GetSettings() == NULL) {
        return;
    }

    auto crashpad = crashpad::CrashpadClient();
    if (!crashpad.StartHandler(
            handler_path,
            db_path,
            metrics_path,
            MODENGINE_CRASH_REPORT_URL,
            { { "version", modengine::g_version } },
            {},
            false,
            false)) {
    }
}

}
