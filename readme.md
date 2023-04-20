# A simple Json library
A simple header only json implementation in C++17.

- [A simple Json library](#a-simple-json-library)
- [Basic Examples](#basic-examples)
  - [Simple to use.](#simple-to-use)
  - [update nested property in place](#update-nested-property-in-place)
  - [serialize without indent](#serialize-without-indent)
- [Roadmap](#roadmap)

# Basic Examples
## Simple to use.
```cpp
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

## update nested property in place
```cpp
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
```

## serialize without indent
```cpp
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
```

# Roadmap
- [x] Serialize
- [ ] Deserialize
- [ ] Escape/Unescape string
- [ ] Keep object's original order
- [ ] Memory pool
- [ ] simd optimization