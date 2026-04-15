#pragma once
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Line.hpp"
#include "Picture.hpp"
#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <regex>
#include <string>

sf::Vector2f parsePoint(const std::string& s, int lineNum);

std::unique_ptr<ScreenObject> createObject(
    const std::string& type,
    sf::Vector2f pos,
    const std::vector<std::string>& rest,
    int lineNum
);


std::vector<std::unique_ptr<ScreenObject>> loadFromFile( //Newwwww
    const std::string& filename,
    std::string& outErrorLog,
    bool& outHasError
);


void saveToFile( //Newwwww
    const std::string& filename,
    const std::vector<std::unique_ptr<ScreenObject>>& objects
);