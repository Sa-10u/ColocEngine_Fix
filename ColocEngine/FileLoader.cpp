#include "FileLoader.h"
#include <filesystem>


using namespace std::filesystem;

bool FileLoad(const char* str ,std::string* ptr)
{
	if (str == nullptr)	return false;

	std::string file_;
	file_ = str;

	if(is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "Resource/3dModel/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_;	return true; }

	file_ = "Resource/Sounds/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "Resource/Texture/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "Resource/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "../" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	return false;
}

bool FileLoad(const wchar_t* str , std::wstring* ptr)
{
	if (str == nullptr)	return false;

	std::wstring file_;
	file_ = str;

	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"Resource/3dModel/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"Resource/Sounds/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"Resource/Texture/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"Resource/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"../" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	return false;
}
