#pragma once
#include "ScreenObjects.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>
//----------------------------------CIRCLE----------------------------------
class Circle : public ScreenObject {
public:
    Circle(sf::Vector2f pos, sf::Color color, float diameter);
    void draw(sf::RenderWindow& window) override;
    bool contains(sf::Vector2f p) const override;
    std::string serialize() const override;
private:
    float diameter;
};