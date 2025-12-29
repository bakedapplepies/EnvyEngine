#include "Path.h"

#include <filesystem>
#include <fmt/format.h>

Path::Path(std::string_view path)
{
    m_path = path;
}

std::string Path::Str() const
{
    return fmt::format("{}",
        (std::filesystem::path(CLIENT_ROOT_DIR) / std::filesystem::path(m_path)).string());
}
