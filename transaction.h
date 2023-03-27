#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#define FAILED_EXECUTION -1

enum tx_type {SPLIT, SELL, NEWTENANT, BYETENANT, INVALID};



class transaction{

    public: 

    transaction(tx_type type, int prop_id, int buyer, int seller, int shares);

    transaction(const transaction&) = default;

    transaction(); // invalid spotholder

    ~transaction() = default;


    private:

    tx_type m_type;

    int m_property_id;

    int m_buyer;

    int m_seller;

    int m_shares;
    

};

#endif /* TRANSACTION_H_ */