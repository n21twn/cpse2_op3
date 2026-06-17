#include <SFML/Graphics.hpp>
#include <Factory.hpp>
#include <iostream>
#include <vector>
#include <string>
//#include "factory.txt"


const std::string FILENAME = "factory.txt"; 
const unsigned    WIN_W = 1280; // Eigen beeld scherm is 2560x1440 
const unsigned    WIN_H = 960; // test voorbeeld is 1280x720 


int main() {
    // SFML 2: VideoMode neemt losse width/height ints
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Factory Object Viewer");
    window.setFramerateLimit(60);

    // Font laden via relatief pad; kopieer arial.ttf naast de exe
    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf");
    if (!fontLoaded)
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");   // Windows fallback


    std::string errorMessage = "";
    bool fileError = false;
	std::vector<std::unique_ptr<ScreenObject>> objects = loadFromFile(FILENAME, errorMessage, fileError); //Newwwww
   /* try {
        objects = loadFromFile(FILENAME);
    }
    catch (const std::exception& e) {
        fileError = true;
        errorMessage = e.what();
        std::cerr << "ERROR: " << errorMessage << "\n";
    }*/

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
                    catch (const FileError& e) {
                        std::cerr << "ERROR: " << e.what() << "\n";
                        // Bestand kon niet opgeslagen worden; sluit alsnog af,
                        // de eerdere factory.txt blijft daardoor onveranderd.
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

			sf::RectangleShape errorBg(sf::Vector2f(float(window.getSize().x), float(window.getSize().y)));
            errorBg.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(errorBg);
            showError(window, font, errorMessage); //Newwwww
        }else {
            for (const auto& obj : objects)
                obj->draw(window);
        }
        
        window.display();
    }

    return 0;
}
