#pragma once
#include <Windows.h>
#include <string>
#include <atlcomcli.h>
#include <vector>

namespace stev {
	class swStringUtil {
	public:
		static CComVariant convertToVariant(std::wstring string);
		static CComBSTR convertToBSTR(std::wstring string);
		static CComVariant convertToVariant(std::string string);
		static CComBSTR convertToBSTR(std::string string);

		static std::string convertToString(CComBSTR str);
		static std::string convertVarToString(CComVariant str);

		static std::vector<std::string> splitText(std::string mainText, std::string delimeter);
		static std::vector<std::wstring> splitTextW(std::wstring mainText, std::wstring delimeter);

		static std::string appendString(std::vector<std::string> list, std::string comb);
		static std::wstring appendStringW(std::vector<std::wstring> list, std::wstring comb);
		static std::wstring appendStringArrayW(std::vector<std::wstring> list, std::wstring comb);

		static std::wstring convertToWString(std::string _string);
		static std::string convertWToString(std::wstring _string);
	};
};