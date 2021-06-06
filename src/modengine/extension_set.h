#pragma once

#include <modengine/extension.h>

#include <map>
#include <memory>
#include <vector>

namespace modengine {

class ExtensionSet {
public:
    ExtensionSet(ModEngineExtensionConnector *connector) : m_connector(connector) {}

    void load_extensions(std::vector<fs::path> dlls, bool enumerate_modules);

    void attach_all(Settings& settings);
    void attach(const std::string& id);

    void detach_all();
    void detach(const std::string& id);

    void reload(const std::string& id);

    template <typename T>
    void register_builtin_extension() requires std::is_base_of_v<ModEngineExtension, T>
    {
        ModEngineExtension* ext = new T(m_connector);
        m_extensions[ext->id()] = std::unique_ptr<ModEngineExtension>(ext);
    }
private:
    ModEngineExtensionConnector* m_connector;
    std::map<std::string, std::unique_ptr<ModEngineExtension>> m_extensions;
    std::map<std::string, HMODULE> m_extension_modules;

    bool load_extension(HMODULE module);
};

}
