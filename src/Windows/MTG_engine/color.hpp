#ifndef MTG_ENGINE_COLOR_H
#define MTG_ENGINE_COLOR_H

#include <unordered_map>
#include <string>

/**
* enum class color
**/
enum class color {W,U,B,R,G};
//white, blue, black, red, green, names are like this because MTG calls them this way


/**
* converts a string to a color
* 
* @param color_string the string to convert
* 
* @returns the color
**/
inline color string_to_color_map(const std::string& color_string) {
	const std::unordered_map<std::string, color> color_map = {
		{"W", color::W},
		{"U", color::U},
		{"B", color::B},
		{"R", color::R},
		{"G", color::G}
	};

	auto it = color_map.find(color_string);
	if (it != color_map.end()) {
		return it->second;
	}
	throw ("invalid color string");
}

/**
* converts a char to a color
* 
* @param color_char the char to convert
* 
* @returns the color
**/
inline color char_to_color_map(const char& color_char) {
	std::string color_string = std::string(1, color_char);
	return string_to_color_map(color_string);
}

/**
* converts a color to a string
* 
* @param color_of_mana the color to convert
* 
* @returns the string
**/
inline std::string color_to_string(color color_of_mana) {
	switch (color_of_mana) {
		case color::W: 
			return "white";
		case color::U:
			return "Blue";
		case color::B:
			return "Black";
		case color::R:
			return "Red";
		case color::G:
			return "Green";
		default:
			throw "NOT A VALID COLOR, START AGAIN";
	}
}

#endif //MTG_ENGINE_COLOR_H
