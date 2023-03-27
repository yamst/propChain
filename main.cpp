#include "transaction.h"
#include "block.h"
#include "sha256.h"
#include "property.h"
#include "hashTable.h"
#include "chain.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

#define INITIAL_TARGET "000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
#define TRIES_UPPER_THRESH 3000000
#define TRIES_LOWER_THRESH 100000






std::vector<block> main_chain;

HashTable<double> users; // key = id, value = money

HashTable<property> properties; //key = id, value = property

chain cha; 

std::string tar = INITIAL_TARGET;





const std::string WHITESPACE = " \n\r\t\f\v";
void init_data(const char* file1_in, const char* file2_in);
void handle_cmd(const char *cmd_line, transaction* curr_block, int* num_tx);
std::string getFirstWord(std::string word);
std::string _ltrim(const std::string& s);
std::string _rtrim(const std::string& s);
std::string _trim(const std::string& s);
void decreaseDiff();
void increaseDiff();
void pay_up();






//first argument is inital currencies file. Second is initial properties file
//first file: every line is <id> <amount> 
//second file: every line is <property_id> <value> <owner_id> - at the start every property is owned by a single person

int main(int argc, char *argv[]){ // argument is file with initial currencies of users.
    if (argc == 3){
      init_data(argv[1], argv[2]);
    } else  {
      init_data("input1default.txt","input2default.txt");
    }
    transaction* current_block_transactions = new transaction[MAX_TX_PER_BLOCK];
    int tx_counter = 0;
    
    
    while(true) {
      std::cout << "enter command or h for help > " ;
      std::string cmd_line;
      std::getline(std::cin, cmd_line) ;
      if(std::cin.eof() && cmd_line.empty()){
        return 0;
      }
      handle_cmd(cmd_line.c_str(), current_block_transactions, &tx_counter);
      //handle op performs transaction if legal
      if (tx_counter == MAX_TX_PER_BLOCK){
        std::cout << "Mining block...";
        block b(0, tar, cha.getLastHash(), current_block_transactions, tx_counter);
        tx_counter = 0;
        current_block_transactions = new transaction[MAX_TX_PER_BLOCK];
        int tries = 0;
        while(!b.attemptMine()){
          tries++;
        }
        cha.insertBlock(b);
        if (tries > TRIES_UPPER_THRESH){
          decreaseDiff();
        } else if (tries < TRIES_LOWER_THRESH){
          increaseDiff();
        }
        if (cha.getLength() % 3 == 0){
          pay_up();
        }
        std::cout << "Done!" <<std::endl;
      }
    }
    return 0;
}





void init_data(const char* file1_in, const char* file2_in){
  std::ifstream file1(file1_in), file2(file2_in);
  std::string line;

  while (std::getline(file1, line)) {
      std::istringstream iss(line);
      int key, value;
      if (!(iss >> key >> value)) {
        continue;
      }
      users.insert(key, value);
  }

  while (std::getline(file2, line)) {
      std::istringstream iss(line);
      int key, value, own_id;
      if (!(iss >> key >> value >> own_id)) {
        continue;
      }
      properties.insert(key, property(key, "", "", 0, 1, value, &own_id , 0, NULL, NULL));
  }
}

