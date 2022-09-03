#pragma once
#include <map>
#include "sJsonNode.h"
#include <string>
#include <variant>
#include <fstream>
#include <sstream>
#include <functional>

namespace stev {
	namespace DOM {
		class sJson {
		private:
			std::string evaluateName(std::wistream& file);
			sJsonNode evaluateObject(std::wistream& file);
			sJsonNode evaluateArray(std::wistream& file);
			sJsonNode evaluateString(std::wistream& file);
			sJsonNode evaluateNumber(std::wistream& file);
			sJsonNode evaluateBoolean(std::wistream& file);
			sJsonNode evaluateNull(std::wistream& file);

		public:
			/// <summary>
			/// Parse a Json Data from memory.
			/// </summary>
			/// <param name="jsonData"></param>
			/// <returns></returns>
			sJsonNode parseData(std::wstring jsonData);
			sJsonNode parseFile(std::wstring filePath);
		};
	}
	namespace SAX {
		class sJsonSax {
			private:
				std::string evaluateName(std::wistream& file);

				void evaluateObject(std::wistream& file, std::variant<int, std::string> varName);
				void evaluateArray(std::wistream& file, std::variant<int, std::string> varName);
				void evaluateString(std::wistream& file, std::variant<int, std::string> varName);
		
				void evaluateNumber(std::wistream& file, std::variant<int,std::string> varName);
				void evaluateBoolean(std::wistream& file, std::variant<int, std::string> varName);
				void evaluateNull(std::wistream& file, std::variant<int, std::string> varName);

			public:
				/// <summary>
				/// Parse a Json Data from memory.
				/// </summary>
				/// <param name="jsonData"></param>
				/// <returns></returns>
				void parseData(std::wstring jsonData);
				void parseFile(std::wstring filePath);
				std::function<void(std::variant<int, std::string>)> onObjectBegin;
				std::function<void(std::variant<int, std::string>, int) > onObjectEnd;
				std::function<void(std::variant<int, std::string>)> onArrayBegin;
				std::function<void(std::variant<int, std::string>, int) > onArrayEnd;
				std::function<void(std::variant<int, std::string>, int)> onInteger;
				std::function<void(std::variant<int, std::string>, float)> onFloat;
				std::function<void(std::variant<int, std::string>, double)> onDouble;
				std::function<void(std::variant<int, std::string>, bool)> onBoolean;
				std::function<void(std::variant<int, std::string>, void*)> onNull;
				std::function<void(std::variant<int, std::string>, std::wstring)> onString;
		};
	}
}