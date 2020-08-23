#ifndef _CONF_H
#define _CONF_H

#include <string>
#include <map>

// The configuration is a simple mapping key->value, where
// the key is a string and the value is a string.
// The configuration file is a simple list, like so:
// KEY = VALUE
// the key is delimited from the value by a '=' character
// The line is disregarded if started with '#'

namespace mrobot {

namespace conf {

class Database {
    public:
        Database() = default;
        ~Database() = default;

        bool addFile(const std::string &filename);

        const std::string &get(const std::string &key);

    private:
        std::string &trimLeft(std::string &str);
        std::string &trimRight(std::string &str);

        bool parseKeyValue(const std::string &input,
                           std::string &key,
                           std::string &value);

        std::map<const std::string, const std::string> m_map;

        static const char *s_comment;
        static const char *s_delimiter;
};

} // namespace conf

} // namespace mrobot

#endif // _CONF_H
