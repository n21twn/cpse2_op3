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

class ScreenObject {
public:
    ScreenObject(sf::Vector2f pos, sf::Color color)
        : position(pos), color(color) {}
    virtual ~ScreenObject() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual std::string serialize() const = 0;
    virtual bool contains(sf::Vector2f pt) const = 0;

	bool isSelected() const { return selected; }  // Flag om aan te geven of dit object geselecteerd is
	void setSelected(bool s) { selected = s; } // Setter voor de geselecteerde status van het object

    void move(sf::Vector2f delta) {
        position += delta;
        onMove(delta);
    }

    sf::Vector2f getPosition() const { return position; }

protected:
    sf::Vector2f position;
    sf::Color    color;
    bool         selected = false;

    virtual void onMove(sf::Vector2f delta) {}

    // SFML 2: FloatRect heeft .left/.top/.width/.height
    void drawSelectionBox(sf::RenderWindow& window, sf::FloatRect bounds) const {
        sf::RectangleShape box(sf::Vector2f(bounds.width + 6.f, bounds.height + 6.f));
        box.setPosition(bounds.left - 3.f, bounds.top - 3.f);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        window.draw(box);
    }
};
