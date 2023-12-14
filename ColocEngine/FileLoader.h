#pragma once
#include<cstdint>
#include <string>

bool FileLoad(const char* str ,std::string* ptr);
bool FileLoad(const wchar_t* str , std::wstring* ptr);

std::string FileNormalization(std::string* str);
std::wstring FileNormalization(std::wstring* str);

std::string FileType(std::string* str);
std::wstring FileType(std::wstring* str);

std::string FileUpperName(std::string* str);
std::wstring FileUpperName(std::wstring* str);

char* FileUpperName(char* str);
wchar_t FileUpperName(wchar_t* str);