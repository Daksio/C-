#pragma once

#include <cstring>
#include <iostream>
#include <vector>

class String {
 private:
  char* str_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
  size_t npos_ = -1;

 public:
  String();

  char& operator[](int);

  const char& operator[](int) const;

  String& operator=(const String&);

  String(size_t, char);

  String(const char*);

  String(const String&);

  ~String();

  void Clear();

  void PushBack(char);

  void PopBack();

  void Resize(size_t);

  void Resize(size_t, char);

  void Reserve(size_t);

  void ShrinkToFit();

  void Swap(String&);

  char& Front();
  char Front() const;

  char& Back();
  char Back() const;

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  char* Data();

  const char* Data() const;

  friend bool operator==(const String&, const String&);

  friend bool operator!=(const String&, const String&);

  friend bool operator<(const String&, const String&);

  friend bool operator<=(const String&, const String&);

  friend bool operator>(const String&, const String&);

  friend bool operator>=(const String&, const String&);

  friend String operator+(const String&, const String&);

  String& operator+=(const String&);

  friend String operator*(const String&, int);

  String& operator*=(int);

  friend std::istream& operator>>(std::istream&, String&);

  friend std::ostream& operator<<(std::ostream&, const String&);

  size_t Find(const String&, size_t) const;

  String Substr(size_t, size_t) const;

  std::vector<String> Split(const String& delimiter = " ") const;

  String Join(const std::vector<String>&) const;
};