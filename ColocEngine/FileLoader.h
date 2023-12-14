#pragma once
#include<cstdint>
#include <string>

bool FileLoad(const char* str ,std::string* ptr);
bool FileLoad(const wchar_t* str , std::wstring* ptr);
std::string FileNormalization(std::string* str);
std::wstring FileNormalization(std::wstring* str);