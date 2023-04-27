#pragma once

#include <string>
#include <istream>
#include "json.hpp"

namespace json {
class JsonBinder {
protected:
    virtual void FromJson(JValue const& obj) = 0;
    virtual JValue ToJson() const = 0;
public:
    void Deserialize(const char* s) {
        Deserialize(std::string(s));
    }

    void Deserialize(std::istream& is) {
        std::string str, s;
        while (is >> s) {
            str += s;
        }
        Deserialize(str);
    }

    void Deserialize(std::string const &s) {
        JValue obj = JValue::Deserialize(s);
        FromJson(obj);
    }

    std::string Serialize() const {
        JValue obj = ToJson();
        return JValue::Serialize(obj);
    }
};
} // namespace json
