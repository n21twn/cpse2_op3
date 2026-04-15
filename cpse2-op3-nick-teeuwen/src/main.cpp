#include <SFML/Graphics.hpp>
#include "Factory.hpp"
#include <iostream>
#include <vector>
#include <string>

// Relatief pad: factory.txt staat naast de executable (gekopieerd door CMake)
const std::string FILENAME = "../factory.txt";  
const unsigned    WIN_W    = 1280;
const unsigned    WIN_H    = 960; 


void showError(sf::RenderWindow& window, const sf::Font& font,
                      const std::string& message)
{
    window.clear(sf::Color(30, 30, 30));

    sf::Text title;
    title.setFont(font);
    title.setString("Error loading factory.txt");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::Red);
    title.setPosition(40.f, 40.f);
    window.draw(title);

    sf::Text msg;
    msg.setFont(font); 
    msg.setString(message);
    msg.setCharacterSize(20);
    msg.setFillColor(sf::Color::White);
    msg.setPosition(40.f, 100.f);
    window.draw(msg);

    sf::Text hint;
    hint.setFont(font);
    hint.setString("Fix factory.txt and restart the application.");
    hint.setCharacterSize(18);
    hint.setFillColor(sf::Color(180, 180, 180));
    hint.setPosition(40.f, (float)WIN_H - 60.f);
    window.draw(hint);

    window.display();
}

int main() {
    // SFML 2: VideoMode neemt losse width/height ints
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Factory Object Viewer");
    window.setFramerateLimit(60);

    // Font laden via relatief pad; kopieer arial.ttf naast de exe
    // of laat CMake het kopieren vanuit de assets/ map
    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf");
    if (!fontLoaded)
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");   // Windows fallback
    if (!fontLoaded)
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");

    std::vector<std::unique_ptr<ScreenObject>> objects;
    bool fileError    = false;
    std::string errorMessage;

    try {
        objects = loadFromFile(FILENAME);
    }
    catch (const std::exception& e) {
        fileError = true;
        errorMessage = e.what();
        std::cerr << "ERROR: " << errorMessage << "\n";
    }

	ScreenObject* dragging = nullptr; //pointer naar huidige object dat gesleept wordt, nullptr als er geen object gesleept wordt
    sf::Vector2f  dragOffset = {};
    bool          isDragging = false;

    // Event loop 
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {

            // window sluiten
            if (event.type == sf::Event::Closed) {
                if (!fileError) {
                    try {
                        saveToFile(FILENAME, objects);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Save error: " << e.what() << "\n";
                    }
                }
                window.close();
            }

            // Muis ingedrukt = selecteer
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left && !fileError)
            {
                sf::Vector2f mousePos((float)event.mouseButton.x,
                                      (float)event.mouseButton.y);

                ScreenObject* hit = nullptr; //pointer naar huidige object dat geselecteerd wordt, nullptr als er geen object geselecteerd wordt
				for (auto& obj : objects) // referentie gebruiken zodat we de objecten kunnen aanpassen (selecteren) zonder ze te hoeven kopieren
                    if (obj->contains(mousePos)) // obj -> pakt functie van het object.
						hit = obj.get(); // als er meerdere objecten op dezelfde plek liggen, wordt het laatste object in de lijst geselecteerd 

                if (hit) {
                    for (auto& obj : objects) 
                        obj->setSelected(false);

                    hit->setSelected(true);
                    dragging   = hit;
                    dragOffset = mousePos - hit->getPosition();
                    isDragging = true;
                }
                else {
                    for (auto& obj : objects) obj->setSelected(false);
                }
            }

            // Muis losgelaten: stoppen met slepen
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                isDragging = false;
                dragging   = nullptr;
            }

            //Muis bewogen: object verplaatsen 
            if (event.type == sf::Event::MouseMoved && isDragging && dragging) {
                sf::Vector2f newPos((float)event.mouseMove.x - dragOffset.x,
                                    (float)event.mouseMove.y - dragOffset.y);

				newPos.x = std::max(0.f, std::min(newPos.x, (float)WIN_W)); //begren dat het object niet buiten het venster kan worden gesleept
                newPos.y = std::max(0.f, std::min(newPos.y, (float)WIN_H));

				dragging->move(newPos - dragging->getPosition()); // berekend verschil tussen nieuwe positie en huidige positie en verplaats het object met dat verschil (move functie van het object)
            }

            //Toetsenbord: Escape deselecteert alles 
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
            {
                for (auto& obj : objects) obj->setSelected(false);
                isDragging = false;
                dragging   = nullptr;
            }
        }

        window.clear(sf::Color(50, 50, 50));

        if (fileError) {
            if (fontLoaded)
                showError(window, font, errorMessage);
            else
                window.clear(sf::Color::Red);
        }
        else {
            for (const auto& obj : objects)
                obj->draw(window);
        }

        window.display();
    }

    return 0;
}
