#ifndef MTG_ENGINE_INI_PARSER_H
#define MTG_ENGINE_INI_PARSER_H

#include <fstream>
#include <string>
#include <map>

class IniParser {
public:
    using IniData = std::map<std::string, std::map<std::string, std::string>>;

    IniParser();
    ~IniParser();

    /**
    * parses an INI file and returns a map of sections and key-value pairs
    * @param filename the name of the INI file
    * 
    * @returns a map of sections and key-value pairs
    **/
    IniData parseIniFile(const std::string& filename);

private:
    void removeLeadingTrailing(std::string& str);
    bool isSectionHeader(std::string& line);
    bool isKeyValuePair(std::string& line);
};

#endif //MTG_ENGINE_INI_PARSER_H
