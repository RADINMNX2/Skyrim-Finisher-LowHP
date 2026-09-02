#include "settings.h"

#include <SimpleIni.h>

namespace {
    Settings::Config g_config;
}

void Settings::Load(const std::filesystem::path& a_iniPath) {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.SetMultiKey(false);
    ini.LoadFile(a_iniPath.string().c_str());

    // Key: G by default. This is the DirectInput scan code printed to the
    // SKSE log once when the plugin loads. Find yours with a key-tester tool.
    g_config.finisherKey = static_cast<std::uint32_t>(
        ini.GetLongValue("General", "FinisherKey", static_cast<long>(0x22)));
    g_config.finisherHP = static_cast<float>(
        ini.GetDoubleValue("General", "FinisherHP", 15.0));

    SKSE::log::info("Settings: finisherKey={:#x}, finisherHP={}", g_config.finisherKey, g_config.finisherHP);
}

const Settings::Config& Settings::Get() {
    return g_config;
}
