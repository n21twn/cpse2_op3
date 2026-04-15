#include "Line.hpp"

//----------------------------------LINE----------------------------------
Line::Line(sf::Vector2f pos, sf::Color color, sf::Vector2f pos2)
    : ScreenObject(pos, color), pos2(pos2) {
}

void Line::draw(sf::RenderWindow& window) {
    sf::Vertex vertices[2] = {
        sf::Vertex(sf::Vector2f(position.x, position.y), color),
        sf::Vertex(sf::Vector2f(pos2.x,     pos2.y),     color)
    };
    window.draw(vertices, 2, sf::Lines);

    if (selected) {
        float x = std::min(position.x, pos2.x);
        float y = std::min(position.y, pos2.y);
        float w = std::abs(pos2.x - position.x);
        float h = std::abs(pos2.y - position.y);
        drawSelectionBox(window, sf::FloatRect(x, y, w, h));
    }
}

bool Line::contains(sf::Vector2f p) const {
    float x1 = std::min(position.x, pos2.x) - 5.f;
    float y1 = std::min(position.y, pos2.y) - 5.f;
    float x2 = std::max(position.x, pos2.x) + 5.f;
    float y2 = std::max(position.y, pos2.y) + 5.f;
    return p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2; // Zelfde manier als een rectangle
}

std::string Line::serialize() const {
    std::ostringstream ss;
    ss << "(" << (int)position.x << "," << (int)position.y << ") LINE "
        << colorToString(color)
        << " (" << (int)pos2.x << "," << (int)pos2.y << ")";
    return ss.str();
}

void Line::onMove(sf::Vector2f delta) { pos2 += delta; } // Zorgt ervoor dat 2de punt ook meebeweegt
