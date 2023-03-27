#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <string>
#include <iostream>
//special owners:
#define GOVT_OWNED -1
#define FIRM_1 -2
#define FIRM_2 -3

#define INVALID_PROPERTY

class property{

    public:

    class Metadata{};

    property(int prop_id, std::string city, std::string street, int house_num, int shares, double total_value,
    int* owners, int num_of_tenants, int* tenants, Metadata* addit_info);

    property(const property& prop);

    ~property();

    void splitshares(int splitting_constant);

    double getCost(int number_of_shares){
        return m_total_value * (double)number_of_shares / (double)m_shares;
    }

    bool tryTransferShares(int buyer, int seller, int num_of_shares);

    bool addtenant(int tenant_id);

    bool removetenant(int tenant_id);

    

    friend std::ostream& operator<<(std::ostream& os, const property& prop);

    friend void pay_up();

    private:

    int m_property_id; //we will assume properties have identifiers

    std::string m_city;

    std::string m_street;

    int m_house_number;

    int m_shares; // number of shares. must be positive

    double m_total_value; // total value of property. needs to be updated monthly

    double m_total_rent_fee;

    int* m_owners; // an array of size @field shares of the ids of all owners

    int m_number_of_tenants; // how many tenants are living in the property

    int* m_tenants; // an array of size @field number_of_tenants of the ids of the tenants.
    
    Metadata m_info;

};





#endif /* PROPERTY_H_ */