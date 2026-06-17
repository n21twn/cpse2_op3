#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>
#include <regex>


struct ParseError : public std::runtime_error { // Documentatie run time error https://en.cppreference.com/w/cpp/error/runtime_error.html
    int line;
    ParseError(int ln, const std::string& msg)
        : std::runtime_error("Line " + std::to_string(ln) + ": " + msg), line(ln) {}
};

// Onbekend object-type (bv. "RXCTANGLE")
struct UnknownTypeError : public ParseError {
    UnknownTypeError(int ln, const std::string& type)
        : ParseError(ln, "Unknown object type: '" + type + "'") {
    }
};

// Ongeldig formaat voor een parameter (bv. diameter, kleur, coordinaten)
struct InvalidParameterError : public ParseError {
    InvalidParameterError(int ln, const std::string& msg)
        : ParseError(ln, msg) {
    }
};

// Ontbrekende parameters (te weinig tokens op een regel)
struct MissingParameterError : public ParseError {
    MissingParameterError(int ln, const std::string& msg)
        : ParseError(ln, msg) {
    }
};

// Bestandsfout: kan bestand niet openen / niet laden
struct FileError : public std::runtime_error {
    FileError(const std::string& msg)
        : std::runtime_error(msg) {}
};

// Afbeelding kan niet geladen worden
struct ImageLoadError : public ParseError {
    ImageLoadError(int ln, const std::string& path)
        : ParseError(ln, "Cannot load image file: '" + path + "'") {
    }
};

struct EmptyFileError : public std::runtime_error {
    EmptyFileError(const std::string& filename)
        : std::runtime_error("Unexpected end of file while reading: '" + filename + "'") {}
};


inline float parseDiameter(const std::string& s, int lineNum) {
    static const std::regex diameterRe(R"(^-?\d+$)"); // regex voor een positief getal en geen floats 
    if (!std::regex_match(s, diameterRe))
        throw InvalidParameterError(lineNum, "Diameter: '" + s + "' is not a valid number");
    return (float)std::stoi(s);
}

inline void showError(sf::RenderWindow& window, const sf::Font& font,
    const std::string& message)
{
    sf::Text text;
    text.setFont(font);
    text.setString(message);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);
    text.setPosition(10.f, 10.f);
    window.draw(text);
    }


inline sf::Color parseColor(const std::string& s, int lineNum) { // zet de string om naar een SFML kleur
    if (s == "black")   return sf::Color::Black;
    if (s == "white")   return sf::Color::White;
    if (s == "red")     return sf::Color::Red;
    if (s == "green")   return sf::Color::Green;
    if (s == "blue")    return sf::Color::Blue;
    if (s == "yellow")  return sf::Color::Yellow;
    if (s == "magenta") return sf::Color::Magenta;
    if (s == "cyan")    return sf::Color::Cyan;
    throw ParseError(lineNum, "Unknown color: '" + s + "'");
}

inline std::string colorToString(const sf::Color& c) { // zet een SFML kleur om naar een string
    if (c == sf::Color::Black)   return "black";
    if (c == sf::Color::White)   return "white";
    if (c == sf::Color::Red)     return "red";
    if (c == sf::Color::Green)   return "green";
    if (c == sf::Color::Blue)    return "blue";
    if (c == sf::Color::Yellow)  return "yellow";
    if (c == sf::Color::Magenta) return "magenta";
    if (c == sf::Color::Cyan)    return "cyan";
    return "black";
}

