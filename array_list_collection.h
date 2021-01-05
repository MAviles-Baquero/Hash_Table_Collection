//----------------------------------------------------------------------
// Author: Maria Elena Aviles-Baquero
// File:   array_list_collection.h
// Date:   December 07, 2020
//
// Assumptions:
//
//   1. Collections consist of a set of unique keys, where each key
//      has an associated value. A key together with its value is
//      called a "key-value pair".
//
//   2. Because a collection stores a particular key only once, if a
//      key can be associated with multiple underlying values, then
//      the key must be associated with a list containing the
//      underlying values. Thus, the collection would consist of
//      key-value pairs such that each key's corresponding value is
//      a list.
// 
//   3. A key should only be added into a collection if the key isn't
//      already in the collection. If the same key is added more than
//      once, then the behavior of the collection becomes undefined
//      (in terms of finding and removing keys).
//----------------------------------------------------------------------

 
#ifndef ARRAY_LIST_COLLECTION_H
#define ARRAY_LIST_COLLECTION_H

#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class ArrayListCollection : public Collection <K,V>
{
public:
  void add(const K& a_key, const V& a_val);
  void remove(const K& a_key);
  bool find(const K& search_key, V& the_val) const;
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  void keys(ArrayList<K>& all_keys) const;
  void sort(ArrayList<K>& all_keys_sorted) const;
  size_t size() const;

private:
  ArrayList<std::pair<K,V>> kv_list;

};

// Adds a new key-value pair to the collection
template<typename K, typename V>
void ArrayListCollection<K,V>::add(const K& a_key, const V& a_val)
{
  kv_list.add(std::pair<K,V>(a_key, a_val));
}

// Removes a key-value pair from the collection
template<typename K, typename V>
void ArrayListCollection<K,V>::remove(const K& a_key)
{
  std::pair<K,V> tmp_list;
  kv_list.get(0, tmp_list);

  size_t idx = 0;
  // increments until the key is found
  while(tmp_list.first != a_key) 
  {
    idx++;
    kv_list.get(idx, tmp_list);
  }

  // Removes the key-value pair
  kv_list.remove(idx);
}

// Find and returns the value associated with the key
template<typename K, typename V>
bool ArrayListCollection<K,V>::find(const K& search_key, V& the_val) const
{
  std::pair<K,V> tmp_list;

  // Iterates through kv_list
  for(int i = 0; i < size(); i++)
  {
    kv_list.get(i, tmp_list);

    // If the key is found, the_val is assigned the value found and returns true
    if(tmp_list.first == search_key)
    {
      the_val = tmp_list.second;
      return 1;
    }
  }

  // If the key is not found, false is returns
  return 0;
}

// Find and returns each key >= k1 and <= k2
template<typename K, typename V>
void ArrayListCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  // If the range is valid then the function continues
  if (k1 <= k2) 
  {
    std::pair<K,V> tmp_list;

    // Iterates through kv_list and when the range is found, keys is assigned the keys found
    for(int i = 0; i < size(); i++)
    {
      kv_list.get(i, tmp_list);
      if((tmp_list.first >= k1) && (tmp_list.first <= k2))
      {
        keys.add(tmp_list.first);
      }
    }
  }
}

// Returns all of the keys in the collection
template<typename K, typename V>
void ArrayListCollection<K,V>::keys(ArrayList<K>& all_keys) const
{
  std::pair<K,V> tmp_list;

  // Iterates thorugh kv_list and adds the keys to all_keys
  for(int i = 0; i < size(); i++)
  {
    kv_list.get(i, tmp_list);
    all_keys.add(i, tmp_list.first);
  }
}

// returns all of the keys in sorted order (ascending)
template<typename K, typename V>
void ArrayListCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  // uses the function above to find all the keys, then sorts them
  keys(all_keys_sorted);
  all_keys_sorted.sort();
}

// Returns the number of pairs in the collection
template<typename K, typename V>
size_t ArrayListCollection<K,V>::size() const
{
  return kv_list.size();
}


#endif