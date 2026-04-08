#include "Shape.hpp"

//----------------------------------CIRCLE----------------------------------
Circle::Circle(sf::Vector2f pos, sf::Color color, float diameter)
    : ScreenObject(pos, color), diameter(diameter) {
}

void Circle::draw(sf::RenderWindow& window) {
    float r = diameter / 2.f;
    sf::CircleShape shape(r);
    shape.setPosition(position.x - r, position.y - r);
    shape.setFillColor(color);
    window.draw(shape);
    if (selected) // Teken selectiebox als de object geselecteerd is
        drawSelectionBox(window,
            sf::FloatRect(position.x - r, position.y - r, diameter, diameter));
}

bool Circle::contains(sf::Vector2f p) const {
    float r = diameter / 2.f; // Straal van de cirkel
    float dx = p.x - position.x; // Afstand in x-richting van de muis tot het midden van de cirkel
    float dy = p.y - position.y; // Afstand in y-richting van de muis tot het midden van de cirkel
    return (dx * dx + dy * dy) <= (r * r); // Checkt door stelling van pythagoras wat de afstand is van de klik tot het midden van de cirkel
    // Kijkt vervolgens of de straal van de cirkel groter is dan deze afstand, zo ja dan zit de klik binnen de cirkel
}

std::string Circle::serialize() const { // zet de waardes van de cirkel in een string volgens de factory.txt format
    std::ostringstream ss;
    ss << "(" << (int)position.x << "," << (int)position.y << ") CIRCLE "
        << colorToString(color) << " " << (int)diameter;
    return ss.str();
}

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

//----------------------------------PICTURE----------------------------------
Picture::Picture(sf::Vector2f pos, const std::string& filename, int lineNum)
    : ScreenObject(pos, sf::Color::White), filename(filename)
{
    if (!texture.loadFromFile(filename))
        throw ParseError(lineNum, "Cannot load image file: '" + filename + "'");
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Picture::draw(sf::RenderWindow& window) {
    sprite.setPosition(position);
    window.draw(sprite);
    if (selected)
        drawSelectionBox(window, sprite.getGlobalBounds());
}

bool Picture::contains(sf::Vector2f p) const {
    return sprite.getGlobalBounds().contains(p); // getGlobalBounds() geeft een FloatRect terug.
}

std::string Picture::serialize() const {
    std::ostringstream ss;
    ss << "(" << (int)position.x << "," << (int)position.y << ") PICTURE " << filename;
    return ss.str();
}