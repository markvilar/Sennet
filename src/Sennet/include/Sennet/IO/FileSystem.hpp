#pragma once

#include <filesystem>
#include <functional>
#include <string>

namespace Sennet
{

class FileSystem
{
public:
    enum class Action
    {
        Add,
        Rename,
        Modify,
        Delete
    };

    struct Event
    {
        Action Act;
        std::filesystem::path Filepath;
        bool IsDirectory;

        std::string OldName = "";
    };

public:
    static bool CreateDirectory(const std::filesystem::path& directory);
    static bool CreateDirectory(const std::string& directory);

    static bool Exists(const std::filesystem::path& filepath);
    static bool Exists(const std::string& filepath);

    static bool Copy(const std::filesystem::path& oldFilepath,
        const std::filesystem::path& newFilepath);
    static bool Move(const std::filesystem::path& oldFilepath,
        const std::filesystem::path& newFilepath);

    static bool MoveFile(const std::filesystem::path& filepath,
        const std::filesystem::path& destination);
    static bool RenameFile(const std::filesystem::path& oldFilepath,
        const std::string& newName);
    static bool DeleteFile(const std::filesystem::path& filepath);

    static bool IsEmpty(const std::filesystem::path& filepath);
    static bool IsEqual(const std::filesystem::path& filepath,
        const std::filesystem::path& other);
    static bool IsFile(const std::filesystem::path& filepath);
    static bool IsDirectory(const std::filesystem::path& filepath);

    static bool SetWorkingDirectory(const std::filesystem::path& filepath);
    static std::filesystem::path GetWorkingDirectory();

public:
    using EventCallbackFunc = std::function<void(const std::vector<Event>&)>;

    static void SetEventCallback(const EventCallbackFunc& callback);

private:
    static EventCallbackFunc s_Callback;
};

} // namespace Sennet
