#include <Picture.hpp>

//----------------------------------PICTURE----------------------------------
Picture::Picture(sf::Vector2f pos, const std::string& filename, int lineNum)
    : ScreenObject(pos, sf::Color::White), filename(filename)
{
	std::string path = "assets/" + filename;  // Voeg "../assets/" toe aan het begin van de bestandsnaam om in de behorend map te komen
    if (!texture.loadFromFile(path))
        throw ParseError(lineNum, "Cannot load image file: '" + path + "'");
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