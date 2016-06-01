## Introduction

Json-11 is yet another C++ class implementing [JSON](http://json.org) data interchange format. It is designed with the (currenly) latest C++11 standard in mind. The code fits in a single file, reasonably fast, and the interface is made as simple as possible. Implements parsing, formatting, traversing and editing, [schema](http://json-schema.org) validation.

## Usage

    #include "json11.h"

Then drop json11.cpp into your project to compile. Note that you need a compiler that supports C++11. I only tested it with GCC 4.8.1 on Linux. For GCC don't forget to add `-std=c++11` to compiler flags.

### The basics

```c++
Json jnull; // creates a null Json object.
Json jtrue = true, pi = 3.14, str = "Hello";  // scalar objects
Json arr { 1, 2, str };   // array object, using initializer list
arr << false;  // do you prefer 'push_back' ?
```

To create key-value objects, start with a null object. Then use method `set(key,value)` or `operator[]` to populate it.

```c++
    Json obj;
    obj.set("one", 1).set("pi", pi);
    obj["arr"] = arr;   // nice, but cannot be chained
    Json hello = arr[2];
    arr[2] = "bye";     // will affect obj["arr"], but not hello
    cout << obj << endl;   // prints {"one":1,"pi":3.14,"arr":[1,2,"bye",false]}
    cout << hello << endl; // still prints "hello"
```

To extract scalar value from a Json instance, assign it to a scalar variable of matching type or use a cast. For collections use `operator[]` with either integer or string indexes.

```c++
    string text = str;     // text is Hello (without quotes)
    double num_pi = pi;
    int one = obj["one"];
    string hello = obj["arr"][2];
```

To 'stringify' a Json instance, write it to a std::ostream. To get it as a std::string use method `stringify()` or its alias `format()`:

    string jsontext = obj.format();

To parse a JSON text, read it from a std::istream. To parse it from std::string use static  method `parse()`:

```c++
    Json obj2 = Json::parse(jsontext);
    cout << (obj == obj2) << endl;   // prints true; yes, Jsons are comparable
    ifstream fs("big.json");
    Json big(fs);
```

To determine the type and size of a Json instance use methods `type()` and `size()`. The first one returns one of `enum Json::Type` values:

    enum Json::Type { JSNULL, BOOL, NUMBER, STRING, ARRAY, OBJECT };

The second one returns either number of array elements or number of properties of an object. Property names are returned by `keys()` as `vector<string>`. These methods, together with `operator[]` allow to fully traverse an object.

### Exceptions

The Json class defines two exception classes, `parse_error` and `use_error`. The first one can be thrown only from `Json(istream&)`. The second is thrown when the class methods are used inappropriately, for example when `operator<<` is used for an object. It is also thrown when adding to a container would lead to circular dependency, like in this example:

```c++
    Json js1, js2;
    js1 << 1 << 2;
    js2 << js1;
    js1 << js2;   // error
```

The Json class methods can also throw standard exceptions out_of_range (from operator[]) and bad_cast.

### Constructors and initializers

<dl>
<dt>Json()</dt>
<dd>Default constructor creates a Json <b>null</b> instance.</dd>
<dt>Json(const Json&)</dt>
<dd>Copy contructor.</dd>
<dt>Json(Json&&)</dt>
<dd>Move contructor.</dd>
<dt>Json(std::istream&, bool full = true)</dt>
<dd>Reads and parses well-formed JSON UTF-8 text. By default tries to parse until end of file. If not <i>full</i>, leaves input stream where all brackets are closed, so there may be more text.</dd>
<dt>Json(T x)</dt>
<dd>Conversions from scalar type <b>T</b>, where <b>T</b> is one of bool, int, long, long long, float, double, long double, const char*, std::string. Internally all numbers are stored as long double.</dd>
<dt>Json& operator = (const Json&)</dt>
<dd>Copy initializer.</dd>
<dt>Json& operator = (Json&&)</dt>
<dd>Move initializer.</dd>
<dt>Json(std::initializer_list<Json>)</dt>
<dd>Creates an instance representing JSON array. Elements of the initializer_list must be either Json instances or convertible to Json.</dd>
<dt>virtual ~Json()</dt>
<dd>Destructor.</dd>
</dl>

### Casts

<dl>
<dt>Type type() const </dt>
<dd>Returns JSON type of this instance, one of enum Json::Type</dd>
<dt>operator T() const </dt>
<dd>If this instance represents a scalar value, returns it. T is one of bool, int, long, long long, float, double, long double, std::string. Throws std::bad_cast if wrong type is requested.</dd>
</dl>

### Arrays

<dl>
<dt>Json& operator << (const Json&)</dt>
<dd>Appends an element to the array. To create a new array just append something to a null instance, or use the constructor with initializer list.</dd>
<dt>Json::Property operator [] (int) const</dt>
<dd>Retrieves array element by index, as in `int x = arr[0]`, or replaces it, as in `arr[0] = "zero"`</dd>
<dt>void insert(int index, const Json& that)</dt>
<dd>Inserts <i>that</i> into array before <i>index</i>, so it becomes the one at <i>index</i>. If <i>index</i> &lt; 0, counts from the end.</dd>
<dt>Json& replace(int index, const Json& that)</dt>
<dd>Replaces array element at <i>index</i> by <i>that</i>. If <i>index</i> &lt; 0, counts from the end.</dd>
<dt>void erase(int index)</dt>
<dd>Removes element at </i>index</i> from the array. If <i>index</i> &lt; 0, counts from the end.</dd>
</dl>

These methods throw `use_error` if this Json instance is not an array.

### Objects

<dl>
<dt>Json& set(std::string key, const Json& value)</dt>
<dd>Adds property "key:value" to this object, or replaces the <i>value</i> if <i>key</i> is already there. To create a new object, start from a null Json instance.</dd>
<dt>Json get(const std::string& key) const</dt>
<dd>Returns value for the given key, or `Json::undefined` if this instance does not have such property.</dd>
<dt>Json::Property operator [] (std::string&)</dt>
<dd>Returns value for the given key, or `Json::undefined` if this instance does not have such property. When used on the left of assignment operator as in `obj["key"] = value`, adds a key with given value or replaces existing one.</dd>
<dt>Json::Property operator [] (const char*)</dt>
<dd>Same as the previous one.</dd>
<dt>bool has(const std::string& key) const</dt>
<dd>Returns `true` if this instance has propery with given <i>key</i>.</dd>
<dt>std::vector<std::string> keys()</dt>
<dd>Returns all property keys for this instance.</dd>
</dl>

These methods throw `use_error` if this Json instance is not an key-value object.

### Subscript operator

Technically, `operator[]` returns and instance of internal class Json::Property. Instances of this class behave very much like Json<i>s</i>, so in practice you don't have to remember about it.

### Comparison

<dl>
<dt>bool operator == (const Json&)</dt>
<dd>Compares Json instances. Scalars are equal if and only if they have same types and same values. Arrays are equal if their elements and order of them are the same. Objects are equal if their property keys and corresponding values are equal, regardless of order.</dd>
<dt>bool operator != (const Json&)</dt>
<dd>The opposite.</dd>
</dl>

### Parsing and formatting

<dl>
<dt>static Json parse(const std::string&)</dt>
<dd>Returns a Json instance built from well-formed JSON text. UTF-8 encoding is assumed. See also Json(std::istream&).</dd>
<dt>std::string stringify()</dt>
<dd>Returns well-formed JSON text representing this object as a string.</dd>
<dt>std::string format()</dt>
<dd>Same as stringify().</dd>
<dt>friend std::ostream& operator << (std::ostream&, const Json&)</dt>
<dd>Writes well-formed JSON text representing this object into std::ostream.</dd>
<dt>friend std::istream& operator >> (std::istream&, Json&)</dt>
<dd>Reads well-formed JSON text into given instance, replacing previous content.</dd>
<dt>static int indent</dt>
<dd>If not 0, result of formatting looks prettier.</dd>
</dl>

### Validation

<dl>
<dt>bool valid(Json& schema, std::string* reason = nullptr)</dt>
<dd>Validates this instance against <i>schema</i>. If not valid and <i>reason</i> is not a null pointer, fills it with a short explanation of the problem.</dd>
<dt>bool to_schema(std::string* reason)</dt>
<dd>If this instance is a schema, tries to "compile" it, returning `false` and filling in <i>reason</i> if this is not a valid schema. Useful if this schema will be used for validation several times, otherwise every call of `valid()` will compile its argument again.</dd>
</dl>

### Etc.

<dl>
<dt>size_t size() const</dt>
<dd>Returns size of an array or number of properties for an object.</dd>
<dt>static Json null</dt>
<dd>The <b>null</b> instance.</dd>
<dt>static Json array()</dt>
<dd>Returns an empty array, that is [].</dd>
<dt>static Json object()</dt>
<dd>Returns an empty object, that is {}</dd>
</dl>

### Miscellaneous notes

Json class has value semantics. The following code:

```c++
Json arr { 1, 2, "three" };
Json three = arr[2];
arr.replace(2, 3);   // or:  arr.erase(2);
cout << three;
```

still prints "three". I hope this is what one would expect. On the other hand, if an array or object is a component of another array or object, changes in the component are visible "through", e.g.:

```c++
Json arr { 1, 2, "three" };
Json obj;
obj["arr"] = arr;
arr[2] = 3;
cout << obj["arr"][2];   // prints 3
```

-------

Json class defines a static member Json::null of type JSNULL. It can be used to test for 'nullity' and for removing all contents from an object:

    if (js == Json::null)
        big_object = Json::null;

------

There is a difference between s1 and s2 in the following example:

    Json hello = "hello";
    string s1 = hello;
    string s2 = hello.format();

The assignment to s1 is a cast, so the value of s1 is *hello*. The value of s2 is *"hello"* (with double quotes) because `format` creates a well-formed JSON text and returns it as a string.

------

The project contains a toy command line application for viewing and editing JSON files. Just compile and run clison.cpp. The dialog looks like this:

```
at top: object
  0. web-app {3}
> 0
.web-app: object
  0. servlet [5]
  1. servlet-mapping {5}
  2. taglib {2}
> 2
.web-app.taglib: object
  0. taglib-uri: "cofax.tld"
  1. taglib-location: "/WEB-INF/tlds/cofax.tld"
> h
enter a number to select an object, q to go back
.             : list current object
p [file.json] : print out current object [into file]
s file.json   : load file as a json schema
= text        : replace current object by parsed text
>
```

------
