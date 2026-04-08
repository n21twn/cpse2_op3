#pragma once
#include "ScreenObjects.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
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

//----------------------------------RECTANGLE----------------------------------
class Rectangle : public ScreenObject {
public:
    Rectangle(sf::Vector2f pos, sf::Color color, sf::Vector2f pos2);
    void draw(sf::RenderWindow& window) override;
    bool contains(sf::Vector2f p) const override;
    std::string serialize() const override;
protected:
    void onMove(sf::Vector2f delta) override; 
private:
    sf::Vector2f pos2;
};

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