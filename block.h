#ifndef BLOCK_H_
#define BLOCK_H_
#include "sha256.h"
#include "transaction.h"


#define MAX_TX_PER_BLOCK 3
#define HASH_HEX 64

#define EASIEST "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"

class block{

    public:

    class header{

        public:

        header(long long nonce, std::string target, std::string prev_hash, std::string root_hash);

        ~header() = default;

        header(const header& head) = default;

        friend block;


        private:

        long long m_nonce;

        std::string m_target;

        std::string m_previous_hash;

        time_t m_timestamp;

        std::string m_root_hash;

    };

    class too_many_TX{};

    // allocated_transactions, if prealloced, should be an array of transactions allocated by new[] 
    block(long long m_nonce, std::string target, std::string prev_hash, transaction* transactions,
    int num_of_transactions, bool prealloc = true);

    block(const block& b);

    ~block();

    bool attemptMine(); // updates timeStamp

    std::string getHeaderHash() ;

    void incNonce();

    void insertTransaction(transaction tx);

    
    public:

    int m_num_transactions;

    transaction* m_transactions;

    SHA256 m_accumulative_tx_hash;

    block::header m_head;

    std::string m_computed_header_hash; // filled only after succeful mine.

};





#endif /* BLOCK_H_ */