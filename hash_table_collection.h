/*
  Maria Elena Aviles-Baquero
  File: hash_table_collection.h
  Date: December 18th, 2020
*/

#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include "array_list.h"
#include "collection.h"
#include "array_list_collection.h"
#include "bin_search_collection.h"
#include <functional>

template<typename K, typename V>
class HashTableCollection : public Collection<K,V>
{
public:
  HashTableCollection();
  HashTableCollection(const HashTableCollection<K,V>& rhs);
  ~HashTableCollection();
  HashTableCollection<K,V>& operator=(const HashTableCollection<K,V>& rhs);

  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;
  size_t min_chain_length();
  size_t max_chain_length();
  double avg_chain_length();

private:
    // the chain (linked list) nodes
    struct Node {
        K key;
        V value;
        Node* next;
    };

    // the (resizable) hash table
    Node** hash_table;

    // number of k-v pairs stored in the collection
    size_t length;

    // current number of buckets in the hash table
    size_t table_capacity;

    // current load factor of the hash table
    double load_factor_threshold = 0.75;
    
    // double size and rehash the hash table
    void resize_and_rehash();
    void make_empty();
};

// Constructor
template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection()
  : length(0), table_capacity(16)
{
  hash_table = new Node*[table_capacity];
  for(int i = 0; i < table_capacity; i++)
  {
    hash_table[i] = nullptr;
  }
}

// Copy Constructor
template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection<K,V>& rhs)
  : hash_table(nullptr), length(0), table_capacity(16)
{
  *this = rhs;
}

// Destructor
template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection()
{
  make_empty();
}

// Operator Assignment
template<typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::operator=(const HashTableCollection<K,V>& rhs)
{
  if(this != &rhs)
  {
    make_empty();
    table_capacity = rhs.table_capacity;
    length = rhs.length;
    hash_table = new Node* [table_capacity];

    for(int i = 0; i < table_capacity; i++)
    {
      hash_table[i] = nullptr;
      Node* tmp = rhs.hash_table[i];

      while(tmp)
      {
        Node* ptr = new Node;
        ptr->key = tmp->key;
        ptr->value = tmp->value;
        ptr->next = hash_table[i];
        hash_table[i] = ptr;
        tmp = tmp->next;
      }
    }
  }
  return *this;
}

// Adds to the Hash Table
template<typename K, typename V>
void HashTableCollection<K,V>::add(const K& a_key, const V& a_val)
{
  std::hash<K> hash_fun;
  size_t val, idx;
  double load_factor = 0.0;
  Node* ptr = new Node;

  load_factor = length / table_capacity;

  if(load_factor >= load_factor_threshold)
  {
    resize_and_rehash();
  }
  idx = hash_fun(a_key) % table_capacity;

  ptr->key = a_key;
  ptr->value = a_val;
  ptr->next = nullptr;

  if(hash_table[idx] == nullptr)
  {
    hash_table[idx] = ptr;
    length++;
    ptr = nullptr;
  }
  else 
  {
    ptr->next = hash_table[idx];
    hash_table[idx] = ptr;
    length++;
    ptr = nullptr;
  }

}

template<typename K, typename V>
void HashTableCollection<K,V>::remove(const K& a_key)
{
  std::hash<K> hash_fun;

  if(length <= 0)
  {
    return;
  }
  size_t val = hash_fun(a_key);
  size_t idx = val % table_capacity;
  Node* tmp = hash_table[idx];

  if(tmp && (a_key == tmp->key))
  {
    hash_table[idx] = tmp->next;
    delete tmp;
    length--;
  }
  else if(tmp)
  {
    Node* prev = tmp;
    tmp = tmp->next;
    while(tmp)
    {
      if(a_key == tmp->key)
      {
        prev->next = tmp->next;
        delete tmp;
      }
      tmp = tmp->next;
      prev = prev->next;
    }
    length--;
  }
}

