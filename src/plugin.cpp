#include "Finisher.h"
#include "log.h"
#include "settings.h"

namespace {
    void OnDataLoaded() {
        Settings::Load(SKSE::GetPluginDirectory() / "FinisherLowHP" / "FinisherLowHP.ini");
        Finisher::Install();
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            SKSE::log::info("Data loaded");
            OnDataLoaded();
            break;
        default:
            break;
        }
    }
} // namespace

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SetupLog();

    SKSE::log::info("FinisherLowHP plugin loaded");

    const auto* plugin = SKSE::PluginDeclaration::GetSingleton();
    SKSE::log::info("Plugin name: {}", plugin->GetName());
    SKSE::log::info("Supported game version: {}", plugin->GetVersion());

    const auto* messaging = SKSE::GetMessagingInterface();
    if (!messaging || !messaging->RegisterListener("SKSE", MessageHandler)) {
        SKSE::log::error("Failed to register SKSE message listener");
        return false;
    }

    return true;
}
