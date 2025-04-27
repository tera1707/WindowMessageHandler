#pragma once
#include <string>
#include <filesystem>
#include <mutex>

class LogOnDesktop
{
private:
    std::mutex mtx;
    std::filesystem::path AppendLogFilePathOnDesktopString(std::wstring filename);
    std::wstring GetTimeString();

public:
    LogOnDesktop();
    void WriteLine(std::wstring txt);
};