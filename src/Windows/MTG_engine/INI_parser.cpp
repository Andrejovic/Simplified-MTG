#include "INI_parser.hpp"

#include <iostream>

IniParser::IniParser() {}

IniParser::~IniParser() {}

void IniParser::removeLeadingTrailing(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t"));
    str.erase(str.find_last_not_of(" \t") + 1);
}

bool IniParser::isSectionHeader(std::string& line) {
    return line[0] == '[' && line[line.size() - 1] == ']';
}

bool IniParser::isKeyValuePair(std::string& line) {
    return line.find('=') != std::string::npos;
}

IniParser::IniData IniParser::parseIniFile(const std::string& filename) {
    IniData data;
    std::ifstream file(filename);

	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filename << "\n";
		return data;
	}

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        removeLeadingTrailing(line);

        if (isSectionHeader(line)) {
            currentSection = line.substr(1, line.size() - 2);
            data[currentSection] = {};
        } else if (isKeyValuePair(line)) {
            size_t equalsPos = line.find('=');
            std::string key = line.substr(0, equalsPos);
            std::string value = line.substr(equalsPos + 1);

            removeLeadingTrailing(key);
            removeLeadingTrailing(value);

            data[currentSection][key] = value;
        }
    }

    file.close();
    return data;
}