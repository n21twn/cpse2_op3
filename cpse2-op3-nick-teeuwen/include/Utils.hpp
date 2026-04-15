#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>

struct ParseError : public std::runtime_error { // Documentatie run time error https://en.cppreference.com/w/cpp/error/runtime_error.html
    int line;
    ParseError(int ln, const std::string& msg)
        : std::runtime_error("Line " + std::to_string(ln) + ": " + msg), line(ln) {}
};

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
