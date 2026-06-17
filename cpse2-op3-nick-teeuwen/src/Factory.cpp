#include <Factory.hpp>

// Regex waardes
//  \( = (       (-?\d+) = een getal, eventueel negatief     \s*,\s* = een , met een Spatie    \) = )  
sf::Vector2f parsePoint(const std::string& s, int lineNum) {
    std::regex re(R"(\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\))");
    std::smatch match;

    if (!std::regex_search(s, match, re)) // kijken of het aan het format voldoet
        throw InvalidParameterError(lineNum, "Expected point like (x,y), got: '" + s + "'");

	return sf::Vector2f((float)std::stoi(match[1]), (float)std::stoi(match[2])); //Match 1 en 2 omzetten naar integers en vervolgens naar floats voor de Vector2f
}

std::unique_ptr<ScreenObject> createObject(
    const std::string& type,
    sf::Vector2f pos,
    const std::vector<std::string>& rest,
    int lineNum)
{
    if (type == "CIRCLE") {
        if (rest.size() < 2)
            throw MissingParameterError(lineNum, "CIRCLE needs color and diameter");
        sf::Color color = parseColor(rest[0], lineNum);
		float diam = parseDiameter(rest[1], lineNum);
        if (diam <= 0)
            throw InvalidParameterError(lineNum, "Diameter must be positive");
		return std::make_unique<Circle>(pos, color, diam); // gebruik make_unique om een nieuw object aan te maken van het type Circle en return deze als een unieke pointer naar een ScreenObject
    }
    else if (type == "RECTANGLE") {
        if (rest.size() < 2)
            throw MissingParameterError(lineNum, "RECTANGLE needs color and second point");
        sf::Color color = parseColor(rest[0], lineNum);
        sf::Vector2f pos2 = parsePoint(rest[1], lineNum);
        return std::make_unique<Rectangle>(pos, color, pos2);
    }
    else if (type == "LINE") {
        if (rest.size() < 2)
            throw MissingParameterError(lineNum, "LINE needs color and second point");
        sf::Color color = parseColor(rest[0], lineNum);
        sf::Vector2f pos2 = parsePoint(rest[1], lineNum);
        return std::make_unique<Line>(pos, color, pos2);
    }
    else if (type == "PICTURE") {
        if (rest.empty())
            throw MissingParameterError(lineNum, "PICTURE needs a filename");
        return std::make_unique<Picture>(pos, rest[0], lineNum);
    }
    else {
        throw UnknownTypeError(lineNum, type);
    }
}

std::vector<std::unique_ptr<ScreenObject>> loadFromFile(const std::string& filename, std::string& outErrorLog, bool& outHasError) {

    std::vector<std::unique_ptr<ScreenObject>> objects;
    std::ifstream file(filename);       // open bestand 

    try {
        if (!file.is_open())
            throw FileError("Bestand niet gevonden: " + filename);

        std::string line;
        int lineNum = 0;

        while (std::getline(file, line)) {  // Per regel bestand uitlezen
            ++lineNum;
            if (line.empty()) continue;     // Lege regels overslaan

            try {
                //Regex code: \([^)]*\) -> alles tussen ( en ) als één token  | -> of   \S+ -> een reeks tekens zonder spaties
                // Reftie https://en.cppreference.com/w/cpp/regex/regex_iterator.html

                std::vector<std::string> tokens;
                std::regex tokenRe(R"(\([^)]*\)|\S+)");
                auto begin = std::sregex_iterator(line.begin(), line.end(), tokenRe);
                auto end = std::sregex_iterator();
                for (auto it = begin; it != end; ++it)
                    tokens.push_back(it->str());

                if (tokens.size() < 2)
                    throw MissingParameterError(lineNum, "Incomplete line: '" + line + "'");

                sf::Vector2f pos = parsePoint(tokens[0], lineNum);
                std::string  type = tokens[1];
                std::vector<std::string> rest(tokens.begin() + 2, tokens.end());
                objects.push_back(createObject(type, pos, rest, lineNum));
            }
            catch (const ParseError& e) {
                outHasError = true;
                outErrorLog += std::string(e.what()) + "\n";
            }
            catch (const std::exception& e) {
                outHasError = true;
                outErrorLog += "Regel " + std::to_string(lineNum) + ": " + e.what() + "\n";
            }
        }

        // "Einde-file": als er na het inlezen geen enkel object overblijft
        if (objects.empty())
            throw EmptyFileError(filename);
    }
    catch (const FileError& e) {
        outHasError = true;
        outErrorLog = e.what();
    }
    catch (const EmptyFileError& e) {
        outHasError = true;
        outErrorLog += std::string(e.what()) + "\n";
    }

    return objects;
}

void saveToFile(const std::string& filename,
    const std::vector<std::unique_ptr<ScreenObject>>& objects)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw FileError("Cannot write to file: '" + filename + "'");

    for (const auto& obj : objects)
        file << obj->serialize() << "\n";

    if (!file)
        throw FileError("Write error while saving: '" + filename + "'");

    printf("Saved %zu objects to %s\n", objects.size(), filename.c_str());
}