void handle_cmd(const char *cmd_line, transaction* curr_block, int* num_tx) 
{
  if (*cmd_line == '\0'){
    return;
  }
  std::stringstream ss;
  std::string cmd_s = _trim(std::string(cmd_line));
  std::string firstWord;
  ss << cmd_s;
  ss >> firstWord;

  if (firstWord.compare("h") == 0) {
    std::cout << "Blockchain Commands are: \n\tsell_shares <id_seller> <id_buyer> <property_id> <shares>" << std::endl;
    std::cout << "\tadd_tenant <tenant_id> <property_id> \n\tremove_tenant <tenant_id> <property_id>" << std::endl;
    std::cout << "\tsplit_shares <property_id> <split_factor>" << std::endl;//add
    std::cout << "Info commands are: \n\tprint_users \n\tprint_properties \n\tchain_status \n\ttarget_hash" <<std::endl;
    std::cout << "\tlast_header_hash" << std::endl;
  }
  else if (firstWord.compare("print_users") == 0) {
    users.printAll();
  }
  else if (firstWord.compare("print_properties") == 0) {
    properties.printAll();
  }

  else if (firstWord.compare("chain_status") == 0) {
    std::cout << "There are " << cha.getLength() << " blocks. There are " << *num_tx << " pending transactions." << std::endl;
  }

  else if (firstWord.compare("target_hash") == 0) {
    std::cout << "Difficulty is: " << tar << std::endl;
  }

  else if (firstWord.compare("split_shares") == 0) {
    int propID, splitFAC;
    if (ss >> propID >> splitFAC){
      property* prop = properties.getData(propID);
      if (prop != nullptr){
        prop -> splitshares(splitFAC);
        transaction tx = transaction(SPLIT, propID, 0, 0, 0);
        curr_block[(*num_tx)++] = tx;
      } else {
        std::cout << "NO SUCH PROPERTY" << std::endl;
      }
    } else  {std::cout << "BAD COMMAND" <<std::endl;}
  }
  else if (firstWord.compare("sell_shares") == 0){
    int seller_id, buyer_id, prop_id, shares;
    if (ss >> seller_id >> buyer_id >> prop_id >> shares){
      property* prop = properties.getData(prop_id);
      double *seller_money = users.getData(seller_id), *buyer_money = users.getData(buyer_id);
      if (seller_money != nullptr && buyer_money != nullptr && prop != nullptr){
        if (*buyer_money >= prop -> getCost(shares)){
          if (prop -> tryTransferShares(buyer_id, seller_id, shares)){
            (*buyer_money) -=prop -> getCost(shares);
            (*seller_money) +=prop -> getCost(shares);
            transaction tx = transaction(SELL, prop_id, buyer_id, seller_id, shares);
            curr_block[(*num_tx)++] = tx;

          }else  {std::cout << "NOT ENOUGH SHARES" <<std::endl;}
        }else  {std::cout << "BUYER IS BROKE" <<std::endl;}
      }else  {std::cout << "NO SUCH ID" <<std::endl;}
    } else  {std::cout << "BAD COMMAND" <<std::endl;}
  }

  else if (firstWord.compare("add_tenant") == 0){
    int tenant, proper;
    if (ss >> tenant >> proper){
      property* prop = properties.getData(proper);
      if (proper){
        if (prop -> addtenant(tenant)){
          transaction tx = transaction(NEWTENANT, proper, tenant, 0, 0);
          curr_block[(*num_tx)++] = tx;
        }else {std::cout << "tenant already exists" << std::endl;}
      }else  {std::cout << "NO SUCH ID" <<std::endl;}
    } else  {std::cout << "BAD COMMAND" <<std::endl;}
  }

  else if (firstWord.compare("remove_tenant") == 0){
    int tenant, proper;
    if (ss >> tenant >> proper){
      property* prop = properties.getData(proper);
      if (proper){
        if (prop -> removetenant(tenant)){
          transaction tx = transaction(BYETENANT, proper, tenant, 0, 0);
          curr_block[(*num_tx)++] = tx;
        }else {std::cout << "tenant already exists" << std::endl;}
      }else  {std::cout << "NO SUCH ID" <<std::endl;}
    } else  {std::cout << "BAD COMMAND" <<std::endl;}
  }
  else if (firstWord.compare("last_header_hash") == 0){
    std::cout << cha.getLastHash() << std::endl;
  }
  else{
    std::cout << "beep bop bad command" << std::endl;
  }
}


std::string getFirstWord(std::string word)
{
  return word.substr(0, word.find_first_of(" \n"));   
}

std::string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

void increaseDiff(){

  int length = tar.length();
  for (int i = 0  ; i < length ; i++){
    if (tar[i] == 'f'){
      tar[i] = '0';
      break;
    }
  }
}

void decreaseDiff(){

  int length = tar.length();
  for (int i = length -1  ; i >= 0 ; i--){
    if (tar[i] == '0'){
      tar[i] = 'f';
      break;
    }
  }
}
//ADD MONEY TO OWNERS
void pay_up(){
  HashTable<property>::Node* temp;
  for (int i = 0 ; i < properties.m_list_length ; i++){
    temp = (properties.m_list)[i];
    while (temp != nullptr){
      property* prop = &(temp -> m_data);
      if (prop -> m_number_of_tenants){

        double fee = prop -> m_total_rent_fee/ prop -> m_number_of_tenants;
        for (int i = 0 ; i < prop -> m_number_of_tenants ; i++){
          int id = prop -> m_tenants[i];
          double* money = users.getData(id);
          if (money){//assuming a tenant exists. otherwise big problem
              if (*money < fee){
                  prop -> removetenant(id);
              }
              *money -= fee;
          }
        }//tenants pay

        for (int i = 0 ; i < prop -> m_shares ; i++){
          int id = prop -> m_owners[i];
          double* money = users.getData(id);
          *money += prop -> m_total_rent_fee/ prop -> m_shares;
        }//owners receive

      }//tenants exist
      temp = temp -> m_next;
    }
  }
}