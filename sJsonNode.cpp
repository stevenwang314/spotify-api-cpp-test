#include "sJsonNode.h"
namespace stev {
	int sJsonNode::getInteger() {
		if (this->value.index() == 1) {
			return std::get<int>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a integer");
			return 0;
		}
	}
	double sJsonNode::getDouble() {
		if (this->value.index() == 2) {
			return std::get<double>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a double");
			return 0.0;
		}
	}
	float sJsonNode::getFloat() {
		if (this->value.index() == 3) {
			return std::get<float>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a float");
			return 0.f;
		}
	}
	std::wstring sJsonNode::getUnicodeString() {
		if (this->value.index() == 4) {
			return std::get<std::wstring>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a unicode string");
			return L"";
			
		}
	}
	sJsonObject sJsonNode::getObject()
	{
		if (this->value.index() == 5) {
			return std::get<sJsonObject>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a object");
			return sJsonObject();
		}
	}
	bool sJsonNode::getBoolean() {
		if (this->value.index() == 6) {
			return std::get<bool>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a boolean");
			return false;
		}
	}
	
	sJsonArray sJsonNode::getArray()
	{
		if (this->value.index() ==7) {
			return std::get<sJsonArray>(this->value);
		}
		else {
			throw sJsonError("Error: this is not a array");
		}
	}
	
	sJsonNode& sJsonNode::operator[](std::string index) {
		if (this->value.index() == 5) {
			if (std::get<sJsonObject>(this->value).find(index) != std::get<sJsonObject>(this->value).end()) { //Check we have the key.
				return std::get<sJsonObject>(this->value)[index];
			}
			else {
				throw sJsonIndexError("Error: no such item exists within the object", -1, index);
				return *this;
			}
		}
		else {
			throw sJsonError("Error: this is not a object");
			return *this;
		}
		
	}
	sJsonNode& sJsonNode::operator[](int index) {
		if (this->value.index() == 7) {
			if (index < (int)std::get<sJsonArray>(this->value).size())
				return std::get<sJsonArray>(this->value)[index];
			else {
				throw sJsonIndexError("Error: no such item exists within the array", index, "");
				return *this;
			}
		}
		else {
			throw sJsonError("Error: this is not a array");
			return *this;
			
		}
	}
	sJsonNode& sJsonNode::operator=(const sJsonValue& value) {
		this->value = value;
		return *this;
	}
	std::wostream& operator<<(std::wostream& os, const sJsonNode& value)
	{
		switch (value.value.index()) {
		case 0:
			os << "null";
			return os;
		case 1:
			os << "Integer-" << std::get<1>(value.value);
			return os;
		case 2:
			os << "Double-" << std::get<2>(value.value);
			return os;
		case 3:
			os << "Float-" << std::get<3>(value.value);
			return os;
		case 4: {
			os << "String-" << std::get<4>(value.value);
			return os;
		}
		case 5: {
			os << L"Map count-" << (int)std::get<5>(value.value).size();
			return os;
		}
		case 6:
			os << L"Boolean-" << (std::get<6>(value.value) ? "true" : "false");
			return os;
		case 7: {
			os << L"Array count-" << (int)std::get<7>(value.value).size();
			return os;
		}
		default:
			return os;
		}
	}
}
