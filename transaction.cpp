#include "transaction.h"
#include "property.h"



transaction::transaction():
m_type(INVALID), m_property_id(INVALID_PROPERTY), m_buyer(0), m_seller(0), m_shares(0){}


transaction::transaction(tx_type type, int prop_id, int buyer, int seller, int shares):
m_type(type), m_property_id(prop_id), m_buyer(buyer), m_seller(seller), m_shares(shares){}
