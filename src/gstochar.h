#ifndef GUARD_GSTOCHAR_H
#define GUARD_GSTOCHAR_H
#pragma once

namespace gtc {
	const char chGSRamp[] = {
		' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I',
		'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?',
		']', '[', '}', '{', '1', ')', '(', '|', '\\', '/',
		't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z',
		'X', 'Y', 'U', 'J', 'C', 'L', 'Q', '0', 'O', 'Z',
		'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o',
		'*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'
	};
	const int nRampLen = (int)strlen(chGSRamp);// 70;
	const float fRampConversion = (float)(255. / (nRampLen - 1));// 3.64285714f;
}

#endif // GUARD_GSTOCHAR_H