#include <iostream>
#include "json.hpp"

using namespace std;
using namespace json;

void basic_example() {
    cout << "**************basic_example********************" << endl;

    // initialize
    JValue person(JValue::type::Object);
    person["name"] = JValue("Pin");
    person["age"] = JValue(18);
    person["height"] = JValue(179.9);
    person["sports"] = {"basketball", "football", "swimming"};

    // nested object
    JValue aFriend(JValue::type::Object);
    aFriend["name"] = "Jun";
    aFriend["sports"] = {"tennis", "dance"};
    aFriend["age"] = 19;

    person["girl_friend"] = aFriend;

    // serialize
    cout << JValue::Serialize(person) << endl;
    cout << endl;
}

void update_example() {
    cout << "**************update_example********************" << endl;
    // initialize
    JValue person(JValue::type::Object);
    person["name"] = JValue("Pin");
    person["age"] = JValue(18);
    person["height"] = JValue(179.9);
    person["sports"] = {"basketball", "football", "swimming"};

    // nested object
    JValue aFriend(JValue::type::Object);
    aFriend["name"] = "Jun";
    aFriend["sports"] = {"tennis", "dance"};
    aFriend["age"] = 19;

    person["girl_friend"] = aFriend;

    person["girl_friend"]["age"] = 20;
    cout << JValue::Serialize(person) << endl;
    cout << endl;
}

void no_indent_example() {
    cout << "**************no_indent_example********************" << endl;
    // initialize
    JValue person(JValue::type::Object);
    person["name"] = JValue("Pin");
    person["age"] = JValue(18);
    person["height"] = JValue(179.9);
    person["sports"] = {"basketball", "football", "swimming"};

    // nested object
    JValue aFriend(JValue::type::Object);
    aFriend["name"] = "Jun";
    aFriend["sports"] = {"tennis", "dance"};
    aFriend["age"] = 19;

    person["girl_friend"] = aFriend;

    cout << JValue::Serialize(person, false) << endl;
    cout << endl;
}

int main() {
    basic_example();
    update_example();
    no_indent_example();
    return 0;
}
