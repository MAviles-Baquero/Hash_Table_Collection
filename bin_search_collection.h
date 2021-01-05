/*
  Maria Elena Aviles-Baquero
  File: bin_search_collection.h
  Date: December 18th, 2020
*/

#ifndef BIN_SEARCH_COLLECTION_H
#define BIN_SEARCH_COLLECTION_H

#include "array_list.h"
#include "collection.h"
#include "array_list_collection.h"

template<typename K, typename V>
class BinSearchCollection : public Collection<K,V>
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

    // bin search helper
    bool bin_search(const K& key, size_t& index) const;

};


// Adds a new key-value pair to the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::add(const K& a_key, const V& a_val)
{
  size_t idx = 0;
  bool check = bin_search(a_key, idx);

  // if the collection is empty then add regularly
  if(size() == 0)
  {
    kv_list.add(std::pair<K,V>(a_key, a_val));
  }
  else
  {
    //find the key value pair at the idx and add into collection
    std::pair<K,V> tmp_list;
    kv_list.get(idx, tmp_list);
    if(tmp_list.first > a_key)
    {
      kv_list.add(idx, std::pair<K,V>(a_key, a_val));
    }
    else 
    {
      kv_list.add(idx + 1, std::pair<K,V>(a_key, a_val));
    }
  }
}

// Removes a key-value pair from the collection
template<typename K, typename V>
void BinSearchCollection<K,V>::remove(const K& a_key)
{
  size_t idx = 0;
  if(bin_search(a_key, idx))
  {
    kv_list.remove(idx);
  }
}

// Find and returns the value associated with the key
template<typename K, typename V>
bool BinSearchCollection<K,V>::find(const K& search_key, V& the_val) const
{
  size_t idx = 0;
  std::pair<K,V> tmp_list;

  // if the key exists in the list, then assign the value to the_val
  if(bin_search(search_key, idx))
  {
    kv_list.get(idx, tmp_list);
    the_val = tmp_list.second;
    return 1;
  }
  return 0;
}

// Find and returns each key >= k1 and <= k2
template<typename K, typename V>
void BinSearchCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const
{
  size_t idx;
  bool check = bin_search(k1, idx);

  std::pair<K,V> tmp_list;
  kv_list.get(idx, tmp_list);

  // only loops if the key is less than or equal to k2 and the idx is still valid
  while((tmp_list.first <= k2) && (idx < size()))
  {
    keys.add(tmp_list.first);
    idx++;
    kv_list.get(idx, tmp_list);
  }
}


template<typename K, typename V>
void BinSearchCollection<K,V>::keys(ArrayList<K>& all_keys) const
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
void BinSearchCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const
{
  keys(all_keys_sorted);
  //all_keys_sorted.sort();
}

// Returns the number of pairs in the collection
template<typename K, typename V>
size_t BinSearchCollection<K,V>::size() const
{
  return kv_list.size();
}

template<typename K, typename V>
bool BinSearchCollection<K,V>::bin_search(const K& key, size_t& index) const
{
  if(size() > 0)
  {
    int mid = 0;
    int high = size() - 1;
    int low = 0;

    // While low is in fact low
    while(low <= high)
    {
      mid = (high + low) / 2;
      
      K mid_key; 
      std::pair<K,V> tmp_list;
      
      // Assign the tmp key variable to the key of tmp_list
      kv_list.get(mid, tmp_list);
      mid_key = tmp_list.first;

      // Key has been found
      if(key == mid_key)
      {
        index = mid;
        return true;
      }
      else 
      {
        // key is less than the tmp key found
        if(key < mid_key)
        {
          // Mid has gone through the whole collection
          if(mid == 0)
          {
            index = mid;
            return false;
          }
          high = mid - 1;
        }
        // key is greater than the tmp key found
        if(key > mid_key)
        {
          low = mid + 1;
        }
      }
    }
    // the key was not found
    index = mid;
    return false;
  }
  index = 0;
  return false;
}

#endif