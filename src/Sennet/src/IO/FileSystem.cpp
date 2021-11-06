#include "Sennet/IO/FileSystem.hpp"
#include "Sennet/Pch.hpp"

namespace Sennet
{

bool FileSystem::CreateDirectory(const std::filesystem::path& directory)
{
    return std::filesystem::create_directory(directory);
}

bool FileSystem::CreateDirectory(const std::string& directory)
{
    return CreateDirectory(std::filesystem::path(directory));
}

bool FileSystem::Exists(const std::filesystem::path& filepath)
{
    return std::filesystem::exists(filepath);
}

bool FileSystem::Exists(const std::string& filepath)
{
    return std::filesystem::exists(std::filesystem::path(filepath));
}

bool FileSystem::Copy(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath)
{
    std::error_code ec;
    std::filesystem::copy(oldFilepath, newFilepath, ec);
    return !ec;
}

bool FileSystem::Move(const std::filesystem::path& oldFilepath,
    const std::filesystem::path& newFilepath)
{
    std::error_code ec;
    std::filesystem::rename(oldFilepath, newFilepath, ec);
    return !ec;
}

bool FileSystem::MoveFile(const std::filesystem::path& filepath,
    const std::filesystem::path& destination)
{
    return Move(filepath, destination);
}

bool FileSystem::RenameFile(
    const std::filesystem::path& oldFilepath, const std::string& newName)
{
    auto newFilepath =
        oldFilepath.parent_path() / newName / oldFilepath.extension();
    return Move(oldFilepath, newFilepath);
}

bool FileSystem::DeleteFile(const std::filesystem::path& filepath)
{
    return std::filesystem::remove(filepath);
}

bool FileSystem::IsEmpty(const std::filesystem::path& filepath)
{
    return std::filesystem::is_empty(filepath);
}

bool FileSystem::IsEqual(
    const std::filesystem::path& filepath, const std::filesystem::path& other)
{
    return std::filesystem::equivalent(filepath, other);
}

bool FileSystem::IsFile(const std::filesystem::path& filepath)
{
    return std::filesystem::is_regular_file(filepath);
}

bool FileSystem::IsDirectory(const std::filesystem::path& filepath)
{
    return std::filesystem::is_directory(filepath);
}

bool FileSystem::SetWorkingDirectory(const std::filesystem::path& filepath)
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

std::filesystem::path FileSystem::GetWorkingDirectory()
{
    return std::filesystem::current_path();
}

} // namespace Sennet
