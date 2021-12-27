//
// Created by liu on 18-10-23.
//

#include "Table.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Database.h"

constexpr const Table::ValueType Table::ValueTypeMax;
constexpr const Table::ValueType Table::ValueTypeMin;

Table::FieldIndex
Table::getFieldIndex(const Table::FieldNameType &field) const {
  try {
    return this->fieldMap.at(field);
  } catch (const std::out_of_range &e) {
    throw TableFieldNotFound(R"(Field name "?" doesn't exists.)"_f % (field));
  }
}

void Table::insertByIndex(KeyType key, std::vector<ValueType> &&data) {
  if (this->keyMap.find(key) != this->keyMap.end()) {
    std::string err = "In Table \"" + this->tableName + "\" : Key \"" + key +
                      "\" already exists!";
    throw ConflictingKey(err);
  }
  this->keyMap.emplace(key, this->data.size());
  this->data.emplace_back(std::move(key), data);
}

/* Table::Iterator Table::deleteByIndex(KeyType key) {
  auto mapIt = this->keyMap.find(key);
  if (mapIt == this->keyMap.end()) {
    std::string err = ""; // TODO
    throw ConflictingKey(err);
  }
  auto returnDataIt = this->data.erase(this->data.begin() + static_cast<long>(mapIt->second));
  this->keyMap.erase(mapIt);
  return Iterator{returnDataIt, this};
}  */

void Table::beforeDelete(Iterator i) {
  i.it->should_delete = true;
}

void Table::doDelete() {
  keyMap.clear();
  std::vector<Datum> temp;
  temp.swap(data);
  for(auto it = temp.begin(); it != temp.end(); ++it) {
    if(!it->should_delete) {
      insertByIndex(it->key, std::move(it->datum));
    }
  }
}

bool Table::evalDuplicate(KeyType key) {
  return this->keyMap.find(key + "_copy") == this->keyMap.end();
}

void Table::dupByIndex(long index, size_t &affected) {
  using std::move;
  auto it = this->data.begin() + index;
  KeyType key = it->key + "_copy";
  if (this->keyMap.find(key) != this->keyMap.end())
    return;
  this->keyMap.emplace(key, this->data.size());
  this->data.emplace_back(move(key), it->datum);
  affected++;
}

Table::Object::Ptr Table::operator[](const Table::KeyType &key) {
  auto it = keyMap.find(key);
  if (it == keyMap.end()) {
    // not found
    return nullptr;
  } else {
    return createProxy(
        data.begin() +
            static_cast<std::vector<Table::Datum>::difference_type>(it->second),
        this);
  }
}

std::ostream &operator<<(std::ostream &os, const Table &table) {
  const int width = 10;
  std::stringstream buffer;
  buffer << table.tableName << "\t" << (table.fields.size() + 1) << "\n";
  buffer << std::setw(width) << "KEY";
  for (const auto &field : table.fields) {
    buffer << std::setw(width) << field;
  }
  buffer << "\n";
  auto numFields = table.fields.size();
  for (const auto &datum : table.data) {
    buffer << std::setw(width) << datum.key;
    for (decltype(numFields) i = 0; i < numFields; ++i) {
      buffer << std::setw(width) << datum.datum[i];
    }
    buffer << "\n";
  }
  return os << buffer.str();
}
