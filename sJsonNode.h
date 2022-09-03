#pragma once
#include <variant>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <iostream>

namespace stev {
	class sJsonNode;
	using sJsonObject = std::map<std::string, sJsonNode>;
	using sJsonArray = std::vector<sJsonNode>;
	using sJsonValue = std::variant<void*, int, double, float, std::wstring, sJsonObject, bool, sJsonArray>;
	class sJsonIndexError : std::exception {
		std::string error_message;
		int source;
		std::string nameSource;
	public:
		sJsonIndexError(std::string errorMessage, int source, std::string nameSource) : error_message(errorMessage), source(source), nameSource(nameSource) {
			error_message = errorMessage + 
			(source != -1 ? " Index: " +  std::to_string(source) : "")
			+ (nameSource != "" ? " String: " + nameSource: "");
		}
		virtual const char* what() const throw () {
			return error_message.c_str();
		}
	};
	class sJsonError : std::exception {
		std::string error_message;
	public:
		sJsonError(std::string errorMessage) : error_message(errorMessage) {

		}
		virtual const char* what() const throw () {
			return error_message.c_str();
		}
	};

	class sJsonNode {
		friend class sJson;
		sJsonValue value;
	public:
		sJsonNode& operator[](std::string index);
		sJsonNode& operator[](int index);
		sJsonNode& operator=(const sJsonValue& value);
		friend std::wostream& operator<<(std::wostream& os, const sJsonNode& value);
		int getInteger();
		double getDouble();
		float getFloat();
		bool getBoolean();
		std::wstring getUnicodeString();
		sJsonArray getArray();
		sJsonObject getObject();
	};
}