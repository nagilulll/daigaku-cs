#include "Map.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Map::Map()
 : m_size(0)
{
    head = nullptr;
    tail = nullptr;
}

Map::Map(const Map& other)
: m_size(0)
{
    head = nullptr;
    tail = nullptr;
    
    Node* copy = other.head;
    while (copy != nullptr)
    {
        insert(copy->key, copy->value);
        copy = copy->next;
    }
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

Map::~Map()
{
    Node* dele = head;
    while (dele != nullptr)
    {
        Node* move = dele->next;
        delete dele;
        dele = move;
    }
    head = nullptr;
    tail = nullptr;
    m_size = 0;
}

  
int Map::size() const
{
    return m_size;
}

  
bool Map::empty() const
{
    if (head == nullptr && tail == nullptr && m_size == 0)
        return true;
    return false;
}

  
bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (contains(key))
    {
        return false;
    }
    Node *newPair = new Node{key, value, nullptr, nullptr};
    if (empty())
    {
        head = newPair;
        tail = newPair;
    }
    else
    {
        tail->next = newPair;
        newPair->previous = tail;
        newPair->next = nullptr;
        tail = newPair;
    }
    m_size++;
    return true;
}
  
bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* keyCheck = head;
    
    while (keyCheck != nullptr)
    {
        if (keyCheck->key == key)
        {
            keyCheck->value = value;
            return true;
        }
        keyCheck = keyCheck->next;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value))
        return true;
    
    return insert(key, value);
}

bool Map::erase(const KeyType& key)
{
    if (!contains(key))
        return false;
    
    Node *keyCheck = head;
    
    while (keyCheck != nullptr)
    {
        if (keyCheck->next != nullptr && keyCheck->next->key == key)
        {
            break;
        }
        keyCheck = keyCheck->next;
    }
    
    if (keyCheck != nullptr)
    {
        Node *killMe = keyCheck->next;
        keyCheck->next = killMe->next;
        delete killMe;
        return true;
    }
    return false;
}

bool Map::contains(const KeyType &key) const
{
    Node* keyCheck = head;
    
    while (keyCheck != nullptr)
    {
        if (keyCheck->key == key)
        {
            return true;
        }
        keyCheck = keyCheck->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* keyCheck = head;
    
    while (keyCheck != nullptr)
    {
        if (keyCheck->key == key)
        {
            value = keyCheck->value;
            return true;
        }
        keyCheck = keyCheck->next;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)
        return false;
    else
    {
        Node* keyCheck = head;
        
        while (keyCheck != nullptr)
        {
            Node* lowerKey = head;
            int count = 0;
            
            while (lowerKey != nullptr)
            {
                if (lowerKey->key < keyCheck->key)
                {
                    count++;
                }
                lowerKey = lowerKey->next;
            }
            if (count == i)
            {
                key = keyCheck->key;
                value = keyCheck->value;
                return true;
            }
            keyCheck = keyCheck->next;
        }
    }
    return false;
}

void Map::swap(Map& other)
{
    Node *tempHead = head;
    head = other.head;
    other.head = tempHead;
    
    Node *tempTail = tail;
    tail = other.tail;
    other.tail = tempTail;
    
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool merge(const Map& m1, const Map& m2, Map& result)
{
    KeyType key;
    ValueType value;
    
    for (int i = 0; i < m1.size(); i++)
    {
        m1.get(i, key, value);
        result.insertOrUpdate(key, value);
    }
    for (int j = 0; j < m2.size(); j++)
    {
        m2.get(j, key, value);
        ValueType valueCheck;
        
        if (result.get(key, valueCheck))
        {
            if (valueCheck != value)
                return false;
        }
        else
        {
            result.insert(key, value);
        }
    }
    return true;
}

void reassign(const Map& m, Map& result)
{
    KeyType key;
    ValueType value;
    
    for (int i = 1; i < m.size(); i++)
    {
        m.get(i, key, value);
        result.insert(key, value);
    }
    m.get(0, key, value);
    result.insert(key, value);
}
