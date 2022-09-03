#include "sJson.h"
#include <fstream>

namespace stev {
    namespace DOM {
        std::string sJson::evaluateName(std::wistream& file)
        {
            std::string text = "";
            bool stop = false;
            wchar_t get_char = L'\0';

            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case '\"': { //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                    stop = true;
                    break;
                }
                default: {
                    text += (char)get_char;
                }

                }
                file.get(); //Iterate the final " to go on to the next character.
            }
            return text;
        }
        sJsonNode sJson::evaluateObject(std::wistream& file)
        {
            sJsonNode node;
            sJsonObject value = sJsonObject();
            std::string getVarName = "";
            bool getName = false; //Specifies we are getting a variable name.
            bool getValue = false; //Specifies we are getting a value.

            //Note that we are already inside a { , so we don't have anything to specify the first '{' here.
            while (!file.eof()) {
                wchar_t get_char = file.peek();
                switch (get_char) {
                case '{': { //Start of a new bracket.
                    file.get();
                    break;
                }
                case '}': { //End of a bracket. Return our findings of our JSON value.
                    file.get();
                    node = value;
                    return node;

                }
                case ',': { //Next variable.
                    getVarName = "";
                    file.get();
                    break;
                }
                case '\"': { //This can specify a beginning of the variable or a string value.
                    //We are looking for a variable name
                    if (getValue == false) {
                        file.get(); //Advance forward to next character and get name.
                        getVarName = this->evaluateName(file);

                    }
                    break;
                }
                case ':': { //The quotations represent that we are getting its value. This will set our 'getvalue' flag to true to capture our value.
                    getValue = true;
                    file.get();
                    break;
                }
                case '\n':
                case '\t':
                case ' ':
                case '\r': //Do not do anything.
                    file.get();
                    break;
                }
                if (getVarName == "EggData") {
                    int i = 0;
                }
                if (getValue) {
                    if (get_char == '{') { //object
                        value[getVarName] = this->evaluateObject(file);
                        getValue = false;
                    }
                    if (get_char == '[') { //array
                        value[getVarName] = this->evaluateArray(file);
                        getValue = false;
                    }
                    else if (get_char == 'f' || get_char == 't') { //boolean
                        value[getVarName] = this->evaluateBoolean(file);
                        getValue = false;
                    }
                    else if (get_char == '\"') { //string
                        value[getVarName] = this->evaluateString(file);
                        getValue = false;
                    }
                    else if (get_char == 'n') { //null
                        value[getVarName] = this->evaluateNull(file);
                        getValue = false;
                    }
                    else if (std::isdigit(get_char) || get_char == '-' || get_char == '.') { //number
                        value[getVarName] = this->evaluateNumber(file);
                        getValue = false;
                    }
                }
            }
            node = value;
            return node;
        }
        sJsonNode sJson::evaluateArray(std::wistream& file)
        {
            sJsonNode data;
            std::vector<sJsonNode> get_array;
            //Note that we are already inside a { , so we don't have anything to specify the first '{' here.
            file.get();

            while (!file.eof()) {
                wchar_t getC = file.peek();
                switch (getC) {
                case '{': {
                    sJsonNode value;
                    value = evaluateObject(file);
                    get_array.push_back(value);
                    break;
                }
                case '[': {
                    get_array.push_back(this->evaluateArray(file));
                    break;
                }
                case ']': {
                    data = get_array;
                    file.get();
                    return data;
                }
                case ',':
                case ' ':
                case '\n':
                case '\t':
                case '\r'://ignore and continue.
                    file.get();
                    break;
                case 'f':
                case 't': //boolean
                    get_array.push_back(this->evaluateBoolean(file));
                    break;
                case '\"': //string
                    get_array.push_back(this->evaluateString(file));
                    break;
                case 'n': //null   
                    get_array.push_back(this->evaluateNull(file));
                    break;
                case '-':
                case '.':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    get_array.push_back(this->evaluateNumber(file));
                    break;
                }
            }
            return data;
        }
        sJsonNode sJson::evaluateString(std::wistream& file)
        {
            sJsonNode data;
            std::wstring text = L"";
            bool stop = false;
            bool ignoreQuote = false;
            wchar_t get_char = L'\0';
            file.get(); //Go on to the next character.
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case '\"': { //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                    if (!ignoreQuote) {
                        stop = true;
                    }
                    else {
                        text += get_char;
                        ignoreQuote = false;
                    }

                    break;
                }
                case '\\': { //Do not finish processing string even if " exists.
                    ignoreQuote = true;
                    break;
                }
                default: {
                    text += get_char;
                }

                }
                file.get();
            }
            data = text;
            return data;
        }

        sJsonNode sJson::evaluateNumber(std::wistream& file)
        {
            sJsonNode data;
            std::wstring text = L"";
            bool stop = false;
            bool isFloat = false;
            bool allowNegative = true;
            bool hasDecimal = false;

            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();
                switch (get_char) {
                case '}':  //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                case ',':
                case ']':
                {
                    stop = true;
                    break;
                }
                case '.': {
                    if (!hasDecimal) {
                        isFloat = true;
                        hasDecimal = true;
                        file.get();
                        break;
                    }
                }
                case '-': {
                    if (allowNegative) {
                        text += get_char;
                        file.get();
                        break;
                    }
                }
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    text += get_char;
                    file.get();
                    break;
                }
                default: {
                    file.get();
                }
                }
                allowNegative = false;
            }
            if (!isFloat) {
                try {
                    data = stoi(text);
                }
                catch (std::out_of_range&) { //overflow error
                    try {
                        data = stof(text); //convert to float instead.
                    }
                    catch (std::out_of_range&) { //convert to a double.
                        data= stod(text);
                    }
                }
            }
            else {
                try {
                    data = stof(text); //convert to float instead.
                }
                catch (std::out_of_range&) { //convert to a double.
                    data = stod(text);
                }
            }
            return data;
        }
        sJsonNode sJson::evaluateBoolean(std::wistream& file)
        {
            sJsonNode data2;
            bool text = false;
            std::wstring data = L"";
            wchar_t get_char = file.peek();
            bool stop = false;
            if (get_char == L't') {
                data = L"t";
            }
            if (get_char == L'f') {
                data = L"f";
            }
            file.get();
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case ',':
                case '\n':
                case '}':
                    stop = true;
                    break;
                default:
                    data += get_char;
                    break;
                }
                file.get();
            }
            if (data == L"true") {
                data2 = true;

            }
            else if (data == L"false") {
                data2 = false;
            }
            else { //Not a boolean it is null by default.
                data2 = (void*)nullptr;
            }
            return data2;
        }
        sJsonNode sJson::evaluateNull(std::wistream& file)
        {
            sJsonNode data2;
            bool text = false;
            std::wstring data = L"";
            wchar_t get_char = file.peek();
            bool stop = false;
            if (get_char == L'n') {
                data = L"n";
            }
            file.get();
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case ',':
                case '\n':
                case '}':
                    stop = true;
                    break;
                default:
                    data += get_char;
                    break;
                }
                file.get();
            }
            if (data == L"null") {
                data2 = (void*)nullptr;
            }
            else { //should throw exception lol.
                data2 = (void*)nullptr;
            }
            return data2;
        }
        sJsonNode sJson::parseData(std::wstring jsonData)
        {
            sJsonNode node;

            std::wstringstream loaded_string;
            loaded_string << jsonData;

            while (!loaded_string.eof()) {

                wchar_t get_char = loaded_string.get();

                switch (get_char) {
                    case '{': {//Beginning of a bracket.    
                        node = evaluateObject(loaded_string);
                        break;
                    }
                    case '}': { //End of a bracket.
                        break;
                    }
                    case '[': {//Beginning of a bracket.    
                        node = evaluateArray(loaded_string);
                        break;
                    }
                    case ']': { //End of a array.
                        break;
                    }
                }
            }

            return node;

        }
        sJsonNode sJson::parseFile(std::wstring filePath)
        {
            sJsonNode node;
            std::wfstream loaded_file(filePath);
            if (loaded_file.is_open()) {
                while (!loaded_file.eof()) {
                    wchar_t get_char = L'\0';
                    loaded_file >> std::noskipws >> get_char;

                    switch (get_char) {
                    case '{': {//Beginning of a bracket.    
                        node = evaluateObject(loaded_file);
                        break;
                    }
                    case '}': { //End of a bracket.
                        break;
                    }
                    case '[': {//Beginning of a bracket.    
                        node = evaluateArray(loaded_file);
                        break;
                    }
                    case ']': { //End of a array.
                        break;
                    }
                    }
                }
                loaded_file.close();
                return node;
            }
            return sJsonNode();
        }
    }
    namespace SAX {
        std::string sJsonSax::evaluateName(std::wistream& file)
        {
            std::string text = "";
            bool stop = false;
            wchar_t get_char = L'\0';

            while (!file.eof() && !stop) {
                wchar_t get_char = file.peek();
                switch (get_char) {
                    case '\"': { //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                        stop = true;
                        break;
                    }
                    default: {
                        text += (char)get_char;
                    }
                }
                file.get(); //Iterate the final " to go on to the next character.
            }
            return text;
        }
        void sJsonSax::evaluateObject(std::wistream& file, std::variant<int, std::string> varName)
        {
            std::string getVarName = "";
            bool getName = false; //Specifies we are getting a variable name.
            bool getValue = false; //Specifies we are getting a value.
            int count = 0;
            if (this->onObjectBegin) {
                this->onObjectBegin(varName);
            }

            //Note that we are already inside a { , so we don't have anything to specify the first '{' here.
            while (!file.eof()) {
                wchar_t get_char = file.peek();
                switch (get_char) {
                case '{': { //Start of a new bracket.
                    file.get();
                    break;
                }
                case '}': { //End of a bracket. Return our findings of our JSON value.
                    file.get();
                    if (this->onObjectEnd) {
                        this->onObjectEnd(varName, count);
                    }

                }
                case ',': { //Next variable.
                    getVarName = "";
                    file.get();
                    break;
                }
                case '\"': { //This can specify a beginning of the variable or a string value.
                    //We are looking for a variable name
                    if (getValue == false) {
                        file.get(); //Advance forward to next character and get name.
                        getVarName = this->evaluateName(file);

                    }
                    break;
                }
                case ':': { //The quotations represent that we are getting its value. This will set our 'getvalue' flag to true to capture our value.
                    getValue = true;
                    file.get();
                    break;
                }
                case '\n':
                case '\t':
                case ' ':
                case '\r': //Do not do anything.
                    file.get();
                    break;
                }

                if (getValue) {
                    if (get_char == '{') { //object
                        this->evaluateObject(file, getVarName);
                        getValue = false;
                        count++;
                    }
                    if (get_char == '[') { //array
                       this->evaluateArray(file, getVarName);
                        getValue = false;
                        count++;
                    }
                    else if (get_char == 'f' || get_char == 't') { //boolean
                        this->evaluateBoolean(file, getVarName);
                        getValue = false;
                        count++;
                    }
                    else if (get_char == '\"') { //string
                        this->evaluateString(file, getVarName);
                        getValue = false;
                        count++;
                    }
                    else if (get_char == 'n') { //null
                        this->evaluateNull(file, getVarName);
                        getValue = false;
                        count++;
                    }
                    else if (std::isdigit(get_char) || get_char == '-' || get_char == '.') { //number
                        this->evaluateNumber(file, getVarName);
                        getValue = false;
                        count++;
                    }
                }
            }
            if (this->onObjectEnd) {
                this->onObjectEnd(varName, count);
            }
        }
      void sJsonSax::evaluateArray(std::wistream& file, std::variant<int, std::string> varName)
        {
 
            //Note that we are already inside a { , so we don't have anything to specify the first '{' here.
            file.get();
            int index = 0;
            if (this->onArrayBegin) {
                this->onArrayBegin(varName);
            }
            while (!file.eof()) {
                wchar_t getC = file.peek();
                switch (getC) {
                case '{': {
                    evaluateObject(file,index);
                   
                    index++;
                    break;
                }
                case '[': {
                    this->evaluateArray(file, index);
                    index++;
                    break;
                }
                case ']': {
                    file.get();
                    if (this->onArrayEnd) {
                        this->onArrayEnd(varName, index);
                    }
                    return;
                }
                case ',':
                case ' ':
                case '\n':
                case '\t':
                case '\r'://ignore and continue.
                    file.get();
                    break;
                case 'f':
                case 't': //boolean
                    this->evaluateBoolean(file, index);
                    index++;
                    break;
                case '\"': //string
                    this->evaluateString(file, index);
                    index++;
                    break;
                case 'n': //null   
                    this->evaluateNull(file, index);
                    index++;
                    break;
                case '-':
                case '.':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    this->evaluateNumber(file, index);
                    index++;
                    break;
                }
            }
            if (this->onObjectEnd) {
                this->onObjectEnd(varName, index);
            }
        }
        void sJsonSax::evaluateString(std::wistream& file, std::variant<int, std::string> varName)
        {
            std::wstring text = L"";
            bool stop = false;
            bool ignoreQuote = false;
            wchar_t get_char = L'\0';
            file.get(); //Go on to the next character.
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                    case '\"': { //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                        if (!ignoreQuote) {
                            stop = true;
                        }
                        else {
                            text += get_char;
                            ignoreQuote = false;
                        }
                        break;
                    }
                    case '\\': { //Do not finish processing string even if " exists.
                        ignoreQuote = true;
                        break;
                    }
                    default: {
                        text += get_char;
                    }
                }
                file.get();
              
            }
            if (this->onString) {
                this->onString(varName, text);
            }
        }
        void sJsonSax::evaluateNumber(std::wistream& file, std::variant<int, std::string> varName)
        {
            std::wstring text = L"";
            bool stop = false;
            bool isFloat = false;
            bool allowNegative = true;
            bool hasDecimal = false;

            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();
                switch (get_char) {
                case '}':  //Stop looping until we get the '"'. However if the string contains a \", we ignore it.
                case ',':
                case ']':
                {
                    stop = true;
                    break;
                }
                case '.': {
                    if (!hasDecimal) {
                        isFloat = true;
                        hasDecimal = true;
                        file.get();
                        break;
                    }
                }
                case '-': {
                    if (allowNegative) {
                        text += get_char;
                        file.get();
                        break;
                    }
                }
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    text += get_char;
                    file.get();
                    break;
                }
                default: {
                    file.get();
                }
                }
                allowNegative = false;
            }
            if (!isFloat) {
                try {
                   int in = stoi(text);
                   if (this->onInteger) {                   
                       this->onInteger(varName, in);
                   }
                }
                catch (std::out_of_range&) { //overflow error
                    try {
                        float in = stof(text); //convert to float instead.
                        if (this->onFloat) {
                            this->onFloat(varName, in);
                        }
                    }
                    catch (std::out_of_range&) { //convert to a double.
                        double in = stod(text);
                        if (this->onDouble) {
                            this->onDouble(varName, in);
                        }
                    }
                }
            }
            else {
                try {
                    float in = stof(text); //convert to float instead.
                    if (this->onFloat) {
                        this->onFloat(varName, in);
                    }
                }
                catch (std::out_of_range&) { //convert to a double.
                    double in = stod(text);
                    if (this->onDouble) {
                        this->onDouble(varName, in);
                    }
                }
            }
        }
        void sJsonSax::evaluateBoolean(std::wistream& file, std::variant<int, std::string> varName)
        {
            bool text = false;
            std::wstring data = L"";
            wchar_t get_char = file.peek();
            bool stop = false;
            if (get_char == L't') {
                data = L"t";
            }
            if (get_char == L'f') {
                data = L"f";
            }
            file.get();
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case ',':
                case '\n':
                case '}':
                    stop = true;
                    break;
                default:
                    data += get_char;
                    break;
                }
                file.get();
            }
            if (data == L"true") {
                if (this->onBoolean) {
                    this->onBoolean(varName, true);
                }

            }
            else if (data == L"false") {
                if (this->onBoolean) {
                    this->onBoolean(varName, false);
                }
            }
            else { //Not a boolean it is null by default.
                (void*)nullptr;
            }
        }
        void sJsonSax::evaluateNull(std::wistream& file, std::variant<int, std::string> varName)
        {
            bool text = false;
            std::wstring data = L"";
            wchar_t get_char = file.peek();
            bool stop = false;
            if (get_char == L'n') {
                data = L"n";
            }
            file.get();
            while (!file.eof() && !stop) {

                wchar_t get_char = file.peek();

                switch (get_char) {
                case ',':
                case '\n':
                case '}':
                    stop = true;
                    break;
                default:
                    data += get_char;
                    break;
                }
                file.get();
            }
            if (data == L"null") {
                if (this->onNull) {
                    this->onNull(varName, nullptr);
                }
            }
            else { //should throw exception lol.
                (void*)nullptr;
            }
        }
        void sJsonSax::parseData(std::wstring jsonData)
        {
            sJsonNode node;

            std::wstringstream loaded_string;
            loaded_string << jsonData;

            while (!loaded_string.eof()) {

                wchar_t get_char = loaded_string.get();

                switch (get_char) {
                    case '{': {//Beginning of a bracket.    
                        evaluateObject(loaded_string, "StartJson");
                        break;
                    }
                    case '}': { //End of a bracket.
                        break;
                    }
                    case '[': {//Beginning of a bracket.    
                        evaluateArray(loaded_string, "StartJson");
                        break;
                    }
                    case ']': { //End of a array.
                        break;
                    }
                }
            }
        }
        void sJsonSax::parseFile(std::wstring filePath)
        {
            std::wfstream loaded_file(filePath);
            if (loaded_file.is_open()) {
                while (!loaded_file.eof()) {
                    wchar_t get_char = L'\0';
                    loaded_file >> std::noskipws >> get_char;

                    switch (get_char) {
                        case '{': {//Beginning of a bracket.    
                            evaluateObject(loaded_file,"StartJson");
                            break;
                        }
                        case '}': { //End of a bracket.
                            break;
                        }
                        case '[': {//Beginning of a bracket.    
                            evaluateArray(loaded_file,"StartJson");
                            break;
                        }
                        case ']': { //End of a array.
                            break;
                        }
                    }
                }
                loaded_file.close();
            }
        }
    }
}