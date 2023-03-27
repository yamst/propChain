#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <assert.h>
#include <iostream>
#include "property.h"

/**
 * HashTable : a data structure that supports insert, remove, and find operations for data elements with integer keys
 * with an average input estimated time complexity of O(1).
 **/
template <class T>
class HashTable{

    public:

    /**
     * default constructor : creates an empty hash table
    **/
    HashTable ();

    /**
     * destructor : deletes the hashtable and all of its allocated data
    **/
    ~HashTable ();

    /**
     * insert : inserts a new key-data pair into the table. does nothing if key is already in table.
     * @param key - an integer value that will be used to search for the data.
     * @param data - a constant refernce to the added data. a copy of it will be made and saved.
     * @return a reference to the structure to enable chains.
    **/
    HashTable<T>& insert (int key, const T& data);

    /**
     * isFound : checks if a key is in the table.
     * @param key - the key that is looked for.
     * @return true if the key is found in the table. false otherwise.
    **/
    bool isFound (int key) const;

    /**
     * getData : gets a pointer to the data attached to a key in the table or nullptr if the key isn't found.
     * @param key - the key that is looked for.
     * @return a pointer to the data element in the table if found (NOT a copy). nullptr if the key isn't found.
    **/
    T* getData (int key);

    /**
     * remove : removes a key and its attached data from the table. does nothing if the key isn't found.
     * @param key - the key to be removed.
     * @return a reference to the table to enable chains.
    **/
    HashTable<T>& remove (int key);

    /**
     * print : prints the contents of each slot in the hashtable in order.
     * @return a reference to the table to enable chains.
    **/
    HashTable<T>& printAll();

    friend void pay_up();

    
    private: 

    /* class to organize elements in each array cell. contains key, data, and pointer to next. */
    class Node;

    /* removes a key from the table and doesn't check if the list needs to be realloced (into a smaller space). */
    bool removeKeyOnly(int key);

    /* inserts a given node pointer while changing only its 'next' field. doesn't check if list needs to be resized. */
    static void insertNode(HashTable<T>::Node** list, int list_length, HashTable<T>::Node* node);

    /* takes integer a to any natural power. */
    static int power (int a, int b);

    /* default table size */
    static const int sm_default_size = 11;

    /* by how much will the table grow when it needs to be resized. */
    static const int sm_inc_factor = 4;

    /* current length of hash array. */
    int m_list_length;

    /* current number of elements in the table. */
    int m_size;

    /* an array of all lists in the table. */
    HashTable<T>::Node** m_list;

};

/*************************************************************
**************HASHTABLE<T> BEGINNING OF METHODS IMPLEMENTATION
*************************************************************/


template <class T>
int HashTable<T>::power (int a, int b){
    int to_return = 1;
    while (b-- > 0){
        to_return *= a;
    }
    return to_return;
}

template <class T>
HashTable<T>::HashTable () : m_list_length(sm_default_size), m_size(0), m_list(new HashTable<T>::Node*[sm_default_size])
{
    for (int i = 0 ; i < sm_default_size ; i++){
        m_list[i] = nullptr;
    }
}

template <class T>
HashTable<T>::~HashTable ()
{
    for (int i = 0 ; i < m_list_length ; i++){
        if (m_list[i] != nullptr){
            m_list[i] -> destroy();
        }
    }
    delete[] m_list;
}

template <class T>
HashTable<T>& HashTable<T>::insert (int key, const T& data)
{
    if (this -> isFound(key)){
        return *this;
    }
    HashTable<T>::Node* to_insert = new Node(key, data);
    if (this -> m_size >= sm_inc_factor * this -> m_list_length){
        HashTable<T>::Node** new_list = nullptr;
        try{
            new_list = new HashTable<T>::Node*[sm_inc_factor * this -> m_list_length];
        }   catch(...)  {
            delete to_insert;
            throw;
        }
        HashTable<T>::Node *temp1, *temp2;
        for (int i = 0 ; i < sm_inc_factor * this -> m_list_length ; i++){
            new_list[i] = nullptr;
        }
        for (int i = 0 ; i < this -> m_list_length ; i++){
            temp1 = this -> m_list[i];
            while (temp1 != nullptr){
                temp2 = temp1 -> m_next;
                insertNode(new_list, sm_inc_factor * this -> m_list_length, temp1);
                temp1 = temp2;
            }
        }
        this -> m_list_length = sm_inc_factor * this -> m_list_length;
        delete[] this -> m_list;
        this -> m_list = new_list;
    }
    this -> m_size += 1;
    insertNode(this -> m_list, this -> m_list_length, to_insert);
    return *this;
}

