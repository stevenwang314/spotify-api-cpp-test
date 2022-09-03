#include "swStringUtil.h"

namespace stev {
	CComVariant swStringUtil::convertToVariant(std::wstring string)
	{
		CComBSTR bb(string.c_str());
		
		//Set variable to a BSTR and using a BSTR value
		CComVariant Variant;
		V_VT(&Variant) = VT_BSTR;
		V_BSTR(&Variant) = bb;
		return Variant;
	}
	CComBSTR swStringUtil::convertToBSTR(std::wstring string)
	{
		CComBSTR bb(string.c_str());
		return bb;
	}
	CComVariant swStringUtil::convertToVariant(std::string string)
	{
		CComBSTR bb(string.c_str());

		//Set variable to a BSTR and using a BSTR value
		CComVariant Variant;
		V_VT(&Variant) = VT_BSTR;
		V_BSTR(&Variant) = bb;
		return Variant;
	}
	CComBSTR swStringUtil::convertToBSTR(std::string string)
	{
		CComBSTR bb(string.c_str());
		return bb;
	}
	std::string swStringUtil::convertVarToString(CComVariant str)
	{
		std::wstring sstr;
		if (str.vt == VT_BSTR) {
			sstr = str.bstrVal;
		}
		return std::string(sstr.begin(),sstr.end());
	}
	std::vector<std::string> swStringUtil::splitText(std::string mainText, std::string delimeter)
	{
		std::string s = mainText;
		std::string delimiter = delimeter;
		std::vector<std::string> vector_list;
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			vector_list.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		vector_list.push_back(s);
		return vector_list;
	}
	std::vector<std::wstring> swStringUtil::splitTextW(std::wstring mainText, std::wstring delimeter)
	{
		std::wstring s = mainText;
		std::wstring delimiter = delimeter;
		std::vector<std::wstring> vector_list;
		size_t pos = 0;
		std::wstring token;
		while ((pos = s.find(delimiter)) != std::wstring::npos) {
			token = s.substr(0, pos);
			vector_list.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		vector_list.push_back(s);
		return vector_list;
	}
	std::string swStringUtil::appendString(std::vector<std::string> list, std::string comb)
	{
		std::string txt = "";
		size_t listCount = list.size();
		for (int i = 0; i < listCount; i++) {
			txt += list[i];
			if (i != listCount - 1) {
				txt += comb;
			}
		}
		return txt;
	}
	std::wstring swStringUtil::appendStringW(std::vector<std::wstring> list, std::wstring comb)
	{
		std::wstring txt = L"";
		size_t listCount = list.size();
		for (int i = 0; i < listCount; i++) {
			txt += list[i];
			if (i != listCount - 1) {
				txt += comb;
			}
		}
		return txt;
	}
	std::wstring swStringUtil::appendStringArrayW(std::vector<std::wstring> list, std::wstring comb)
	{
		std::wstring txt = L"[";
		size_t listCount = list.size();
		for (int i = 0; i < listCount; i++) {
			txt += '\"';
			txt += list[i];
			txt += '\"';
			if (i != listCount - 1) {
				txt += comb;
			}
		}
		txt += L"]";
		return txt;
	}
	std::wstring swStringUtil::convertToWString(std::string _string)
	{
		return std::wstring(_string.begin(),_string.end());
	}
	std::string swStringUtil::convertWToString(std::wstring _string)
	{
		return std::string(_string.begin(), _string.end());
	}
	std::string swStringUtil::convertToString(CComBSTR str)
	{
		std::wstring ws(str, SysStringLen(str));
	
		return 	std::string(ws.begin(), ws.end());
	}

};