#include "Factory.hpp"

// Regex waardes
//  \( = (       (-?\d+) = een getal, eventueel negatief     \s*,\s* = een , met een Spatie    \) = )  
sf::Vector2f parsePoint(const std::string& s, int lineNum) {
    std::regex re(R"(\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\))");
    std::smatch match;

    if (!std::regex_search(s, match, re)) // kijken of het aan het format voldoet
        throw ParseError(lineNum, "Expected point like (x,y), got: '" + s + "'");

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
            throw ParseError(lineNum, "CIRCLE needs color and diameter");
        sf::Color color = parseColor(rest[0], lineNum);
        float diam;
        try { diam = std::stof(rest[1]); } // convert string to float 
        catch (...) { // vang alle exception op 
            throw ParseError(lineNum, "Invalid diameter: '" + rest[1] + "'"); }
        if (diam <= 0)
            throw ParseError(lineNum, "Diameter must be positive");
		return std::make_unique<Circle>(pos, color, diam); // gebruik make_unique om een nieuw object aan te maken van het type Circle en return deze als een unieke pointer naar een ScreenObject
    }
    else if (type == "RECTANGLE") {
        if (rest.size() < 2)
            throw ParseError(lineNum, "RECTANGLE needs color and second point");
        sf::Color color = parseColor(rest[0], lineNum);
        sf::Vector2f pos2 = parsePoint(rest[1], lineNum);
        return std::make_unique<Rectangle>(pos, color, pos2);
    }
    else if (type == "LINE") {
        if (rest.size() < 2)
            throw ParseError(lineNum, "LINE needs color and second point");
        sf::Color color = parseColor(rest[0], lineNum);
        sf::Vector2f pos2 = parsePoint(rest[1], lineNum);
        return std::make_unique<Line>(pos, color, pos2);
    }
    else if (type == "PICTURE") {
        if (rest.empty())
            throw ParseError(lineNum, "PICTURE needs a filename");
        return std::make_unique<Picture>(pos, rest[0], lineNum);
    }
    else {
        throw ParseError(lineNum, "Unknown object type: '" + type + "'");
    }
}

std::vector<std::unique_ptr<ScreenObject>> loadFromFile(const std::string& filename) {
    std::ifstream file(filename);       // open bestand 
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: '" + filename + "'");

    std::vector<std::unique_ptr<ScreenObject>> objects;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {  // Per regel bestand uitlezen
        ++lineNum;
        if (line.empty()) continue;     // Legen regels overslaan

        //Regex code: \([^)]*\) -> alles tussen ( en ) als één token  | -> of   \S+ -> een reeks tekens zonder spaties
        // Reftie https://en.cppreference.com/w/cpp/regex/regex_iterator.html

        std::vector<std::string> tokens;
        std::regex tokenRe(R"(\([^)]*\)|\S+)");
        auto begin = std::sregex_iterator(line.begin(), line.end(), tokenRe);   // iterat door de matches van de regex
        auto end = std::sregex_iterator();                                      // eind van de matches en dus leeg
        for (auto it = begin; it != end; ++it)                                  // Loop door de matches en voeg ze toe aan variable tot gelijk is aan end
            tokens.push_back(it->str());

        if (tokens.size() < 2)
            throw ParseError(lineNum, "Incomplete line: '" + line + "'");

        sf::Vector2f pos = parsePoint(tokens[0], lineNum);
        std::string  type = tokens[1];
        std::vector<std::string> rest(tokens.begin() + 2, tokens.end());        // Tokens van af index 2 tot eind in een nieuwe vector zetten
        objects.push_back(createObject(type, pos, rest, lineNum));              // Push waardes van createObject in de vector van objects zo dat deze gefilterd en getekend kunne worden
    }

    if (file.bad())
        throw std::runtime_error("Read error on file: '" + filename + "'");

    return objects;
}

void saveToFile(const std::string& filename,
    const std::vector<std::unique_ptr<ScreenObject>>& objects)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot write to file: '" + filename + "'");
    for (const auto& obj : objects)
        file << obj->serialize() << "\n";
}