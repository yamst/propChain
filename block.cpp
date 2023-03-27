#include "block.h"
#include <time.h>
#include "SHA256.h"
#include <string>
#include <cstring>
#include <iostream>


block::block(long long nonce, std::string target, std::string prev_hash, transaction* transactions, int n, bool prealloc):
m_num_transactions(n), m_transactions(transactions), m_accumulative_tx_hash((void*)m_transactions, n*sizeof(transaction)),
m_head(nonce, target, prev_hash, m_accumulative_tx_hash.getHash()){
    if (n > MAX_TX_PER_BLOCK){
        throw too_many_TX();
    }
    if (!prealloc){
        m_transactions = new transaction[MAX_TX_PER_BLOCK];
        for (int i = 0 ; i < n ; i++){
            m_transactions[i] = transactions[i];
        }
    }
    m_computed_header_hash = attemptMine();
}

block::~block(){
    delete[] m_transactions;
}

block::block(const block& b): m_num_transactions(b.m_num_transactions), m_head(b.m_head), m_transactions(b.m_transactions),
m_computed_header_hash(b.m_computed_header_hash),
m_accumulative_tx_hash((void*)m_transactions, m_num_transactions*sizeof(transaction)){
    m_transactions = new transaction[MAX_TX_PER_BLOCK];
    for (int i = 0 ; i < m_num_transactions ; i++){
         m_transactions[i] = b.m_transactions[i];
    }
}



block::header::header(long long nonce, std::string target, std::string prev_hash, std::string root_hash):
m_nonce(nonce), m_target(target), m_previous_hash(prev_hash), m_timestamp(time(NULL)), m_root_hash(root_hash){}



void block::incNonce(){
    m_head.m_nonce++;
}

bool block::attemptMine(){
    incNonce();
    SHA256 calculated;
    m_head.m_timestamp = time(NULL);
    calculated.add(&(m_head.m_nonce), sizeof(long long));
    calculated.add(&(m_head.m_timestamp), sizeof(time_t));
    calculated.add((const void*)(m_head.m_target.c_str()), m_head.m_target.length());
    calculated.add((const void*)(m_head.m_previous_hash.c_str()), m_head.m_previous_hash.length());
    calculated.add((const void*)(m_head.m_root_hash.c_str()), m_head.m_root_hash.length());
    m_computed_header_hash = calculated.getHash();
    return ((strcmp(m_computed_header_hash.c_str(), m_head.m_target.c_str()))<= 0);
}

std::string block::getHeaderHash(){
    return m_computed_header_hash;
}


void block::insertTransaction(transaction tx){
    if (this -> m_num_transactions > MAX_TX_PER_BLOCK){
        throw too_many_TX();
    }
    this -> m_transactions[m_num_transactions++] = tx;
    m_accumulative_tx_hash.add(&tx, sizeof(transaction));
    m_head.m_root_hash = m_accumulative_tx_hash.getHash();
}
