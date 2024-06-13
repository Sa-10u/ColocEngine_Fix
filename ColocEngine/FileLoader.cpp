#include "FileLoader.h"
#include <filesystem>
#include<sstream>

using namespace std::filesystem;

bool isResourceFile(const char* str ,std::string* ptr)
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

	file_ = "Resource/Shader/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_;	return true; }

	file_ = "Resource/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "../" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = "Resource/Others/" + std::string(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	return false;
}

bool isResourceFile(const wchar_t* str , std::wstring* ptr)
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

	file_ = L"Resource/Shader/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_;	return true; }

	file_ = L"Resource/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"../" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	file_ = L"Resource/Others/" + std::wstring(str);
	if (is_regular_file(file_)) { *ptr = file_; return true; }

	return false;
}

std::string FileNormalization(std::string* str)
{
	path p(*str);
	auto temp = p.extension();;

	return std::string(p.filename().string());
}

std::wstring FileNormalization(std::wstring* str)
{
	path p(*str);
	auto temp = p.extension();;

	return std::wstring(p.filename().wstring());
}

std::string FileType(std::string* str)
{
	path p(*str);
	auto temp = p.extension();

	auto type = std::string(p.extension().string());
	return FileUpperName(&type);
}

std::wstring FileType(std::wstring* str)
{
	path p(*str);
	auto temp = p.extension();

	auto type = std::wstring(p.extension().wstring());
	return FileUpperName(&type);
	
}

std::string FileUpperName(std::string* str)
{
	std::string t = {};
	constexpr unsigned int  size= ('a' - 'A');

	for (auto i = 1u; i < str->size(); i++) {

		char c = str->at(i);

		if (str->at(i) >= 'a'&& str->at(i) <='z')
		{
			c = str->at(i) - size;
		}

		t.push_back(c);
	}

	return t;
}

std::wstring FileUpperName(std::wstring* str)
{
	std::wstring t = {};
	constexpr unsigned int  size = (L'a' - L'A');

	for (auto i = 1u; i < str->size(); i++) {

		wchar_t c = str->at(i);

		if (str->at(i) >= L'a' && str->at(i) <= L'z')
		{
			c = str->at(i) - size;
		}

		t.push_back(c);
	}

	return t;
}

char* FileUpperName(char* str)
{
	return nullptr;
}

wchar_t FileUpperName(wchar_t* str)
{
	return L'\0';
}
