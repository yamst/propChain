Documentation of blockchain:

This code is a simplified version of a complete blockchain for real estate.
We planned on adding all the important features of a blockchain step by step. 
However, we switched to a smart contracts project midway.

Therefore, there are limitations in this blockchain, and they are as follows:
    1. Encrption is not added yet, meaning transactions aren't signed by sellers/buyers/govt.
    2. Sha256 hash library is imported (with a few tweaks) and not our proprietery.
    3. Transactions in each block are organized in what we call an "N-way Merkel Tree"; N is the number of transactions in the block and a single hash of all of them is calculated and included in the header.
    4. Communication isn't added. Currently, main.cpp runs as a full-node/miner and its operations are received via a command prompt and not from other nodes in the network. As there are is no communication, in reality, a single chain is created and there are no fork conflicts.

That being said, the consensus rules are listed below:
 - genesis block: In our blockchain, the files input1 and input2 act as a genesis block and they include initial information about how much money every person has, and all properties and their initial owners respectively. Replacement files can be supplied at compilation for the purpose of experimenting with the blockchain.
 - Mining: the blockchain uses a PoW mechanism where the inital target is 000f...f and is adjusted after each block if the time it took to mine it was too long or too short.
 - Blocks: Blocks include an array of transactions and a header with metadata similar to bitcoin. The number of txs must not be greater than MAX_TX_PER_BLOCK(3). Our application (main.cpp) which implements this protocol, waits until this number of transactions has been gathered and then tries mining a block with them.
 - transactions: Transactions in our blockchain can be to add or remove a tenant, to sell shares of a property, or to split shares. Every such opeartion is saved as a transaction and added to the next block in the chain. 
 - Property owning mechanism: every property has a list of owners where an owner can appear multiple times (for owning multiple shares). The total value of a property is independent of the number of shares.
 - Property Usage: every property has a list of current tenants. (trying to add a tenant that already lives in the property will cause an invalid transaction). Once every three blocks, the rent fee is reduced from all tenant accounts of all properties. Rent fee of a property equals the property value divided by 400 (based on real life observations) and will be paid equaly among the tenants. Not having enough money will cause a negative balance and getting thrown out. If no tenants are present  in a property then no rent will be paid.

The application (which implements the above protocol):
As stated, main.cpp is a lone client/full-node/miner. Try compiling the project using "g++ *.cpp" in the main directory and then running it. The command line supports some blockchain commands as well as queries, and provides an explenation. Bad input handeling partially exists. I do not recommend testing your luck with it. All blockchain commands are added as transactions to blocks. This is perhaps the most integral part as the chain is the official record of events while the users/properties tables are just there to keep track of the system-state locally

input file format :
first file is inital currencies file. Second is initial properties file
first file: every line is <id> <amount> 
second file: every line is <property_id> <value> <owner_id> - at the start every property is owned by a single person

