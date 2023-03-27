#include "property.h"
#include <iostream>
#include "HashTable.h"

property::property(int prop_id, std::string city, std::string street, int house_num, int shares, double total_value,
int* owners, int num_of_tenants, int* tenants, Metadata* addit_info):
m_property_id(prop_id), m_city(city), m_street(street), m_house_number(house_num), m_shares(shares), 
m_total_value(total_value), m_owners(NULL), m_number_of_tenants(num_of_tenants), m_tenants(NULL), m_info(*addit_info),
m_total_rent_fee(total_value /400.0)
{
    m_owners = new int[shares];
    m_tenants = new int[num_of_tenants];
    for (int i = 0 ; i < shares ; i++){
        m_owners[i] = owners[i];
    }
    for (int i = 0 ; i < num_of_tenants ; i++){
        m_tenants[i] = tenants[i];
    }
}


property::property(const property& prop): m_property_id(prop.m_property_id), m_city(prop.m_city), m_street(prop.m_street),
m_house_number(prop.m_house_number), m_shares(prop.m_shares), m_total_value(prop.m_total_value),
m_total_rent_fee(prop.m_total_rent_fee),
m_owners(NULL), m_number_of_tenants(prop.m_number_of_tenants), m_tenants(NULL), m_info(prop.m_info){
    m_owners = new int[m_shares];
    m_tenants = new int[m_number_of_tenants];
    for (int i = 0 ; i < m_shares ; i++){
        m_owners[i] = prop.m_owners[i];
    }
    for (int i = 0 ; i < m_number_of_tenants ; i++){
        m_tenants[i] = prop.m_tenants[i];
    }

}

property::~property(){
    delete[] m_owners;
    delete[] m_tenants;
}

void property::splitshares(int splitting_constant){

    int* new_owners = new int[splitting_constant*(this -> m_shares)];
    
    for (int i = 0 ; i < (this -> m_shares) ; i++){
        for (int j = 0 ; j < splitting_constant ; j++){
            new_owners[(splitting_constant)*i +j] = (this -> m_owners)[i];
        }
    }
    delete[] (this -> m_owners);
    this -> m_owners = new_owners;
    this -> m_shares = (this -> m_shares) * splitting_constant;
    
}


bool property::tryTransferShares(int buyer, int seller, int num_of_shares){
    int seller_shares = 0;
    for (int i = 0 ; i < m_shares ; i++){
        if (m_owners[i] == seller){
            seller_shares++;
        }
    }
    if (seller_shares < num_of_shares){
        return false;
    }
    for (int i = 0 ; i < m_shares && num_of_shares > 0; i++){
        if (m_owners[i] == seller){
            m_owners[i] = buyer;
            num_of_shares--;
        }
    }
    return true;

}

bool property::addtenant(int tenant_id){
    for (int i = 0 ; i < m_number_of_tenants ; i++){
        if (m_tenants[i] == tenant_id){
            return false;
        }
    }
    int* new_list = new int[++m_number_of_tenants];
    new_list[m_number_of_tenants - 1] = tenant_id;
    for (int i = 0 ; i < m_number_of_tenants - 1 ; i++){
        new_list[i] == m_tenants[i];
    }
    delete[] m_tenants;
    m_tenants = new_list;
    return true;
}

bool property::removetenant(int tenant_id){
    bool is_found = false;
    int index = 0;
    for (int i = 0 ; i < m_number_of_tenants ; i++){
        if (m_tenants[i] == tenant_id){
            is_found = true;
            index = i;
        }
    }
    if (!is_found) {return false;}


    int* new_list = new int[--m_number_of_tenants];

    for (int i = 0 ; i < index ; i++){
        new_list[i] == m_tenants[i];
    }

    for (int i = index ; i < m_number_of_tenants ; i++){
        new_list[i] == m_tenants[i+1];
    }
    delete[] m_tenants;
    m_tenants = new_list;
    return true;

}




std::ostream& operator<<(std::ostream& os, const property& prop){
    os << "Total value: " << prop.m_total_value << "; no. shares: " << prop.m_shares << "; owner_ids: ";
    for (int i = 0 ; i < prop.m_shares ; i ++){
        os << (prop.m_owners)[i];
        if (i < prop.m_shares - 1)  os << ", ";
    }
    return os;
}