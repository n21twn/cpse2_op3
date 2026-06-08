#include <Circle.hpp>

//----------------------------------CIRCLE----------------------------------
Circle::Circle(sf::Vector2f pos, sf::Color color, float diameter)
    : ScreenObject(pos, color), diameter(diameter) {
}

void Circle::draw(sf::RenderWindow& window) {
    float r = diameter / 2.f; // r is straal 
    sf::CircleShape shape(r);
    shape.setPosition(position.x , position.y ); // gebruik -r om van uit midden punt teteken.
    shape.setFillColor(color);
    window.draw(shape);
    if (selected) // Teken selectiebox als de object geselecteerd is
        drawSelectionBox(window,
            sf::FloatRect(position.x , position.y , diameter, diameter)); // voeg -r toe om van uit midden te pakken
}

bool Circle::contains(sf::Vector2f p) const {
    float r = diameter / 2.f; // Straal van de cirkel
    float centerX = position.x + r; // alleen toevoegen als je van af boven hoek tekent
    float centerY = position.y + r;
    float dx = p.x - centerX; // Afstand in x-richting van de muis tot het midden van de cirkel
    float dy = p.y - centerY; // Afstand in y-richting van de muis tot het midden van de cirkel
    return (dx * dx + dy * dy) <= (r * r); // Checkt door stelling van pythagoras wat de afstand is van de klik tot het midden van de cirkel
    // Kijkt vervolgens of de straal van de cirkel groter is dan deze afstand, zo ja dan zit de klik binnen de cirkel
}

std::string Circle::serialize() const { // zet de waardes van de cirkel in een string volgens de factory.txt format
    std::ostringstream ss;
    ss << "(" << (int)position.x << "," << (int)position.y << ") CIRCLE "
        << colorToString(color) << " " << (int)diameter;
    return ss.str();
}