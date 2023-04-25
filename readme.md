# A simple Json library
A simple header only JSON implementation in C++17.

This is a subway project, as an entertainment, I did most work during my daily time to company in the subway.

- [A simple Json library](#a-simple-json-library)
- [Roadmap](#roadmap)
- [Features](#features)
  - [STL-style API is provided.](#stl-style-api-is-provided)
  - [Simple to Seriliaze an JSON object](#simple-to-seriliaze-an-json-object)
- [build](#build)

# Roadmap
- [x] Serialize
- [ ] Deserialize
- [ ] Structure binding
- [ ] Memory pool optimization
- [ ] simd optimization
- [ ] Escape/Unescape string
- [ ] Unicode

# Features
## STL-style API is provided.
you can initialize an JSON object by:
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
```

You can consider that a JSON object as std::map, so you can also update nested property in place.
```cpp
person["girl_friend"]["age"] = 20;
```

Similarly, the JSON array is very close to std::vector, you can use JSON array simply like:
```cpp
JValue array = {"a", "b", "c"};
array.Array().push_back("a");
```

Convert any supported type to JSON object/array/value directly
```cpp
JValue b = bool;
JValue s = "a string";
JValue i = 24;
JValue d = 1.0
```

Get the actual type of the JSON value by
```
JValue s = "a string";
cout << s.Type() << endl;
// to type string
cout << JValue::TypeString(s.Type()) << endl;
```

## Simple to Seriliaze an JSON object
```cpp
cout << JValue::Serialize(person) << endl;
```
the output is:
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

# build
Tested on 
```bash
clang version 10.0.0-4ubuntu1
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
```bash
g++ (Ubuntu 10.3.0-1ubuntu1~20.04) 10.3.0
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

compile the test file with
```bash
g++ test.cpp -std=c++17
```
or
```bash
clang++ test.cpp -std=c++17
```