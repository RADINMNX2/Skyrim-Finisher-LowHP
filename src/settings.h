#pragma once

namespace Settings {
    struct Config {
        std::uint32_t finisherKey  = 0x22;  // G (DX scancode 34)
        float         finisherHP   = 15.0f; // execute when target health is at or below this
    };

    void Load(const std::filesystem::path& a_iniPath);
    const Config& Get();
} // namespace Settings
