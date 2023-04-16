#ifndef _JSON_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <list>
#include <concepts>
#include <exception>

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

    JValue(int64_t value) : t_{type::Number}, nt_{number_type::Interger}, i_{value} {}

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
    T& Value();

    template<typename T>
    typename std::enable_if_t<std::is_integral<T>::value, T>& Value() {
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
            throw std::runtime_error("Expect type number of bool, but get " + TypeString(t_));
        }
    }

    template<>
    std::string& Value<std::string>() {
        if (t_ != type::String) {
            throw std::runtime_error("Expect type string but get " + TypeString(t_));
        }
        return s_;
    }

    template<>
    bool& Value<bool>() {
        if (t_ != type::Bool) {
            throw std::runtime_error("Expect type bool but get " + TypeString(t_));
        }
        return b_;
    }

    std::vector<JValue>& Array() {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " + TypeString(t_));
        }
        return a_;
    }

    std::unordered_map<std::string, JValue>& Object() {
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

    JValue& operator[](const std::string& s) {
        if (t_ != type::Object) {
            throw std::runtime_error("Expect type object but get " +
                                     TypeString(t_));
        }
        return o_[s];
    }

private:
    type t_;
    number_type nt_;
    std::string s_;
    int64_t i_;
    double d_;
    bool b_;
    std::unordered_map<std::string, JValue> o_;
    std::vector<JValue> a_;
};
} // namespace json

#define _JSON_H
#endif
