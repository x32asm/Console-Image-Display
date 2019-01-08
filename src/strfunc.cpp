#include "strfunc.h"

#include <windows.h>
#include <algorithm>
#include <stdio.h>


// multi byte to wide char:
std::wstring s2ws(const std::string & str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
// wide char to multi byte:
std::string ws2s(const std::wstring & wstr) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}
// trim from start (in place)
inline void ltrim(std::string & s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
	}));
}
// trim from end (in place)
inline void rtrim(std::string & s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), s.end());
}
// trim from both ends (in place)
std::string & trim(std::string & s) {
	ltrim(s);
	rtrim(s);
	return s;
}