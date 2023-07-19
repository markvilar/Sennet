#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <string_view>

namespace pine::filesystem {

bool create_directory(const std::filesystem::path& directory);
bool create_directory(const std::string_view& directory);

bool exists(const std::filesystem::path& filepath);
bool exists(const std::string& filepath);

bool copy(const std::filesystem::path& old_filepath,
    const std::filesystem::path& new_filepath);
bool move(const std::filesystem::path& old_filepath,
    const std::filesystem::path& new_filepath);

bool move_file(const std::filesystem::path& filepath,
    const std::filesystem::path& destination);
bool rename_file(const std::filesystem::path& old_filepath,
    const std::string& new_name);
bool delete_file(const std::filesystem::path& filepath);

bool is_empty(const std::filesystem::path& filepath);
bool is_equal(const std::filesystem::path& filepath,
    const std::filesystem::path& other);
bool is_file(const std::filesystem::path& filepath);
bool is_directory(const std::filesystem::path& filepath);

bool set_working_directory(const std::filesystem::path& filepath);
std::filesystem::path get_working_directory();

std::string read_file_source(const std::filesystem::path& filepath);

} // namespace pine::filesystem
