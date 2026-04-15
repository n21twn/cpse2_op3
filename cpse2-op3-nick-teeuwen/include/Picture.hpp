#pragma once
#include "ScreenObjects.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>
//----------------------------------PICTURE----------------------------------
class Picture : public ScreenObject {
public:
    Picture(sf::Vector2f pos, const std::string& filename, int lineNum);
    void draw(sf::RenderWindow& window) override;
    bool contains(sf::Vector2f p) const override;
    std::string serialize() const override;
private:
    std::string filename;
    sf::Texture texture;
    sf::Sprite  sprite;
};