#include "pine/utils/filesystem.hpp"

#include <fstream>

#include "pine/core/log.hpp"
#include "pine/pch.hpp"

namespace pine {

bool filesystem::create_directory(const std::filesystem::path& directory) {
    return std::filesystem::create_directory(directory);
}

bool filesystem::create_directory(const std::string_view& directory) {
    return std::filesystem::create_directory(directory);
}

bool filesystem::exists(const std::filesystem::path& filepath) {
    return std::filesystem::exists(filepath);
}

bool filesystem::exists(const std::string& filepath) {
    return std::filesystem::exists(std::filesystem::path(filepath));
}

bool filesystem::copy(const std::filesystem::path& old_filepath,
    const std::filesystem::path& new_filepath) {
    std::error_code ec;
    std::filesystem::copy(old_filepath, new_filepath, ec);
    return !ec;
}

bool filesystem::move(const std::filesystem::path& old_filepath,
    const std::filesystem::path& new_filepath) {
    std::error_code ec;
    std::filesystem::rename(old_filepath, new_filepath, ec);
    return !ec;
}

bool filesystem::move_file(const std::filesystem::path& filepath,
    const std::filesystem::path& destination) {
    return move(filepath, destination);
}

bool filesystem::rename_file(const std::filesystem::path& old_filepath,
    const std::string& new_name) {
    auto new_filepath =
        old_filepath.parent_path() / new_name / old_filepath.extension();
    return move(old_filepath, new_filepath);
}

bool filesystem::delete_file(const std::filesystem::path& filepath) {
    return std::filesystem::remove(filepath);
}

bool filesystem::is_empty(const std::filesystem::path& filepath) {
    return std::filesystem::is_empty(filepath);
}

bool filesystem::is_equal(const std::filesystem::path& filepath,
    const std::filesystem::path& other) {
    return std::filesystem::equivalent(filepath, other);
}

bool filesystem::is_file(const std::filesystem::path& filepath) {
    return std::filesystem::is_regular_file(filepath);
}

bool filesystem::is_directory(const std::filesystem::path& filepath) {
    return std::filesystem::is_directory(filepath);
}

bool filesystem::set_working_directory(const std::filesystem::path& filepath) {
    if (filesystem::is_directory(filepath)) {
        std::error_code ec;
        std::filesystem::current_path(filepath, ec);
        return !ec;
    } else {
        return false;
    }
}

std::filesystem::path filesystem::get_working_directory() {
    return std::filesystem::current_path();
}

std::string filesystem::read_file_source(
    const std::filesystem::path& filepath) {
    if (!pine::filesystem::is_file(filepath)) {
        return "";
    }
    std::string source;
    std::ifstream input_stream(filepath, std::ios::in | std::ios::binary);
    if (input_stream) {
        input_stream.seekg(0, std::ios::end);
        source.resize(static_cast<uint32_t>(input_stream.tellg()));
        input_stream.seekg(0, std::ios::beg);
        input_stream.read(&source[0], static_cast<int>(source.size()));
        input_stream.close();
    } else {
        PINE_CORE_ERROR("Could not open file '{0}'", filepath);
    }

    return source;
}

} // namespace pine
