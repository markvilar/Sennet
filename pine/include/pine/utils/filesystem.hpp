#pragma once

#include <filesystem>
#include <functional>
#include <string>

namespace pine::filesystem
{

bool CreateDirectory(const std::filesystem::path& directory);
bool CreateDirectory(const std::string& directory);

bool Exists(const std::filesystem::path& filepath);
bool Exists(const std::string& filepath);

bool Copy(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath);
bool Move(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath);

bool MoveFile(const std::filesystem::path& filepath,
    const std::filesystem::path& destination);
bool RenameFile(const std::filesystem::path& oldFilepath,
    const std::string& newName);
bool DeleteFile(const std::filesystem::path& filepath);

bool IsEmpty(const std::filesystem::path& filepath);
bool IsEqual(const std::filesystem::path& filepath,
    const std::filesystem::path& other);
bool IsFile(const std::filesystem::path& filepath);
bool IsDirectory(const std::filesystem::path& filepath);

bool SetWorkingDirectory(const std::filesystem::path& filepath);
std::filesystem::path GetWorkingDirectory();

} // namespace pine::filesystem
