# A simple Json library
A simple header only json implementation in C++17.

- Simple to use.
```cpp
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
```

the output
```bash
{
  "girl_friend": {
    "name": "Jun",
    "age": 19,
    "sports": [
      "tennis",
      "dance"
    ]
  },
  "sports": [
    "basketball",
    "football",
    "swimming"
  ],
  "height": 179.9,
  "age": 18,
  "name": "Pin"
}
```

- update nested property in place
```cpp
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

// update property
person["girl_friend"]["age"] = 20;
cout << JValue::Serialize(person) << endl;
```

- serialize without indent
```cpp
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

// serialize without indent
cout << JValue::Serialize(person, false) << endl;
cout << endl;
```
