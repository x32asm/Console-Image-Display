#ifndef GUARD_STRFUNC_H
#define GUARD_STRFUNC_H
#pragma once

#include <string>


std::wstring s2ws(const std::string & str);
std::string ws2s(const std::wstring & wstr);

std::string & trim(std::string & s);


#endif // GUARD_STRFUNC_H