template<typename K, typename V>
bool HashTableCollection<K,V>::find(const K& search_key, V& the_val) const
{
  std::hash<K> hash_fun;
  size_t val = hash_fun(search_key);
  size_t idx = val % table_capacity;
  Node* tmp = hash_table[idx];

  if(!tmp)
  {
    return 0;
  }
  else if(tmp->key == search_key)
  {
    the_val = tmp->value;
    return 1;
  }
  else
  {
    while(tmp)
    {
      if(tmp->key == search_key)
      {
        the_val = tmp->value;
        return 1;
      }
      tmp = tmp->next;
    }
  }
  return 0;
}

template<typename K, typename V>
void HashTableCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  std::hash<K> hash_fun;

  size_t val_1 = hash_fun(k1);
  size_t val_2 = hash_fun(k2);
  size_t idx_1 = val_1 % table_capacity;
  size_t idx_2 = val_2 % table_capacity;

  Node* tmp = new Node;
  for(int i = 0; i < table_capacity; i++)
  {
    if(hash_table[i])
    {
      tmp = hash_table[i];
      while(tmp)
      {
        if((tmp->key >= k1) && (tmp->key <= k2))
        {
          keys.add(tmp->key);
        }
        tmp = tmp->next;
      }
    }
  }
}

template<typename K, typename V>
void HashTableCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  Node* tmp = new Node;

  for(int i = 0; i < table_capacity; i++)
  {
    tmp = hash_table[i];
    while(tmp)
    {
      all_keys.add(tmp->key);
      tmp = tmp->next;
    }
  }
}

template<typename K, typename V>
void HashTableCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
  all_keys_sorted.sort();
}

template<typename K, typename V>
size_t HashTableCollection<K,V>::size() const
{
  return length;
}

template<typename K, typename V>
size_t HashTableCollection<K,V>::min_chain_length()
{
  if(length == 0)
  {
    return 0;
  }

  size_t min_len = length;
  for(int i = 0; i < table_capacity; i++)
  {
    size_t curr_len = 0;
    Node* tmp = hash_table[i];
    while(tmp)
    {
      curr_len++;
      tmp = tmp->next;
    }
    if(hash_table[i] && (curr_len < min_len))
    {
      min_len = curr_len;
    }
  }
  return min_len;
}


template<typename K, typename V>
size_t HashTableCollection<K,V>::max_chain_length()
{
  if(length == 0)
  {
    return 0;
  }

  size_t max_len = 0;
  for(int i = 0; i < table_capacity; i++)
  {
    size_t curr_len = 0;
    Node* tmp = hash_table[i];
    while(tmp)
    {
      curr_len++;
      tmp = tmp->next;
    }
    if(curr_len > max_len)
    {
      max_len = curr_len;
    }
  }
  return max_len;
}


template<typename K, typename V>
double HashTableCollection<K,V>::avg_chain_length()
{
  return length / (1.0 * table_capacity);
}


template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash()
{
  std::hash<K> hash_fun;

  size_t new_table_capacity = table_capacity * 2;
  size_t new_length = length;

  Node** new_hash_table = new Node*[new_table_capacity];

  for(int i = 0; i < new_table_capacity; i++)
  {
    new_hash_table[i] = nullptr;
  }
  
  for(int j = 0; j < table_capacity; j++)
  {
    Node* ptr = hash_table[j];
    while(ptr)
    {
      size_t val = hash_fun(hash_table[j]->key);
      size_t idx = val % new_table_capacity;

      Node* tmp = new Node;
      tmp->key = ptr->key;
      tmp->value = ptr->value;
      tmp->next = new_hash_table[j];
      new_hash_table[j] = tmp;
      ptr = ptr->next;
    }
  }
  make_empty();
  hash_table = new_hash_table;
  table_capacity = new_table_capacity;
  length = new_length;
}

// Empties the linked list
template<typename K, typename V>
void HashTableCollection<K,V>::make_empty() 
{  
  Node *tmp, *ptr;

  if(!hash_table)
  {
    return;
  }
  
  for(int i = 0; i < table_capacity - 1; i++)
  {
    if(hash_table[i])
    {
      tmp = hash_table[i];
      while(tmp)
      {
        ptr = tmp->next;
        delete tmp;
        tmp = ptr;
        length--;
      }
      hash_table[i] = nullptr;
    }
  }
  delete hash_table;
  hash_table = nullptr;
}






#endif