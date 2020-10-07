#include "conf.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>

namespace mrobot {

namespace conf {

const char *Database::s_comment = "#";
const char *Database::s_delimiter = "=";

bool Database::addFile(const std::string &filename) {
    std::ifstream ifile;

    ifile.open(filename);
    if (!ifile.is_open()) {
        std::cout << "Error opening configuration file " << filename << '\n';
        return false;
    }

    bool res = true;
    uint32_t count = 0;
    while (!ifile.eof()) {
        std::string line;
        std::getline(ifile, line);

        trimLeft(line);
        trimRight(line);

        // Ignoring commented lines
        if (line.empty() || line.find(s_comment) == 0) {
            continue;
        }

        std::string key, value;
        if (!Database::parseKeyValue(line, key, value)) {
            std::cout << "Erron in line #" << std::to_string(count) << '\n';
            res = false;
            continue;
        }

        if (m_map.find(key) != m_map.end()) {
            std::cout << "key \"" << key << "\" in line #" << std::to_string(count) <<
                " already exists\n";
            res = false;
            continue;
        }

        m_map.insert(std::pair(key, value));

        count++;
    }

    return res;
}

const std::string &Database::get(const std::string &key) {
    static const std::string empty;

    auto it = m_map.find(key);
    if (it == m_map.end()) {
        return empty;
    }

    return it->second;
}

std::string &Database::trimLeft(std::string &str) {
    auto firstNonWs = std::find_if(str.begin(), str.end(), [](char c) -> bool {
        return std::isspace(c) == 0;
    });

    str.erase(str.begin(), firstNonWs);

    return str;
}

std::string &Database::trimRight(std::string &str) {
    auto lastNonWs = std::find_if(str.rbegin(), str.rend(), [](char c) -> bool {
        return std::isspace(c) == 0;
    });

    str.erase(lastNonWs.base(), str.end());

    return str;
}

bool Database::parseKeyValue(const std::string &input,
                             std::string &key,
                             std::string &value) {
    auto idx = input.find(s_delimiter);
    if (idx == std::string::npos) {
        std::cout << "No delimiter in \"" << input << "\"\n";
        return false;
    }

    key = input.substr(0, (idx > 0) ? idx : 0);
    trimRight(key);
    if (key.empty()) {
        std::cout << "No key in \"" << input<< "\"\n";
        return false;
    }

    value = input.substr(idx+1, input.length());
    trimLeft(value);
    if (value.empty()) {
        std::cout << "No value in \"" << input<< "\"\n";
        return false;
    }

    return true;
}

} // namespace conf

} // namespace mrobot
