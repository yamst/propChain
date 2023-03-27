#ifndef CHAIN_H_
#define CHAIN_H_


#include "block.h"


class chain{

    class Node{

        public:
        Node(const block& block, Node* next = NULL): m_block(block), m_next(next){}

        ~Node() = default;

        void update_next(Node* new_next){
            m_next = new_next;
        }

        friend chain;

        private:
        block m_block;
        Node* m_next;
    };

    public:

    chain(): m_first(NULL), m_last(NULL), m_length(0){
        Node* node = new Node(block(0, EASIEST, EASIEST, NULL, 0));
        m_first = node;
        m_last = node;
    }

    ~chain(){
        Node *iter = m_first, *helper = NULL;
        while(iter != NULL){
            helper = iter -> m_next;
            delete iter;
            iter = helper;
        }
    }

    void insertBlock(const block& b){
        Node* node = new Node(b);
        m_last -> update_next(node);
        m_last = node;
        m_length++;
        
    }

    std::string getLastHash(){
        return m_last -> m_block.getHeaderHash();
    }

    int getLength(){
        return m_length;
    }

    private:
    int m_length;
    Node* m_first;
    Node* m_last;
};

#endif