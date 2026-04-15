#include "Rectangle.hpp"

//----------------------------------RECTANGLE----------------------------------
Rectangle::Rectangle(sf::Vector2f pos, sf::Color color, sf::Vector2f pos2)
    : ScreenObject(pos, color), pos2(pos2) {
}

void Rectangle::draw(sf::RenderWindow& window) {
    float x = std::min(position.x, pos2.x);
    float y = std::min(position.y, pos2.y);
    float w = std::abs(pos2.x - position.x);
    float h = std::abs(pos2.y - position.y);
    sf::RectangleShape shape(sf::Vector2f(w, h));
    shape.setPosition(x, y);
    shape.setFillColor(color);
    window.draw(shape);
    if (selected)
        drawSelectionBox(window, sf::FloatRect(x, y, w, h));
}

bool Rectangle::contains(sf::Vector2f p) const {
    float x1 = std::min(position.x, pos2.x);
    float y1 = std::min(position.y, pos2.y);
    float x2 = std::max(position.x, pos2.x);
    float y2 = std::max(position.y, pos2.y);
    return p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2; // kijkt of de klik binnen de laagste x en y en de hoogste x en y ligt.
}

std::string Rectangle::serialize() const {
    std::ostringstream ss;
    ss << "(" << (int)position.x << "," << (int)position.y << ") RECTANGLE "
        << colorToString(color)
        << " (" << (int)pos2.x << "," << (int)pos2.y << ")";
    return ss.str();
}

void Rectangle::onMove(sf::Vector2f delta) { pos2 += delta; } // Zorgt ervoor dat 2de punt ook meebeweegt
