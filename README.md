# Real Estate Blockchain (C++ Implementation)

This codebase represents a simplified version of a complete blockchain for real estate transactions. While we initially intended to add all crucial features to this implementation, we decided to switch to a smart contracts project midway through the development process.

## Limitations:

1. No encryption: Transactions are not signed by sellers, buyers, or government entities.
2. Imported Sha256 hash library: The Sha256 hash library was imported and slightly modified, rather than developed as a proprietary solution.
3. N-way Merkle Tree: Transactions in each block are organized using an N-way Merkle Tree, where N is the number of transactions in the block. A single hash of all transactions is calculated and included in the block header.
4. No node communication: Main.cpp runs as a full-node/miner, with operations received via a command prompt rather than from other nodes in the network. This results in a single chain without fork conflicts.

## Consensus Rules:

* Genesis block: Input1 and Input2 files serve as the genesis block, containing initial information about each individual's funds and all properties with their initial owners. Replacement files can be provided for experimentation purposes.
* Mining: The blockchain employs a Proof of Work (PoW) mechanism with an initial target of 000f...f. The target is adjusted after each block based on the time taken to mine.
* Blocks: Blocks consist of an array of transactions and a header containing metadata similar to Bitcoin. The number of transactions must not exceed MAX_TX_PER_BLOCK (3). The application (main.cpp) implementing this protocol waits until the maximal required number of transactions is collected before attempting to mine a block.
* Transactions: Transactions in this blockchain can add or remove tenants, sell property shares, or split shares. Each operation is stored as a transaction and added to the next block in the chain.
* Property Ownership: Properties have a list of owners, with each owner potentially appearing multiple times if they own multiple shares. The total value of a property is independent of the number of shares.
* Property Usage: Each property maintains a list of current tenants. Adding an existing tenant will result in an invalid transaction. Rent fees are deducted from all tenant accounts for all properties every three blocks. The rent fee for a property is equal to the property value divided by 400 (based on real-life observations) and is paid equally among tenants. Insufficient funds lead to a negative balance and eviction. No rent is collected if a property has no tenants.

## Application:

Main.cpp serves as a standalone client, full-node, and miner. Compile the project using "g++ *.cpp" in the main directory and run the executable. The command-line interface supports various blockchain commands and queries, with some explanations provided. Partial error handling is in place, but deliberately attempting to cause issues is not recommended. All blockchain commands are added as transactions to blocks, making the chain the official record of events, while user and property tables maintain the system's local state.

## Input File Format:

There are two input files: the initial currencies file (first file) and the initial properties file (second file).
* First file: Each line is formatted as <id> <amount>, representing an individual's initial funds.
* Second file: Each line is formatted as <property_id> <value> <owner_id>. At the start, every property is owned by a single person.
By modifying these input files, you can experiment with different initial states for the blockchain.

## Conclusion:

This README provides an overview of our initial C++ implementation of a real estate blockchain, including its limitations, consensus rules, and the application's functionality. While we eventually transitioned to a smart contracts-based approach, this project still serves as a valuable learning experience and a foundation for understanding the principles of blockchain technology in the real estate domain. Feel free to explore the code, experiment with different initial states, and gain insights into the workings of a simplified real estate blockchain system.