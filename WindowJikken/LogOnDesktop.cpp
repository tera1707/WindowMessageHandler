#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <shlobj.h>
#include <time.h>
#include <thread>   // std::this_thread::get_id()���g���̂ɕK�v
#include <fstream>  // std::wofstream���g���̂ɕK�v
#include <filesystem>
#include <mutex>
#include <locale.h>

#include "LogOnDesktop.h"

// �����̃R�[�h���u���E�U����R�s�[����VisualStudio�ɓ\��t����ƁA�r���h�����Ƃ���
// �@�����R�[�h�]�X�ŃG���[�ɂȂ������Ƃ��������B���������Ƃ��́A�R�[�h���̃R�����g��S�������ƃG���[�������B

LogOnDesktop::LogOnDesktop()
{
    _wsetlocale(LC_ALL, L"japanese");
}

std::filesystem::path LogOnDesktop::AppendLogFilePathOnDesktopString(std::wstring filename)
{
    WCHAR* buf = nullptr;
    std::filesystem::path desktop;

    auto hr = SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &buf);

    if (SUCCEEDED(hr)) {
        desktop = buf;
        desktop.append(filename);
    }

    CoTaskMemFree(buf);

    return desktop;
}

std::wstring LogOnDesktop::GetTimeString()
{
    WCHAR buf[64];
    auto t = time(nullptr);
    auto tmv = tm();
    auto error = localtime_s(&tmv, &t); // ���[�J������(�^�C���]�[���ɍ��킹������)���擾

    wcsftime(buf, 64, L"%Y/%m/%d %H:%M:%S", &tmv);
    auto aika = std::wstring(buf);

    return aika;
}

void LogOnDesktop::WriteLine(std::wstring txt)
{
    std::lock_guard<std::mutex> lock(mtx);

    auto desktop = AppendLogFilePathOnDesktopString(L"mylog.log");

    DWORD processId = GetCurrentProcessId();

    auto aika = GetTimeString();

    // ���݂̃X���b�hID���o��
    auto thId = std::this_thread::get_id();

    // �t�@�C�����J��(�Ȃ���΍쐬)
    std::wofstream ofs;
    ofs.imbue(std::locale("Japanese", LC_ALL));
    ofs.open(desktop, std::ios::app);

    if (!ofs)
        return;

    // ���ݎ����ƃX���b�hID��t�������O���t�@�C���ɏ�������
    ofs << aika << L" " << processId << L" " << thId << L" " << txt << std::endl;
    ofs.flush();

    // �t�@�C������
    ofs.close();
}