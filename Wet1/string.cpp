#include <cstring>
#include "string.h"
#include <iostream>

typedef enum { SUCCESS = 0, FAIL } Result;

//------------------help_functions--------------------------------------//
void printString(const char* data , const size_t length) {
    std::cout << "String len is " << length << std::endl;
    if (data == nullptr)
        std::cout << "pointer is null " << std::endl;
    else {
        std::cout << "String actual len is " << strlen(data) << std::endl;
        std::cout << "String data is " << data << std::endl;
    }
    std::cout << std::endl;
}

char* copy_string(const char* data, const size_t length) {
    if (data == nullptr || length == 0) {
        char* cpy_data = new char[1];
        *cpy_data = '\0';
        return cpy_data;
    }
    
    char* cpy_data = new char[length + 1];
    std::memcpy(cpy_data, data, length);
    cpy_data[length] = '\0';
    return cpy_data;
}
 
int destroy_string(char *data) {
    if(data == NULL)
        return FAIL;  
    delete[] data;
    return SUCCESS;
}

//------------------constructors--------------------------------------//

String::String() {
    //std::cout << "default const" <<  std::endl;
    this->data = new char[1];
    *(this->data) = '\0';
    this->length = 0;
}
String::String(const String &str) {//done
    //std::cout << "copy form string const" <<  std::endl;
    this->length = str.length;
    if (str.length == 0 || str.data == nullptr){
        this->data = new char[1];
        *(this->data) = '\0';
    } 
    else { 
        this->data = copy_string(str.data, str.length);
    }
}

String::String(const char *str) {
    //std::cout << "copy form char* const" <<  std::endl;
    if(str == nullptr || *str == '\0') {
        this->data = new char[1];
        *(this->data) = '\0';
    }
    else {
        size_t str_len = strlen(str);
        this->data = copy_string(str , str_len);
        this->length = str_len;
    }
}

//------------------destructor-------------------------------------//

String::~String() {
    //std::cout << "destructor" << std::endl;
    if (data != nullptr)
        delete[] this->data;
}

//------------------operator=-------------------------------------//
String& String::operator=(const String& rhs) {
    //std::cout << "into = operator string" <<  std::endl;
    if (this != &rhs) { // Deallocate the existing data
        if (this->data != nullptr) { // Deallocate the existing data
            delete[] this->data;
        }
        if (rhs.data == nullptr || rhs.length == 0) {
            this->data = new char[1];
            *(this->data) = '\0';
            this->length = 0;
            return *this;
        }
        // Copy the data from rhs

        this->data = copy_string(rhs.data, rhs.length);
        this->length = rhs.length;
    }
return *this;
}

String& String::operator=(const char *str) {
    //std::cout << "into = operator char*" <<  std::endl;
    if (strcmp(this->data, str) == 0)
        return *this;

    if (this->data != nullptr) { // Deallocate the existing data
        delete[] this->data;
    }
    if (str == nullptr || *str == '\0') {
        this->data = new char[1];
        *(this->data) = '\0';
        this->length = 0;
        return *this;
    } 
    // Copy the data from rhs
    size_t str_len = strlen(str);
    this->data = copy_string(str , str_len);
    this->length = str_len;
    return *this;
}

//------------------class_functions-------------------------------------//
bool String::equals(const String &rhs) const {
    if (this->data == nullptr || rhs.data == nullptr)
        return false;

    if(rhs.length != this->length) {
        return false;
    }
    if(strcmp(rhs.data,this->data) == 0)
        return true;
    return false;
}

bool String::equals(const char *rhs) const {
    if (rhs == NULL || this->data == NULL)
        return false;
    if(strcmp(rhs , this->data) == 0)
        return true;
    return false;
}

void String::split(const char *delimiters, String **output, size_t *size)
	const{
        if( *data == '\0' || length == 0) {
            *output = nullptr;
            *size = 0;
            return;
        }
        char *our_string1 = copy_string(data , length);
        int deli_count = 0; 
        char *token = strtok(our_string1 ,delimiters);

        while(token != nullptr) {
            deli_count++;
            token = strtok(nullptr,delimiters);
        }
        *size = deli_count;

        if(output == nullptr) {
            delete[] our_string1;
            return;
        }
        if (*output != nullptr)
            delete[] *output;

        String *sub_strings = new String[deli_count];
        char *our_string2 = copy_string(data , length); 
        strcpy(our_string2 , data);

        int curr = 0;
        char *token_2 = strtok(our_string2 ,delimiters);
        while(token_2 != nullptr || curr < deli_count) {
            delete[] sub_strings[curr].data;
            sub_strings[curr].data = new char[strlen(token_2) + 1];
            strcpy(sub_strings[curr].data,token_2);
            sub_strings[curr].length = strlen(token_2);
            curr++;
            token_2 = strtok(nullptr, delimiters);
        }
        *output = sub_strings;
        delete[] our_string1;
        delete[] our_string2;
    }


int String::to_integer() const {
    if (length == 0)
        return 0;
    return std::atoi(this->data); //asuuming the input is valid
}

String String::trim() const {
    if (length == 0) {
        return String();
    }
    size_t start_index = 0;
    size_t end_index = length - 1 ;

    while (end_index > start_index && (data[start_index] == ' ')) {
        start_index++;
    }
    while (end_index > start_index && (data[end_index] == ' ')) {
        end_index--;
    }

    if (end_index <= start_index) {
        // All characters are whitespace, return an empty string
        return String();
    }

    size_t trimmed_len = end_index - start_index + 1;
    char *trimmed_data = copy_string((this->data )+ start_index ,
                                    trimmed_len);
    String trimmedString = String(trimmed_data);
    delete[] trimmed_data;
    return trimmedString;
}