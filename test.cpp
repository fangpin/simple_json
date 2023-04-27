#include <iostream>
#include <map>
#include <string>
#include "json/json.hpp"
#include "json/struct_bind.hpp"

using namespace std;
using namespace json;

void basic_example() {
    cout << "**************basic_example********************" << endl;

    JValue person = map<string, JValue> {
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"flag", true},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         map<string, JValue>{
             {"name", "Jun"}, {"sports", {"tennis", "dance"}}, {"age", 19}}}};

    // serialize
    cout << JValue::Serialize(person) << endl;
    cout << endl;
}

void update_example() {
    cout << "**************update_example********************" << endl;

    JValue person = map<string, JValue> {
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         map<string, JValue>{
             {"name", "Jun"},
             {"sports", {"tennis", "dance"}},
             {"age", 19}
         }}
    };

    // update in place
    person["girl_friend"]["age"] = 20;
    cout << JValue::Serialize(person) << endl;
    cout << endl;
}

void no_indent_example() {
    cout << "**************no_indent_example********************" << endl;
    JValue person = map<string, JValue> {
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         map<string, JValue>{
             {"name", "Jun"},
             {"sports", {"tennis", "dance"}},
             {"age", 19}
         }}
    };

    cout << JValue::Serialize(person, false) << endl;
    cout << endl;
}

void deserialize_example() {
    cout << "**************deserialize_example********************" << endl;
    JValue object = JValue::Deserialize(R"(
    {
        "name" : "pin",
        "age" : 18,
        "height" : 179.9,
        "sports" : ["basketball", "football", "swimming"],
        "girl_friend" : {
            "name" : "Jun",
            "age" : 19,
            "sports" : ["tennis", "dance"]
        }
    })");

    cout << JValue::Serialize(object) << endl;
    cout << endl;
}

void struct_bind_example() {
    cout << "**************sturct_bind_example********************" << endl;
    struct Person : public JsonBinder
    {
        string name_;
        int age_;
        string secrets_; // an example property that won't work in serialize and deserialize

        void FromJson(JValue const& obj) override {
            JValue name = obj["name"], age = obj["age"];
            name_ = obj["name"].Value<string>();
            age_ = age.Value<int>();
        }

        JValue ToJson() const override {
            JValue obj(JValue::type::Object);
            obj["name"] = name_;
            obj["age"] = age_;
            return obj;
        }
    } person;

    person.Deserialize(R"({"name":"Pin", "age": 18})");
    cout << person.Serialize() << endl;
}

int main() {
    basic_example();
    update_example();
    no_indent_example();
    deserialize_example();
    struct_bind_example();

    return 0;
}
