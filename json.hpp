#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <initializer_list>
#include <list>
#include <exception>
#include <algorithm>

static inline void output_spaces(std::ostream &os, int space_width, int space_number) {
    for (int i = 0; i < space_width * space_number; ++i) {
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

    JValue(const std::string& s) : t_(type::String), s_(s) {}

    JValue(const char* s) : t_{type::String}, s_(s) {}

    JValue(const char* s, int cout) : JValue(std::string(s, cout)) {}

    JValue(int64_t value) : t_{type::Number}, nt_{number_type::Interger}, i_{value} {}

    JValue(int32_t value) : t_{type::Number}, nt_{number_type::Interger}, i_{value} {}

    JValue(double value) : t_{type::Number}, nt_{number_type::Float}, d_{value} {}

    JValue(bool flag) : t_{type::Bool}, b_{flag} {}

    JValue(const std::initializer_list<JValue>& il) : t_{type::Array}, a_(il) {}

    operator bool() const {
        return t_ != type::Unknown;
    }

    template<typename ForwardIter> JValue(ForwardIter first, ForwardIter last) : t_{type::Array} {
        for (; first != last; ++first) {
            a_.push_back(*first);
        }
    }

    JValue(const std::map<std::string, JValue>& map) : t_{type::Object}, o_(map) {}

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
    const T Value() const {
        if constexpr (std::is_integral<T>::value) {
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
        if constexpr (std::is_same_v<T, std::string>) {
            return s_;
        }
        if constexpr (std::is_same_v<T, double>) {
            return d_;
        }

        throw std::runtime_error("Unsported template type");
    }

    const std::vector<JValue>& Array() const {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " +
                                     TypeString(t_));
        }
        return a_;
    }

    std::vector<JValue>& Array() {
        if (t_ != type::Array) {
            throw std::runtime_error("Expect type array but get " +
                                     TypeString(t_));
        }
        return a_;
    }

    const std::map<std::string, JValue>& Object() const {
        if (t_ != type::Object) {
            throw std::runtime_error("Expect type object but get " + TypeString(t_));
        }
        return o_;
    }

    std::map<std::string, JValue>& Object() {
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

    JValue& operator[](const char* s) {
        return operator[](std::string{s});
    }

    bool Has(const std::string& sv) {
        if (t_ != type::Object) {
            throw std::runtime_error(
                "Expect type object to invoke Has method, but get type " +
                TypeString(t_));
        }
        return o_.find(sv) != o_.end();
    }

    void SetError() {
        t_ = type::Unknown;
        nt_ = number_type::Unknown;
        o_.clear();
        a_.clear();
    }

    type Type() const {
        return t_;
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
            os << (jvalue.Value<bool>() ? "true" : "false");
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

    static JValue Deserialize(std::string const& sv) {
        struct Parser
        {
            Parser(std::string const& sv) : data_(sv), pos_(0) {}

            json::JValue Parse() {
                ConsumeSpaces();
                if (pos_ >= data_.size()) {
                    return {};
                }
                if (data_[pos_] == '{') {
                    return ParseObject();
                } else if (data_[pos_] == '[') {
                    return ParseArray();
                } else if (data_[pos_] == '\"') {
                    return ParseString();
                } else if (data_[pos_] == 't') {
                    if (data_.substr(pos_, 4) == "true") {
                        pos_ += 4;
                        return {true};
                    } else {
                        return {};
                    }
                } else if (data_[pos_] == 'f') {
                    if (data_.substr(pos_, 5) == "false") {
                        pos_ += 5;
                        return {false};
                    } else {
                        return {};
                    }
                } else {
                    return ParseNumber();
                }
            }

            JValue ParseString() {
                if (!Consume('\"')) {
                    return {};
                }
                int start = pos_;
                while (1) {
                    if (data_[pos_] == '\"') {
                        ++pos_;
                        std::string ret = data_.substr(start, pos_ - 1 - start);
                        return ret;
                    } else {
                        ++pos_;
                        if (pos_ >= data_.size()) {
                            return {};
                        }
                    }
                }
                return {};
            }

            JValue ParseArray() {
                JValue ret(JValue::type::Array);
                ConsumeSpaces();
                if (!Consume('[')) {
                    return {};
                }
                if (Consume(']')) {
                    return ret;
                }

                while (1) {
                    ConsumeSpaces();
                    if (pos_ >= data_.size()) {
                        ret.SetError();
                        break;
                    }

                    JValue item = Parse();
                    if (item) {
                        ret.Array().push_back(item);
                    } else {
                        return {};
                    }

                    if (data_[pos_] == ']') {
                        ++pos_;
                        break;
                    }

                    if (!Consume(',')) {
                        return {};
                    }
                    ConsumeSpaces();
                }
                return ret;
            }

            JValue ParseObject() {
                JValue ret(JValue::type::Object);
                ConsumeSpaces();
                if (!Consume('{')) {
                    return {};
                }
                ConsumeSpaces();
                if (Consume('}')) {
                    ++pos_;
                    return ret;
                }

                while (1) {
                    ConsumeSpaces();
                    if (pos_ >= data_.size()) {
                        return {};
                    }
                    JValue keyValue = ParseString();
                    ConsumeSpaces();
                    if (!keyValue) {
                        return {};
                    }
                    if (!Consume(':')) {
                        return {};
                    }
                    JValue v = Parse();
                    ConsumeSpaces();
                    if (!v) {
                        return {};
                    }
                    std::string key = keyValue.Value<std::string>();
                    ret[key] = v;

                    if (data_[pos_] == '}') {
                        ++pos_;
                        break;
                    }
                    if (!Consume(',')) {
                        return {};
                    }
                    ConsumeSpaces();
                }
                return ret;
            }

            JValue ParseNumber() {
                enum State {
                Start,
                Minus,
                ZeroFirst,
                Digits,
                DigitsAfterMinus,
                DigitsAfterPoint,
                Point,
                Invliad,
                } state{Start};
                size_t start = pos_;
                while (state != State::Invliad) {
                    switch (data_[pos_]) {
                    case '-':
                        if (state == State::Start) {
                            state = State::Minus;
                        } else {
                            state = State::Invliad;
                        }
                        break;
                    case '0':
                        if (state == State::Start) {
                            state = State::ZeroFirst;
                        } else if (state == State::ZeroFirst || state == State::Digits || state == State::DigitsAfterMinus || state == State::DigitsAfterPoint) {
                            // do nothing
                        } else if (state == State::Point) {
                            state = State::DigitsAfterPoint;
                        } else {
                            state = State::Invliad;
                        }
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        if (state == State::Start || state == State::Minus || state == State::Point) {
                            state = State::Digits;
                        } else if (state == State::ZeroFirst) {
                            start = pos_;
                            state = State::Digits;
                        } else if (state == State::Digits || state == State::DigitsAfterMinus || state == State::DigitsAfterPoint) {
                            // do nothing
                        } else {
                            state = State::Invliad;
                        }
                        break;
                    case '.':
                        if (state == State::Start || state == State::ZeroFirst || state == State::Minus || state == State::Digits || state == State::DigitsAfterMinus) {
                            state = State::Point;
                        } else if (state == State::ZeroFirst) {
                            state = State::Point;
                            start = pos_;
                        } else {
                            state = State::Invliad;
                        }
                        break;
                    default:
                        if (state == State::DigitsAfterPoint || state == State::Point) {
                            return JValue(data_.substr(start, pos_ - start));
                        } else {
                            return JValue(data_.substr(start, pos_ - start));
                        }
                    }
                    ++pos_;
                }
                return {};
            }

            void ConsumeSpaces() {
                for (; pos_ < data_.size() && (data_[pos_] == ' ' || data_[pos_] == '\t' || data_[pos_] == '\n' || data_[pos_] == '\r'); ++pos_);
            }

            bool Consume(const char c) {
                if (pos_ >= data_.size() || data_[pos_] != c) {
                    return false;
                }
                ++pos_;
                return true;
            }

            std::string const& data_;
            size_t pos_;
        };

        Parser parser(sv);
        return parser.Parse();
    }

    static JValue Deserialize(const char* s) {
        return Deserialize(std::string(s));
    }

private:
    type t_;
    number_type nt_;
    std::string s_;
    int64_t i_;
    double d_;
    bool b_;
    std::map<std::string, JValue> o_;
    std::vector<JValue> a_;
    const static int MaxDepth = 1000;
};

} // namespace json
