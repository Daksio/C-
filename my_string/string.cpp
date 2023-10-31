#include "string.hpp"

char& String::operator[](int ind) { return str_[ind]; }

const char& String::operator[](int ind) const { return str_[ind]; }

String& String::operator=(const String& other) {
  if (this == &other) {
    return *this;
  }
  Resize(other.size_);
  for (size_t i = 0; i < size_; i++) {
    str_[i] = other.str_[i];
  }
  return *this;
}

String::String() : String("") {}

String::String(size_t size, char character) { Resize(size, character); }

String::String(const char* str) {
  size_t new_size = strlen(str);
  Resize(new_size);
  for (size_t i = 0; i < new_size; i++) {
    str_[i] = str[i];
  }
}

String::String(const String& other) {
  Resize(other.size_);
  for (size_t i = 0; i < size_; i++) {
    str_[i] = other.str_[i];
  }
}

String::~String() { delete[] str_; }

void String::Clear() {
  size_ = 0;
  str_[0] = '\0';
}

void String::PushBack(char character) {
  Resize(size_ + 1);
  str_[size_ - 1] = character;
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  size_--;
  str_[size_] = '\0';
}

void String::Resize(size_t new_size) {
  Reserve(new_size + 1);
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  Reserve(new_size + 1);
  if (new_size > size_) {
    for (size_t i = size_; i < new_size; i++) {
      str_[i] = character;
    }
  }
  size_ = new_size;
  str_[size_] = '\0';
}

void String::Reserve(size_t new_cap) {
  if (new_cap > capacity_) {
    while (new_cap < 2 * capacity_) {
      new_cap = 2 * capacity_;
    }
    char* buff = str_;
    str_ = new char[new_cap + 1];
    if (size_ > 0) {
      for (size_t i = 0; i < size_; i++) {
        str_[i] = buff[i];
      }
    }
    delete[] buff;
    capacity_ = new_cap;
  } else {
    return;
  }
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    char* buff = str_;
    str_ = new char[size_];
    for (size_t i = 0; i < size_; i++) {
      str_[i] = buff[i];
    }
    capacity_ = size_;
    delete[] buff;
  }
}

void String::Swap(String& other) {
  String tmp = other;
  other = *this;
  *this = tmp;
}

char& String::Front() { return str_[0]; }

char String::Front() const { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

char String::Back() const { return str_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

char* String::Data() { return str_; }

const char* String::Data() const { return str_; }

bool operator==(const String& first, const String& second) {
  return strcmp(first.Data(), second.Data()) == 0;
}

bool operator!=(const String& first, const String& second) {
  return strcmp(first.Data(), second.Data()) != 0;
}

bool operator<(const String& first, const String& second) {
  size_t size = first.Size() < second.Size() ? first.Size() : second.Size();
  for (size_t i = 0; i < size; i++) {
    if (first[i] != second[i]) {
      return first[i] < second[i];
    }
  }
  return first.Size() < second.Size();
}

bool operator>(const String& first, const String& second) {
  return second < first;
}

bool operator<=(const String& first, const String& second) {
  return (first < second) || (first == second);
}

bool operator>=(const String& first, const String& second) {
  return (first > second) || (first == second);
}

String operator+(const String& first, const String& second) {
  int new_size = first.Size() + second.Size();
  String new_str;
  new_str.Resize(new_size);
  memcpy(new_str.Data(), first.Data(), first.Size());
  memcpy(new_str.Data() + first.Size(), second.Data(), second.Size());
  return new_str;
}

String& String::operator+=(const String& other) {
  *this = *this + other;
  return *this;
}

String operator*(const String& str, int n) {
  if (n == 0) {
    return String("");
  }
  String new_str;
  new_str.Resize(str.Size() * n);
  for (int i = 0; i < n; i++) {
    memcpy(new_str.Data() + i * str.Size(), str.Data(), str.Size());
  }
  return new_str;
}

String& String::operator*=(int n) {
  *this = *this * n;
  return *this;
}

std::istream& operator>>(std::istream& ins, String& other) {
  char character;  // символ для чтения
  while (ins.get(character) && (character == ' ' || character == '\n')) {
    // пропускаем пробельные символы
  }
  if (ins) {
    do {
      other.PushBack(character);
    } while (ins.get(character) && character != ' ' && character != '\n');
    if (ins) {
      ins.unget();
    }
  }
  return ins;
}

std::ostream& operator<<(std::ostream& ous, const String& other) {
  ous << other.str_;
  return ous;
}

size_t String::Find(const String& substring, size_t pos) const {
  if (pos >= size_) {
    return npos_;
  }
  for (size_t i = pos; i < size_; i++) {
    if (str_[i] == substring.str_[0]) {
      bool flag = true;
      for (size_t j = 1; j < substring.size_; j++) {
        if (str_[i + j] != substring.str_[j]) {
          flag = false;
          break;
        }
      }
      if (flag) {
        return i;
      }
    }
  }
  return npos_;
}

String String::Substr(size_t pos, size_t count) const {
  if (pos >= size_) {
    return String("");
  }
  if (count > size_ - pos) {
    count = size_ - pos;
  }
  String new_str;
  new_str.Resize(count);
  for (size_t i = 0; i < count; i++) {
    new_str.str_[i] = str_[pos + i];
  }
  return new_str;
}

std::vector<String> String::Split(const String& delimiter) const {
  std::vector<String> split;
  size_t pos = 0;
  size_t prev_pos = 0;
  while ((pos = Find(delimiter, prev_pos)) != String::npos_) {
    split.push_back(Substr(prev_pos, pos - prev_pos));
    prev_pos = pos + delimiter.Size();
  }
  split.push_back(Substr(prev_pos, Size() - prev_pos));
  return split;
}

String String::Join(const std::vector<String>& strings) const {
  String total_string;
  for (size_t i = 0; i < strings.size(); ++i) {
    total_string += strings[i];
    if (i < strings.size() - 1) {
      total_string += *this;
    }
  }
  return total_string;
}