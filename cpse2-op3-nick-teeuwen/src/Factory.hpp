#pragma once
#include "Shape.hpp"
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

std::vector<std::unique_ptr<ScreenObject>> loadFromFile(const std::string& filename);

void saveToFile(const std::string& filename,
    const std::vector<std::unique_ptr<ScreenObject>>& objects);