#include "crash_handler_extension.h"
#include "modengine/version.h"

#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>
#include <memory>

namespace modengine::ext {

void CrashHandlerExtension::on_attach()
{
    // patch SetUnhandledExceptionFilter to nop chain
    register_patch(DS3, 0x141fe49c7, replace_with<unsigned char>({ 0x90, 0x90, 0x90, 0x90, 0x90 }));

    // change panic mode to RAISE_EXCEPTION_ON_PANIC
    register_patch(DS3, 0x1446418c8, replace_with<uint32_t>({ 0x02 }));

    const auto data_path = settings().modengine_data_path();
    const auto install_path = settings().modengine_install_path();
    const auto crashpad_path = install_path / "crashpad";
    const auto crashdump_path = data_path / "crashdumps";

    info(L"Saving crash dumps to {}", crashdump_path.native());

    if (!fs::create_directories(crashdump_path)) {
        warn(L"Couldn't create crash dump folder, crash dumps may not be saved to disk");
    }

    base::FilePath db_path(crashdump_path);
    base::FilePath handler_path(crashpad_path / "crashpad_handler.exe");
    base::FilePath metrics_path(crashpad_path / "crash_metrics");

    const auto database = crashpad::CrashReportDatabase::Initialize(db_path);

    if (database == nullptr || database->GetSettings() == NULL) {
        return;
    }

    database->GetSettings()->SetUploadsEnabled(true);

    info("Setting up crash handler");

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
        warn("Failed to set up crash handler");
    }
}

void CrashHandlerExtension::on_detach()
{
}

}
