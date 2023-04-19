#ifndef _JSON_H

#include <string>
#include <iostream>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <list>
#include <concepts>
#include <exception>

inline void output_spaces(std::ostream& os, int space_width, int space_number) {
    for (int i=0; i<space_width * space_number; ++i) {
        os << ' ';
    }
}

namespace json {
class JValue {
public:
    enum class type : char {
        String,
        Number,
        Bool,
        Array,
        Object,
        Unknown,
    };

    enum class number_type : char {
        Interger,
        Float,
        Unknown,
    };

    static std::string TypeString(type tp) {
        switch (tp) {
        case type::String:
            return "String";
        case type::Number:
            return "Number";
        case type::Bool:
            return "Bool";
        case type::Array:
            return "Array";
        case type::Object:
            return "Object";
        default:
            return "Unknown";
        }
    }

    static std::string NumberTypeString(number_type nt) {
        switch (nt) {
        case number_type::Interger:
            return "Interger";
        default:
            return "Float";
        }
    }

    JValue(type tp = type::Unknown) : t_{tp} {}

    JValue(std::string_view sv) : t_{type::String}, s_(sv) {}

    JValue(const char* s) : t_{type::String}, s_(s) {}

    JValue(int64_t value) : t_{type::Number}, nt_{number_type::Interger}, i_{value} {}
    JValue(int32_t value) : t_{type::Number}, nt_{number_type::Interger}, i_{value} {}

    JValue(double value) : t_{type::Number}, nt_{number_type::Float}, d_{value} {}

    JValue(bool flag) : t_{type::Bool}, b_{flag} {}

    JValue(const std::initializer_list<JValue>& il) : t_{type::Array}, a_(il) {}

    template<typename ForwardIter> JValue(ForwardIter first, ForwardIter last) : t_{type::Array} {
        for (; first != last; ++first) {
            a_.push_back(*first);
        }
    }

    JValue(std::unordered_map<std::string, JValue> obj) : t_{type::Object}, o_(obj) {}

    JValue(const std::unordered_map<std::string, JValue>& map) : t_{type::Object}, o_(map) {}

    JValue(const JValue& other) : t_{other.t_}, nt_{other.nt_}, s_{other.s_}, i_{other.i_}, d_{other.d_}, b_{other.b_}, o_{other.o_}, a_{other.a_} {}

    JValue(JValue&& other) : t_{other.t_}, nt_{other.nt_}, s_{std::move(other.s_)}, i_{other.i_}, d_{other.d_}, b_{other.b_}, o_{std::move(other.o_)}, a_{std::move(other.a_)} {}

    JValue& operator=(const JValue& other) {
        t_ = other.t_;
        nt_ = other.nt_;
        s_ = other.s_;
        i_ = other.i_;
        d_ = other.d_;
        b_ = other.b_;
        o_ = other.o_;
        a_ = other.a_;
        return *this;
    }

    JValue& operator=(JValue&& other) {
        t_ = other.t_;
        nt_ = other.nt_;
        s_ = std::move(other.s_);
        i_ = other.i_;
        d_ = other.d_;
        b_ = other.b_;
        o_ = std::move(other.o_);
        a_ = std::move(other.a_);
        return *this;
    }

    template<typename T>
    const T& Value() const {
        return ValueImpl<T>();
    }

    template<>
    const double& Value<double>() const {
        return d_;
    }

    template<>
    const std::string& Value<std::string>() const {
        if (t_ != type::String) {
            throw std::runtime_error("Expect type string but get " + TypeString(t_));
        }
        return s_;
    }

    const std::vector<JValue>& Array() const {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " +
                                     TypeString(t_));
        }
        return a_;
    }

    const std::unordered_map<std::string, JValue>& Object() const {
        if (t_ != type::Object) {
            throw std::runtime_error("Expect type object but get " + TypeString(t_));
        }
        return o_;
    }

    JValue& operator[](size_t index) {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " + TypeString(t_));
        }
        return a_[index];
    }

    const JValue& operator[](size_t index) const {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " +
                                     TypeString(t_));
        }
        return a_[index];
    }

    JValue& operator[](const std::string& s) {
        if (t_ != type::Object) {
            throw std::runtime_error("Expect type object but get " +
                                     TypeString(t_));
        }
        return o_[s];
    }

    static void Serialize(std::ostream &os, const JValue &jvalue, bool indent = true, int indentWidth = 2, int depth = 0) {
        if (depth > MaxDepth) {
            throw std::runtime_error("json depth " + std::to_string(depth) + " exceeds the max depth " + std::to_string(MaxDepth));
        }
        bool nonFirst = false;
        switch (jvalue.t_) {
        case type::String:
            os << "\"" + jvalue.Value<std::string>() + "\"";
            break;
        case type::Number:
            if (jvalue.nt_ == number_type::Float) {
                os << jvalue.Value<double>();
            } else {
                os << jvalue.Value<int64_t>();
            }
            break;
        case type::Bool:
            os << jvalue.Value<bool>();
            break;
        case type::Array:
            os << (indent ? "[\n" : "[");
            for (const auto& item: jvalue.Array()) {
                if (nonFirst) {
                    os << (indent ? ",\n" : ",");
                }
                nonFirst = true;
                if (indent) output_spaces(os, indentWidth, depth + 1);
                Serialize(os, item, indent, indentWidth, depth + 1);
            }
            if (indent) {
                os << "\n";
                output_spaces(os, indentWidth, depth);
            }
            os << "]";
            break;
        case type::Object:
            os << (indent ? "{\n" : "{");
            for (auto & kvp : jvalue.Object()) {
                if (nonFirst) {
                    os << (indent ? ",\n" : ",");
                }
                nonFirst = true;
                if (indent) output_spaces(os, indentWidth, depth + 1);
                os << (indent ? "\"" + kvp.first + "\": " : "\"" + kvp.first + "\":");
                Serialize(os, kvp.second, indent, indentWidth, depth + 1);
            }
            if (indent) {
                os << "\n";
                output_spaces(os, indentWidth, depth);
            }
            os << "}";
            break;
        default:
            throw std::runtime_error("Unknown/unsupported jvalue type " + TypeString(jvalue.t_));
        }
    }

    static std::string Serialize(const JValue& jvalue, bool indent = true, int indentWidth = 2, int depth = 0) {
        std::ostringstream ss;
        Serialize(ss, jvalue, indent, indentWidth, depth);
        return ss.str();
    }

private:
    template <typename T>
    const typename std::enable_if_t<std::is_integral<T>::value, T>& ValueImpl() const {
        switch (t_) {
        case type::Bool:
            return b_;
        case type::Number:
            if (nt_ == number_type::Float) {
                return d_;
            } else {
                return i_;
            }
        default:
            throw std::runtime_error("Expect type number or bool, but get " +
                                     TypeString(t_));
        }
    }

    type t_;
    number_type nt_{number_type::Unknown};
    std::string s_;
    int64_t i_;
    double d_;
    bool b_;
    std::unordered_map<std::string, JValue> o_;
    std::vector<JValue> a_;
    const static int MaxDepth = 1000;
};
} // namespace json

#define _JSON_H
#endif
