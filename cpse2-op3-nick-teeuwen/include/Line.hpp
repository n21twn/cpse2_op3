#pragma once
#include <ScreenObjects.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
//----------------------------------LINE----------------------------------
class Line : public ScreenObject {
public:
    Line(sf::Vector2f pos, sf::Color color, sf::Vector2f pos2);
    void draw(sf::RenderWindow& window) override;
    bool contains(sf::Vector2f p) const override;
    std::string serialize() const override;
protected:
    void onMove(sf::Vector2f delta) override;
private:
    sf::Vector2f pos2;
};