template <class T>
void HashTable<T>::insertNode(HashTable<T>::Node** list, int list_length, HashTable<T>::Node* node)
{
    int cell = (node -> m_key % (list_length));
    HashTable<T>::Node* temp = list[cell];
    list[cell] = node;
    node -> m_next = temp;
}


template <class T>
bool HashTable<T>::isFound (int key) const
{
    int cell = (key % (this -> m_list_length));
    if ((this -> m_list)[cell] == nullptr){
        return false;
    }  
    return ((this -> m_list)[cell]) -> isFound(key);
}

template <class T>
T* HashTable<T>::getData (int key)
{
    int cell = (key % (this -> m_list_length));
    if ((this -> m_list)[cell] == nullptr){
        return nullptr;
    }  
    return ((this -> m_list)[cell]) -> getData(key);
}

template <class T>
HashTable<T>& HashTable<T>::remove (int key)
{
    if (!(this -> isFound(key))){
        return *this;
    }
    if ((this -> m_list_length > sm_default_size) && (this -> m_size <= (this -> m_list_length)/sm_inc_factor)){
        HashTable<T>::Node** new_list = new HashTable<T>::Node*[(this -> m_list_length)/sm_inc_factor];
        HashTable<T>::Node *temp1, *temp2;
        for (int i = 0 ; i <  (this -> m_list_length)/sm_inc_factor ; i++){
            new_list[i] = nullptr;
        }
        for (int i = 0 ; i < this -> m_list_length ; i++){
            temp1 = this -> m_list[i];
            while (temp1 != nullptr){
                temp2 = temp1 -> m_next;
                insertNode(new_list, (this -> m_list_length)/sm_inc_factor, temp1);
                temp1 = temp2;
            }
        }
        this -> m_list_length = (this -> m_list_length)/sm_inc_factor;
        delete[] this -> m_list;
        this -> m_list = new_list;
    }
    this -> m_size -= 1;
    bool removed_ver = this -> removeKeyOnly(key);
    if (removed_ver == false || this -> m_size < 0){
        assert(false);
    }
    return *this;
}

template <class T>
bool HashTable<T>::removeKeyOnly(int key){
    int cell = (key % (this -> m_list_length));
    HashTable<T>::Node *iterator = this -> m_list[cell], *temp = nullptr;
    if (iterator == nullptr){
        return false;
    }
    if (iterator -> m_key == key){
        temp = iterator -> m_next;
        delete iterator;
        this -> m_list[cell] = temp;
        return true;
    }   
    while (iterator -> m_next != nullptr){
        if (iterator -> m_next -> m_key == key){
            temp = iterator -> m_next -> m_next;
            delete iterator -> m_next;
            iterator -> m_next = temp;
            return true;
        }   else    {
            iterator = iterator -> m_next;
        }
    }
    return false;
}

template <class T>
HashTable<T>& HashTable<T>::printAll(){
    HashTable<T>::Node* temp;
    for (int i = 0 ; i < this -> m_list_length ; i++){
        temp = (this -> m_list)[i];
        while (temp != nullptr){
            std::cout << "id: " << temp -> m_key << ".\tinfo: " << temp -> m_data << std::endl;
            temp = temp -> m_next;
        }
    }
    return *this;
}








/**********************************************************
**************HASHTABLE<T> ENDING OF METHODS IMPLEMENTATION
**********************************************************/




/***********************************************************
**************HASHTABLE<T>::NODE BEGINNING OF IMPLEMENTATION
***********************************************************/

template <class T>
class HashTable<T>::Node{

    private:

    friend class HashTable<T>;

    friend void pay_up();

    Node (int key, const T& data, HashTable<T>::Node* next = nullptr);

    ~Node () = default;

    void destroy ();

    bool isFound (int key) const;

    T* getData (int key);

    int m_key;

    T m_data;

    HashTable<T>::Node* m_next;
};

template <class T>
HashTable<T>::Node::Node (int key, const T& data, HashTable<T>::Node* next):
m_key(key), m_data(data), m_next(next)  {}

template <class T>
void HashTable<T>::Node::destroy ()
{
    if (this -> m_next != nullptr){
        this -> m_next -> destroy();
    }
    delete this;
}

template <class T>
bool HashTable<T>::Node::isFound (int key) const
{
    if (this -> m_key == key){
        return true;
    }
    if (this -> m_next == nullptr){
        return false;
    }
    return (this -> m_next -> isFound(key));
}

template <class T>
T* HashTable<T>::Node::getData (int key)
{
    if (this -> m_key == key){
        return &(this -> m_data);
    }
    if (this -> m_next == nullptr){
        return nullptr;
    }
    return (this -> m_next -> getData(key));
}

/********************************************************
**************HASHTABLE<T>::NODE ENDING OF IMPLEMENTATION
********************************************************/



#endif /* HASHTABLE_H_ */
