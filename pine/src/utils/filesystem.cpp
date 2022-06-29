#include "pine/utils/filesystem.hpp"

#include "pine/pch.hpp"

namespace pine
{

bool filesystem::CreateDirectory(const std::filesystem::path& directory)
{
    return std::filesystem::create_directory(directory);
}

bool filesystem::CreateDirectory(const std::string& directory)
{
    return CreateDirectory(std::filesystem::path(directory));
}

bool filesystem::Exists(const std::filesystem::path& filepath)
{
    return std::filesystem::exists(filepath);
}

bool filesystem::Exists(const std::string& filepath)
{
    return std::filesystem::exists(std::filesystem::path(filepath));
}

bool filesystem::Copy(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath)
{
    std::error_code ec;
    std::filesystem::copy(oldFilepath, newFilepath, ec);
    return !ec;
}

bool filesystem::Move(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath)
{
    std::error_code ec;
    std::filesystem::rename(oldFilepath, newFilepath, ec);
    return !ec;
}

bool filesystem::MoveFile(const std::filesystem::path& filepath,
    const std::filesystem::path& destination)
{
    return Move(filepath, destination);
}

bool filesystem::RenameFile(const std::filesystem::path& oldFilepath,
    const std::string& newName)
{
    auto newFilepath =
        oldFilepath.parent_path() / newName / oldFilepath.extension();
    return Move(oldFilepath, newFilepath);
}

bool filesystem::DeleteFile(const std::filesystem::path& filepath)
{
    return std::filesystem::remove(filepath);
}

bool filesystem::IsEmpty(const std::filesystem::path& filepath)
{
    return std::filesystem::is_empty(filepath);
}

bool filesystem::IsEqual(const std::filesystem::path& filepath,
    const std::filesystem::path& other)
{
    return std::filesystem::equivalent(filepath, other);
}

bool filesystem::IsFile(const std::filesystem::path& filepath)
{
    return std::filesystem::is_regular_file(filepath);
}

bool filesystem::IsDirectory(const std::filesystem::path& filepath)
{
    return std::filesystem::is_directory(filepath);
}

bool filesystem::SetWorkingDirectory(const std::filesystem::path& filepath)
{
    if (IsDirectory(filepath))
    {
        std::error_code ec;
        std::filesystem::current_path(filepath, ec);
        return !ec;
    }
    else
    {
        return false;
    }
}

std::filesystem::path filesystem::GetWorkingDirectory()
{
    return std::filesystem::current_path();
}

} // namespace pine
