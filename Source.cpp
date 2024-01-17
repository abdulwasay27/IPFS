#pragma once
// Text colors
#define RESET       "\033[0m"   // Reset all attributes
#define BLACK       "\033[30m"  // Black text
#define RED         "\033[31m"  // Red text
#define GREEN       "\033[32m"  // Green text
#define YELLOW      "\033[33m"  // Yellow text
#define BLUE        "\033[34m"  // Blue text
#define MAGENTA     "\033[35m"  // Magenta text
#define CYAN        "\033[36m"  // Cyan text
#define WHITE       "\033[37m"  // White text

// Text color modifiers
#define BOLD        "\033[1m"   // Bold text
#define UNDERLINE   "\033[4m"   // Underlined text
#define REVERSED    "\033[7m"   // Reversed (background becomes foreground)

#include "Bigint.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>
#include<direct.h>
#include<stdlib.h>
using namespace std; 
#include"circularLinkedList.h"



std::string sha1(const std::string& input) {
	// Initialize SHA-1 context
	uint32_t h0 = 0x67452301;
	uint32_t h1 = 0xEFCDAB89;
	uint32_t h2 = 0x98BADCFE;
	uint32_t h3 = 0x10325476;
	uint32_t h4 = 0xC3D2E1F0;

	// Append padding to the message
	std::string msg = input;
	msg += static_cast<char>(0x80); // append the "1" bit

	// Append 0 bits until message length in bits ≡ 448 (mod 512)
	while ((msg.length() * 8) % 512 != 448) {
		msg += static_cast<char>(0x00);
	}

	// Append the original length in bits mod (2^64) to the end
	uint64_t originalLength = input.length() * 8;
	for (int i = 0; i < 8; ++i) {
		msg += static_cast<char>((originalLength >> (56 - 8 * i)) & 0xFF);
	}

	// Process the message in 512-bit blocks
	for (size_t i = 0; i < msg.length(); i += 64) {
		uint32_t w[80];

		// Break block into 16 words of 32 bits
		for (int j = 0; j < 16; ++j) {
			w[j] = (static_cast<uint32_t>(msg[i + j * 4 + 0]) << 24) |
				(static_cast<uint32_t>(msg[i + j * 4 + 1]) << 16) |
				(static_cast<uint32_t>(msg[i + j * 4 + 2]) << 8) |
				(static_cast<uint32_t>(msg[i + j * 4 + 3]) << 0);
		}

		// Extend the 16 words into 80 words
		for (int j = 16; j < 80; ++j) {
			w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
			w[j] = (w[j] << 1) | (w[j] >> 31);
		}

		// Initialize hash value for this chunk
		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;

		// Main loop
		for (int j = 0; j < 80; ++j) {
			uint32_t f, k;

			if (j < 20) {
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (j < 40) {
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (j < 60) {
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else {
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}

			uint32_t temp = ((a << 5) | (a >> 27)) + f + e + k + w[j];
			e = d;
			d = c;
			c = (b << 30) | (b >> 2);
			b = a;
			a = temp;
		}

		// Update hash value
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
	}

	// Produce the final hash result
	std::stringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(8) << h0
		<< std::setw(8) << h1
		<< std::setw(8) << h2
		<< std::setw(8) << h3
		<< std::setw(8) << h4;

	return ss.str();
}

Bigint generateUniqueId(string sha1Value, Bigint identifierSpace) {
	Bigint sha2Value = 0;
	int index = 0;
	while (sha1Value[index] != '\0') {
		sha2Value += sha1Value[index] + index;
		index++;
	}

	sha2Value = sha2Value % identifierSpace;
	return sha2Value;
}







class ringDHT {
public:
	CircularLinkedList* m;
	Bigint numMachines;
	Bigint identifierSpace;
	int identifierBit;
	
	string filePath = "C:\\Users\\abdul\\Desktop\\IPFS";
	
	ringDHT() {
		m = new CircularLinkedList;
		numMachines = 0;
		identifierSpace = 0;
		identifierBit = 0;
		fs::create_directory(filePath);

	}

	void userInput() {
		
		do {
			cout << YELLOW<< "\nEnter the identifier space: " << RESET;
			cin >> identifierBit;
			Bigint two = 2;
			identifierSpace = pow(two, identifierBit);
		} while (identifierSpace <= 0);

		do {
			cout << YELLOW << "Enter the number of machines: " << RESET;
			cin >> numMachines;
		} while (numMachines > identifierSpace );

		Bigint count = 0;
		char option = 'N';
		string machineName;
		Bigint id;
		while (count < numMachines) {
			count += 1;
			cout << "Do you wish to specify the machine " << count << " name ? (Y / N)";
			count -= 1;
			cin >> option;
			switch (option) {
				case 'Y':
				case 'y':  // Taking user input for machine ids
				{
					count += 1;
					cout << "Enter the name of the machine " << count << ": ";
					count -= 1;
					cin >> machineName;
					bool isAlreadyPresent = false;
					isAlreadyPresent = m->search(generateUniqueId(sha1(machineName), identifierSpace));
					Bigint factor = 1;
					 while (isAlreadyPresent){
						isAlreadyPresent = m->search((generateUniqueId(sha1(machineName), identifierSpace)+factor)%identifierSpace);
						factor++;
					 }
					 m->insert((generateUniqueId(sha1(machineName), identifierSpace) + (factor - 1)) % identifierSpace);

					break;
				}

				case 'N':
				case 'n':  // Automatically Assigning ids to machine
				{	
					count += 1;
					cout << "Do you wish to assign id to machine " << count << "? (Y / N)";
					count -= 1;
					cin  >> option;
					switch (option) {
					case 'Y':
					case 'y':
					{
						count += 1;
						cout << "Enter the id for the machine " << count << ": ";
						count -= 1;
						cin >> id;
						bool isAlreadyPresent = false;
						isAlreadyPresent = m->search(id % identifierSpace);
						Bigint factor = 1;
						while (isAlreadyPresent) {
							isAlreadyPresent = m->search((id + factor) % identifierSpace);
							factor++;
						}
						m->insert((id + (factor - 1)) % identifierSpace);
						break;
					}
					case 'N':
					case 'n': {
						bool isAlreadyPresent = false;
						Bigint count2 = 0;
						isAlreadyPresent = m->search(count2 % identifierSpace);
						while (isAlreadyPresent) {
							count2++;
							isAlreadyPresent = m->search(count2 % identifierSpace);
						}
						m->insert(count2 % identifierSpace);
						break;
					}
					}

					break;
				}
			}
			count++;
		}

		m->makeRoutingTables(identifierBit, identifierSpace);
	}

	void printRoutingTable() {
		m->printRoutingTable();
	}

	void printMachines() {
		m->print();
	}

	void deleteMachine() {
			
		if (m->getHead() == NULL) {
			cout << RED << "\nNo Machines Present!" << endl << RESET;
			return;
		}
			cout << "The following machines are available: " << endl;
			printMachines();
			Bigint inputId;
			char option = 'N';
			MachineNode* node = NULL;
			do {
				cout << "Enter Machine Id: ";
				cin >> inputId;
			} while (!(node = m->searchNode(inputId)));

			if (node->key == node->next->key) {
				cout << RED << "This is the last node in the Circular Linked List. If you delete it ALL DATA will be lost! \n"
					<< "Do you really want to delete it?(Y/N)" << RESET;
				cin >> option;
				if (option == 'Y' || option == 'y') {
					if (fs::exists(node->B_Tree.path))
					fs::remove_all(node->B_Tree.path);
					m->deleteRoutingTables();
					m->deleteData(inputId);
				}

			}

			else {
				splitKeysDelete(node);
				if (fs::exists(node->B_Tree.path))
					fs::remove_all(node->B_Tree.path);
				m->deleteRoutingTables();
				m->deleteData(inputId);
				m->makeRoutingTables(identifierBit, identifierSpace);
				numMachines--;

				printMachines();
			}
			
		return;
	}

	void addMachine() {
		if (numMachines >= identifierSpace) {
			cout << "Maximum no. of machines Reached!" << endl;
			cout << "Delete a machine to accomodate another" << endl;
			return;
		}
		char option = 'N';
		string machineName;
		Bigint id;
		MachineNode* insertedNode = NULL;
		cout << "Do you wish to specify the machine name ? (Y / N)";
		cin >> option;
		switch (option) {
		case 'Y':
		case 'y':  // Taking user input for machine ids
		{
			cout << "Enter the name of the machine: ";
			cin >> machineName;
			bool isAlreadyPresent = false;
			isAlreadyPresent = m->search(generateUniqueId(sha1(machineName), identifierSpace));
			Bigint factor = 1;
			while (isAlreadyPresent) {
				isAlreadyPresent = m->search((generateUniqueId(sha1(machineName), identifierSpace) + factor) % identifierSpace);
				factor++;
			}
			m->insert((generateUniqueId(sha1(machineName), identifierSpace) + (factor - 1)) % identifierSpace);
			insertedNode = m->searchNode((generateUniqueId(sha1(machineName), identifierSpace) + (factor - 1)) % identifierSpace);
			break;
		}

		case 'N':
		case 'n':  // Automatically Assigning ids to machine
		{
			cout << "Do you wish to assign id to machine? (Y / N)";
			cin >> option;
			switch (option) {
			case 'Y':
			case 'y':
			{
				cout << "Enter the id for the machine: ";
				cin >> id;
				bool isAlreadyPresent = false;
				isAlreadyPresent = m->search(id % identifierSpace);
				Bigint factor = 1;
				while (isAlreadyPresent) {
					isAlreadyPresent = m->search((id + factor) % identifierSpace);
					factor++;
				}
				m->insert((id + (factor - 1)) % identifierSpace);
				insertedNode = m->searchNode((id + (factor - 1)) % identifierSpace);
				break;
			}
			case 'N':
			case 'n': {
				bool isAlreadyPresent = false;
				Bigint count2 = 0;
				isAlreadyPresent = m->search(count2 % identifierSpace);
				while (isAlreadyPresent) {
					count2++;
					isAlreadyPresent = m->search(count2 % identifierSpace);
				}
				m->insert(count2 % identifierSpace);
				insertedNode = m->searchNode(count2 % identifierSpace);
				break;
			}
			}

			break;
		}
		}

		insertedNode->B_Tree.path = filePath;
		string a = insertedNode->key.a;
		reverse(a.begin(), a.end());
		insertedNode->B_Tree.path += "\\Machine-" + a;

		m->deleteRoutingTables();
		m->makeRoutingTables(identifierBit,identifierSpace);
		numMachines++;
		splitKeysInsert(insertedNode);
		printMachines();
	}

	void splitKeysInsert(MachineNode* node) {
		MachineNode* successor = node->getNext();
		bool isHead = false;
		if (node == m->getHead()) {
			isHead = true;
		}
		Bigint toBeAdded = successor->B_Tree.splitKeysHelperInsert(node->key,isHead,m->tail->key);
		while (toBeAdded != -1) {
			node->B_Tree.insert(toBeAdded);
			string a = toBeAdded.a;
			reverse(a.begin(), a.end());
			string source = successor->B_Tree.path + "\\" + a + ".txt";
			string destination = node->B_Tree.path + "\\" + a + ".txt";
			
			if (fs::exists(source)) {
				ofstream ifileStream(destination);
				fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
				fs::remove(source);
			}

			successor->B_Tree.deletion(toBeAdded);
			toBeAdded = successor->B_Tree.splitKeysHelperInsert(node->key, isHead, m->tail->key);

		}
	}

	void splitKeysDelete(MachineNode* node) {
		MachineNode* successor = node->getNext();
		Bigint toBeAdded = node->B_Tree.splitKeysHelperDelete();
		while (toBeAdded != -1) {
			successor->B_Tree.insert(toBeAdded);
			string a = toBeAdded.a;
			reverse(a.begin(), a.end());
			string source = node->B_Tree.path + "\\" + a + ".txt";
			string destination = successor->B_Tree.path + "\\" + a + ".txt";

			if (fs::exists(source)) {
				ofstream ifileStream(destination);
				fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
				fs::remove(source);
			}

			node->B_Tree.deletion(toBeAdded);
			toBeAdded = node->B_Tree.splitKeysHelperDelete();

		}
	}

	void printRoutingTableofSpecificMachine() {
		cout << "The following are the machines in the network: " << endl;
		printMachines();
		Bigint id;
		MachineNode* current = NULL;
		if (m->getHead() == NULL) {
			return;
		}
		do {
			cout << "Enter the id of the machine whose routing table you want to see: ";
			cin >> id;
			current = m->searchNode(id);
		} while (!current);

		cout << "The Routing Table for " << id << "is: " << endl;
		current->routingTable->print();
		cout << endl;
	}

	void addFile() {
		if (m->getHead() == NULL) {
			cout << RED << "\nNo Machines Present!" << endl << RESET;
			return;
		}
		string filePath;
		do {
			cout << "Enter the file Path: ";
			cin.ignore();
			getline(cin,filePath);

		} while (!fs::exists(filePath));

		ifstream fileStream(filePath);

		string fileContent ="";
		// Check if the file is successfully opened
		if (fileStream.is_open()) {
			// Read and print the contents line by line
	
			string line;
			while (getline(fileStream, line)) {
				fileContent += line;
			}

			// Close the file stream
			fileStream.close();
		}
		else {
			std::cerr << "Error opening file: " << filePath << std::endl;
		}


		Bigint fileHash = generateUniqueId(sha1(filePath), identifierSpace);
		printMachines();
		MachineNode* node = NULL;
		Bigint id;
		do {
			cout << "Specify the machine id from where you want to begin search query for insertion: ";
			cin >> id;
		} while (!(node = m->searchNode(id)));
		cout << endl << "The file Hash is " << fileHash << "." << endl;
		cout << "Path Taken: ";
		bool found = false;
		if (fileHash < m->getHead()->key || fileHash > m->tail->key) {
			node = m->getHead();
			cout << GREEN << node->key << RESET << endl;
		}

		else {
			while (!found)
			{
				cout << GREEN << node->key << RESET << " -> ";

				if (node->key == fileHash) {
					found = true;

				}
				else if (fileHash > node->key && fileHash < node->next->key) {
					node = node->next;
					cout << GREEN << node->key << RESET << " -> ";
					found = true;
				}


				else if (node->key < fileHash && fileHash <= node->routingTable->getHead()->data->key) {
					node = node->routingTable->getHead()->data;
				}


				else {

					DoublyNode<MachineNode>* temp = node->routingTable->getHead();
					DoublyNode<MachineNode>* temp2 = node->routingTable->getHead()->getNext();
					DoublyNode<MachineNode>* tempNode = NULL;

					while (temp2 != NULL) {
						if (fileHash > temp->getData()->key && fileHash <= temp2->getData()->key) {
							tempNode = temp;
							break;
						}

						else {
							temp = temp2;
							temp2 = temp2->getNext();
						}
					}

					if (!tempNode) {


						if (temp->data->key <= fileHash && temp->data->routingTable->getHead()->data->key <= fileHash) {
							node = temp->data;
						}

						else {
							node = node->next;
						}
					}

					else {
						node = tempNode->data;
					}

				}
			}
			cout << "\b\b\b     ";
			cout << RESET << endl;
		}

		node->B_Tree.insert(fileHash);
		string temp = node->B_Tree.path;
		string a = fileHash.a;
		reverse(a.begin(), a.end());
		temp += "\\" + a + ".txt";
		ofstream ifileStream(temp);
		fs::copy_file(filePath, temp, fs::copy_options::overwrite_existing);
		
		return;
	}

	void removeFile() {
		if (m->getHead() == NULL) {
			cout << RED << "\nNo Machines Present!" << endl << RESET;
			return;
		}
		Bigint fileHash;
		do {
			cout << "Enter file key which you want to remove: ";
			cin >> fileHash;
		} while (fileHash >= identifierSpace);

		printMachines();
		MachineNode* node = NULL;
		Bigint id;
		do {
			cout << "Specify the machine id from where you want to begin search query for deletion: ";
			cin >> id;
		} while (!(node = m->searchNode(id)));
		bool found = false;
		cout << "Path Taken: ";
		if (fileHash < m->getHead()->key || fileHash > m->tail->key) {
			node = m->getHead();
			cout << GREEN << node->key << RESET << endl;
		}

		else {
			while (!found)
			{
				cout << GREEN << node->key << RESET << " -> ";
				if (fileHash > node->key && fileHash < node->next->key) {
					node = node->next;
					cout << GREEN << node->key << RESET << " -> ";
					found = true;
				}

				else if (node->key == fileHash) {
					found = true;

				}

				else if (node->key < fileHash && fileHash <= node->routingTable->getHead()->data->key) {
					node = node->routingTable->getHead()->data;
				}

				else {

					DoublyNode<MachineNode>* temp = node->routingTable->getHead();
					DoublyNode<MachineNode>* temp2 = node->routingTable->getHead()->getNext();
					DoublyNode<MachineNode>* tempNode = NULL;

					while (temp2 != NULL) {
						if (fileHash > temp->getData()->key && fileHash <= temp2->getData()->key) {
							tempNode = temp;
							break;
						}

						else {
							temp = temp2;
							temp2 = temp2->getNext();
						}
					}

					if (!tempNode) {

						if (temp->data->key <= fileHash && temp->data->routingTable->getHead()->data->key <= fileHash) {
							node = temp->data;
						}

						else {
							node = node->next;
						}
					}

					else {
						node = tempNode->data;
					}

				}


				cout << "\b\b\b     ";
				cout << RESET << endl;
			}
		}

		if (node->B_Tree.getRoot()->searchNode(fileHash) != NULL) {

			string path = node->B_Tree.path;
			string a = fileHash.a;
			reverse(a.begin(), a.end());
			path += "\\" + a + ".txt";
			if (fs::exists(path))
				fs::remove(path);
			node->B_Tree.deletion(fileHash);
		}

		else {
			cout << "File doesnot exist!" << endl;
		}
	
		

		return;
	}

	void printBTree() {
		if (m->getHead() == NULL) {
			cout << RED << "\nNo Machines Present!" << endl << RESET;
			return;
		}
		cout << "The following are the machines in the network: " << endl;
		printMachines();
		Bigint id;
		MachineNode* current = NULL;
		do {
			cout << "Enter the id of the machine whose B-Tree you want to see: ";
			cin >> id;
			current = m->searchNode(id);
		} while (!current);

		cout << "The B-Tree is: " << endl;
		current->B_Tree.traverse(current->B_Tree.getRoot());
	}

	void drawIPFS() {
		string IPFSHeading = "InterPlanetary File System (IPFS)";
		cout << setw(50) << REVERSED << BOLD  <<  IPFSHeading << RESET << endl << endl;
	}

	void menu() {
		drawIPFS();
		userInput();
		int option;
		while (1) {
			cout << "----------------------------------------" << endl;
			cout << "1. Print Machines" << endl;
			cout << "2. Print All Routing Tables" << endl;
			cout << "3. Print Routing Table of Specific Machine" << endl;
			cout << "4. Add File" << endl;
			cout << "5. Remove File" << endl;
			cout << "6. Add Machine" << endl;
			cout << "7. Remove Machine" << endl;
			cout << "8. Print B-Tree" << endl;
			cout << "9. Exit Program" << endl << endl;
			cout << YELLOW << "Enter option: " << RESET;
			while (!(cin >> option)) {
				cout << RED <<"Invalid input." <<RESET <<endl<< YELLOW<<"Please enter option again: " << RESET;
				cin.clear();  // Clear the error flag
				cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
			}
			cout << "----------------------------------------" << endl;
			switch (option) {
			case 1: {
				printMachines();
				break;
			}
			case 2: {
				printRoutingTable();
				break;
			}
			case 3: {
				printRoutingTableofSpecificMachine();
				break;
			}
			case 4: {
				addFile();
				break;
			}
			case 5: {
				removeFile();
				break;
			}
			case 6: {
				addMachine();
				break;
			}
			case 7: {
				deleteMachine();
				break;
			}
			case 8: {
				printBTree();
				break;
			}

			case 9: {
				return;
			}
			default:
				cout << "Invalid Option Entered!" << endl;

			}
		}
	}

	~ringDHT() {
		fs::remove_all(filePath);
	}
};

int main() {
	ringDHT r;
	r.menu();

	
	return 0;
}


