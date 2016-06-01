/*
 * This file is part of json11 project (https://github.com/borisgontar/json11).
 *
 * Copyright (c) 2013 Boris Gontar.
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

// Version 0.6.5, 2013-11-07

#ifndef JSON11_H_
#define JSON11_H_

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <cfloat>
#include <stdexcept>
#include <initializer_list>
#include "toast/library.hpp"

class Json {
public:
    API enum Type {
        JSNULL, BOOL, NUMBER, STRING, ARRAY, OBJECT
    };
    
    struct Schema;  // forward dcl
    struct Node {
    	unsigned refcnt;
        API Node(unsigned init = 0);
        API virtual ~Node();
        API virtual Type type() const { return Type::JSNULL; }
        API virtual void print(std::ostream& out) const { out << "null"; }
        API virtual void traverse(void (*f)(const Node*)) const { f(this); }
        API virtual bool contains(const Node* that) const { return false; }
        API virtual bool operator == (const Node& that) const { return this == &that; }
        API virtual bool is_schema() const { return false; }
        API void unref();
        static Node null, undefined;
    };
    //
    struct Bool : Node {
        API Bool(bool x) { refcnt = 1; }
        API Type type() const override { return Type::BOOL; }
        API void print(std::ostream& out) const override;
        static Bool T;
        static Bool F;
    };
    //
    struct Number : Node {
        long double value;
        int prec;
        API Number(long double x) { prec = LDBL_DIG; value = x; }
        API Number(double x) { prec = DBL_DIG; value = x; }
        API Number(float x) { prec = FLT_DIG; value = x; }
        API Number(long long x) { prec = DBL_DIG; value = x; }
        API Number(long x) { prec = -1; value = x; }
        API Number(int x) { prec = -1; value = x; }
        API Number(std::istream&);
        API Type type() const override { return Type::NUMBER; }
        API void print(std::ostream& out) const override;
        API bool operator == (const Node& that) const override;
    };
    //
    struct String : Node {
        std::string value;
        API String(std::string s) { value = s; }
        API String(std::istream&);
        API Type type() const override { return Type::STRING; }
        API void print(std::ostream& out) const override;
        API bool operator == (const Node& that) const override;
    };
    //
    struct Array : Node {
        std::vector<Node*> list;
        API virtual ~Array();
        API Type type() const override { return Type::ARRAY; }
        API void print(std::ostream&) const override;
        API void traverse(void (*f)(const Node*)) const override;
        API void add(Node*);
        API void ins(int, Node*);
        API void del(int);
        API void repl(int, Node*);
        API bool contains(const Node*) const override;
        API bool operator == (const Node& that) const override;
    };
    //
    struct Object : Node {
        std::map<const std::string*, Node*> map;
        API virtual ~Object();
        API Type type() const override { return Type::OBJECT; }
        API void print(std::ostream&) const override;
        API void traverse(void (*f)(const Node*)) const override;
        API Node* get(const std::string&) const;
        API void set(const std::string&, Node*);
        API bool contains(const Node*) const override;
        API bool operator == (const Node& that) const override;
    };
    //
    class Property {
        Node* host;
        std::string key;
        int index;
        API Json target() const;
    public:
        API Property(Node*, const std::string&);
        API Property(Node*, int);
        API operator Json() const { return target(); }
        API operator bool() { return target(); }
        API operator int() { return target(); }
        API operator long() { return target(); }
        API operator long long() { return target(); }
        API operator float() { return target(); }
        API operator double() { return target(); }
        API operator long double() { return target(); }
        API operator std::string() const { return target(); }
        API Property operator [] (const std::string& k) { return target()[k]; }
        API Property operator [] (const char* k) { return (*this)[std::string(k)]; }
        API Property operator [] (int i) {return target()[i]; }
        API Json operator = (const Json&);
        API Json operator = (const Property&);
        API bool operator == (const Json& js) const { return (Json)(*this) == js; }
        API bool operator != (const Json& js) const { return !(*this == js); }
        API std::vector<std::string> keys() { return target().keys(); }
        API bool has(const std::string& key) const { return target().has(key); }
        friend std::ostream& operator << (std::ostream& out, const Property& p) {
            return out << (Json)p;
        }
        friend Json;
    };
    API Array* mkarray();
    API Object* mkobject();
    static std::set<std::string> keyset;   // all propery names
    static int level;   // for pretty printing
    //
    API Json(Node* node);
    Node* root;
    //
public:
    // constructors
    API Json();
    API Json(const Json& that);
    API Json(Json&& that);
    API Json(std::istream&, bool full = true);   // parse
    API virtual ~Json();
    //
    // initializers
    API Json& operator = (const Json&);
    API Json& operator = (Json&&);
    //
    // more constructors
    API Json(bool x);
    API Json(int x) { (root = new Number(x))->refcnt++; }
    API Json(long x) { (root = new Number(x))->refcnt++; }
    API Json(long long x) { (root = new Number(x))->refcnt++; }
    API Json(float x) { (root = new Number(x))->refcnt++; }
    API Json(double x) { (root = new Number(x))->refcnt++; }
    API Json(long double x) { (root = new Number(x))->refcnt++; }
    API Json(std::string& s) { (root = new String(s))->refcnt++; }
    API Json(const char* s) { (root = new String(s))->refcnt++; }
    API Json(std::initializer_list<Json>);
    API Json(const Property& p) { (root = p.target().root)->refcnt++; }
    //
    // casts
    API Type type() const { return root->type(); }
    API operator bool() const;
    API operator int() const;
    API operator long() const;
    API operator long long() const;
    API operator float() const;
    API operator double() const;
    API operator long double() const;
    API operator std::string() const;
    //
    // object
    API Json& set(std::string key, const Json& val);
    API Json get(const std::string& key) const;
    API bool has(const std::string& key) const;
    API std::vector<std::string> keys();
    //
    // array
    API Json& operator << (const Json&);
    API void insert(int index, const Json&);
    API void erase(int index);
    API Json& replace(int index, const Json&);
    //
    // subscript
    API size_t size() const;
    API Json::Property operator [] (const std::string&);
    API Json::Property operator [] (const char* k) { return (*this)[std::string(k)]; }
    API Json::Property operator [] (int);
    //
    // stringify
    API std::string stringify() { return format(0); }
    API std::string stringify(int indent) { return format(indent); }
    API std::string format(int indent);
    API friend std::ostream& operator << (std::ostream&, const Json&);
    API friend std::istream& operator >> (std::istream&, Json&);
    //
    // compare
    API bool operator == (const Json&) const;
    API bool operator != (const Json& that) const { return !(*this == that); }
    //
    //
    API static Json null, undefined;
    API static Json parse(const std::string&);
    API static Json array() { return new Array(); }    // returns empty array
    API static Json object() { return new Object(); }  // returns empty object
    static int indent;  // for pretty printing
    //
    struct parse_error : std::runtime_error {
        unsigned line = 0, col = 0;
        parse_error(const char* msg, std::istream& in);
    };
    struct use_error : std::logic_error {
        use_error(const char* msg) : std::logic_error(msg) {}
        use_error(const std::string msg) : std::logic_error(msg.c_str()) {}
    };
};

#endif /* JSON11_H_ */
