#include <iostream>
#include <unordered_map>
#include "json.hpp"

using namespace std;
using namespace json;

void basic_example() {
    cout << "**************basic_example********************" << endl;

    JValue person = unordered_map<string, JValue>{
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         unordered_map<string, JValue>{
             {"name", "Jun"},
             {"sports", {"tennis", "dance"}},
             {"age", 19}
         }}
    };

    // serialize
    cout << JValue::Serialize(person) << endl;
    cout << endl;
}

void update_example() {
    cout << "**************update_example********************" << endl;

    JValue person = unordered_map<string, JValue>{
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         unordered_map<string, JValue>{
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
    JValue person = unordered_map<string, JValue>{
        {"name", "pin"},
        {"age", 18},
        {"height", 179.9},
        {"sports", {"basketball", "football", "swimming"}},
        // nested object
        {"girl_friend",
         unordered_map<string, JValue>{
             {"name", "Jun"},
             {"sports", {"tennis", "dance"}},
             {"age", 19}
         }}
    };

    cout << JValue::Serialize(person, false) << endl;
    cout << endl;
}

int main() {
    basic_example();
    update_example();
    no_indent_example();

    return 0;
}
