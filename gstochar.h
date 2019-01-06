#ifndef GUARD_GSTOCHAR_H
#define GUARD_GSTOCHAR_H
#pragma once

namespace gtc {
	const int nRampLen = 70;
	const float fRampConversion = 3.64285714f;
	const char chGSRamp[] = {
		' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I',
		'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?',
		']', '[', '}', '{', '1', ')', '(', '|', '\\', '/',
		't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
		'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z',
		'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o',
		'*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'
	};
}

#endif // GUARD_GSTOCHAR_H