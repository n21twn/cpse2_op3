#pragma once
#include "Utils.hpp"
#include <SFML/Graphics.hpp>
#include <string>

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
