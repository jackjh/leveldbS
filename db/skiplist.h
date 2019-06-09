#ifndef LEVELDB_DB_SKIPLIST_H_
#define LEVELDB_DB_SKIPLIST_H_

#include <stdlib.h>
#include <assert.h>
#include "util/arena.h"
#include "util/random.h"

namespace leveldb {

class Arena;

template<typename Key, class Comparator>
class SkipList {
private:
    struct ListNode;

public: 
    explicit SkipList(Comparator comparator, Arena* arena);

    // Insert the key(key-value(memtable)) into the skiplist
    void Insert(const Key& key);

    // if there is a entry(key-value) in the skiplist, return true
    bool Contains(const Key& key) const;

    // iteration over a skiplist
    class Iterator {
    public: 
        explicit Iterator(const SkipList* skiplist);

        // if the node which is iterating is a valid node, return true
        bool Valid() const;

        // return the key of the Node in the skiplist at the current position
        const Key& GetKey() const;

        // advances to the next position (next Node)
        void Next();

        // advances to the previous position
        void Prev();

        // advances to the first entry(key-value) with the key >= key
        void Seek(const Key& key);

        // seek to the position at the first entry in the skiplist
        void SeekToFirst();

        // seek to the position at the last entry in the skiplist
        void SeekToLast();

    private:
        const SkipList* slist;
        ListNode* node;
    };

private:
    static const int MaxLevels = 12;

    Comparator const _comparator;
    Arena* const _arena;        // used for allocation of listNodes
    
    Random _rnd;

    ListNode* const head;

    int levels;      // levels(height) of the skiplist, modified only by Insert().

    inline int GetHeight() const {
        return levels;
    }

    bool IsEqual(const Key& a, const Key& b) const {
        return (_comparator(a, b) == 0);
    }

    ListNode* createNode(const Key& key, int levels);

    int RandomLevels();

    // if the key is greater than the data stored in node.
    bool KeyIsGreaterThanNode(const Key& key, ListNode* node) const;

    // return the earliest ListNode that comes at or after key, if there is no such node, return NULL.
    ListNode* FindGreater_Equal(const Key& key, ListNode** prev) const;

    // return the lastest ListNode with a key < key, if there is no such node, return head.
    ListNode* FindLessThanNode(const Key& key) const;

    // return the lastest ListNode in the skiplist, if the skiplist is empty, return head.
    ListNode* FindLastNode() const;

    SkipList(const SkipList&);
    void operator=(const SkipList&);
};


template<typename Key, class Comparator>
struct SkipList<Key, Comparator>::ListNode {
    const Key key;
    explicit ListNode(const Key& _key) : key(_key) { }

    ListNode* Next(int n) {
        assert(n >= 0);
        return next_node[n];
    }

    void SetNext(ListNode* x, int n) {
        assert(n >= 0);
        next_node[n] = x;
    }

    // AtomicPointer

    private: 
        ListNode* next_node[1];
};

template<typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(Comparator cmp, Arena* arena) :
    _comparator(cmp), 
    _arena(arena), 
    head(createNode(0, MaxLevels)),
    levels(1), 
    _rnd(0xdeadbeef) {
   for(int i = 0; i < MaxLevels; i++) {
       head->SetNext(NULL, i);
   }
}

template<typename Key, class Comparator>
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList* skiplist) {
    slist = skiplist;
    node = NULL;
}

template<typename Key, class Comparator>
inline bool SkipList<Key, Comparator>::Iterator::Valid() const {
    return node != NULL;
}

template<typename Key, class Comparator>
inline const Key& SkipList<Key, Comparator>::Iterator::GetKey() const {
    assert(Valid());
    return node->key;
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Next() {
    assert(Valid());
    node = node->Next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Prev() {
    assert(Valid());
    node = slist->FindLessThanNode(node->key);
    if(node == slist->head) {
        node = NULL;
    }
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key& key) {
    node = slist->FindGreater_Equal(key, NULL);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToFirst() {
    node = slist->head->Next(0);
}

template<typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToLast() {
    node = slist->FindLastNode();
    if(node == slist->head) {
        node = NULL;
    }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::ListNode* SkipList<Key, Comparator>::createNode(const Key& key, int levels) {
    char* area = _arena->AllocateAligned(sizeof(ListNode) + (levels - 1) * sizeof(ListNode*));
    return new(area)ListNode(key);
}

template<typename Key, class Comparator>
int SkipList<Key, Comparator>::RandomLevels() {
    static const unsigned int branching = 4;
    int levels = 1;
    while(levels < MaxLevels && ((_rnd.Next() % branching) == 0)) {
        levels++;
    }

    assert(levels > 0);
    assert(levels <= MaxLevels);

    return levels;
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::KeyIsGreaterThanNode(const Key& key, ListNode* node) const {
    return ((node != NULL) && _comparator(node->key, key) < 0);
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::ListNode* SkipList<Key, Comparator>::FindGreater_Equal(const Key& key, ListNode** prev) const {
    int level = GetHeight() - 1;
    ListNode* tmp = head;
    
    while(true) {
        ListNode* next = tmp->Next(level);
        if(KeyIsGreaterThanNode(key, next)) {
            tmp = next;
        }
        else {
            if(prev != NULL) prev[level] = tmp;
            if(level == 0){     // the lowest level link
                return next;
            }
            else {
                level--;
            }
        }
    }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::ListNode* SkipList<Key, Comparator>::FindLessThanNode(const Key& key) const {
    int level = GetHeight() - 1;
    ListNode* tmp = head;

    while(true) {
        assert(tmp == head || _comparator(tmp->key, key) < 0);
        ListNode* next = tmp->Next(level);
        if(next == NULL || _comparator(next->key, key) >= 0) {
            if(level == 0) {
                return tmp;
            }
            else {
                level--;
            }
        }
        else {
            tmp = next;
        }
    }
}

template<typename Key, class Comparator>
typename SkipList<Key, Comparator>::ListNode* SkipList<Key, Comparator>::FindLastNode() const {
    int level = GetHeight() - 1;
    ListNode* tmp = head;

    while(true) {
        ListNode* next = tmp->Next(level);
        if(next == NULL) {
            if(level == 0) {
                return tmp;
            }
            else {
                level--;
            }
        }
        else {
            tmp = next;
        }
    }
}

template<typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert(const Key& key) {
    ListNode* prev[MaxLevels];
    ListNode* node = FindGreater_Equal(key, prev);

    assert(node != NULL || !IsEqual(key, node->key));

    int rnd_levels = RandomLevels();
    if(rnd_levels > GetHeight()) {
        for(int i = GetHeight(); i < rnd_levels; i++) {
            prev[i] = head;
        }
        levels = rnd_levels;
    }

    node = createNode(key, rnd_levels);
    for(int i = 0; i < rnd_levels; i++) {
        node->SetNext(prev[i]->Next(i), i);
        prev[i]->SetNext(node, i);
    }
}

template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& key) const {
    ListNode* node = FindGreater_Equal(key, NULL);
    if(node != NULL && IsEqual(key, node->key)) {
        return true;
    }
    else {
        return false;
    }
}

}


#endif