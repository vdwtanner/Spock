#include "../FileUtils.h"

#include <fstream>
#include "Common/Exceptions/FileNotFoundException.h"

namespace Spock::Common
{
    std::vector<char> FileUtils::ReadFile(const std::filesystem::path& filepath) {
        if (!std::filesystem::exists(filepath)) {
            THROW_EXCEPTION(FileNotFoundException, filepath);
        }
        std::ifstream file(filepath.string(), std::ios::ate | std::ios::binary);
        size_t fileSize = file.tellg();
        std::vector<char> data(fileSize);
        file.seekg(0);
        file.read(data.data(), fileSize);
        return data;
    }
}