#pragma once
#include"doublyLinkedList.h"
#include"BTree.h"
using namespace std;
#include<filesystem>
#include <fstream>
namespace fs = std::filesystem;
class MachineNode
{
public:
	Bigint key; // key for machine
	BTree B_Tree;// B-tree of file keys storing the file as key and key will be generated through H(file Content)->key
	DoublyLinkedList<MachineNode>* routingTable;
	MachineNode* next;
	string path;
	MachineNode()
	{

		key = 0;
		next = NULL;
		routingTable = new DoublyLinkedList<MachineNode>;
		path = "C:\\Users\\abdul\\Desktop\\IPFS";
		B_Tree.path = path;
	}
	MachineNode(Bigint d, MachineNode* p)
	{
		path = "C:\\Users\\abdul\\Desktop\\IPFS";
		B_Tree.path = path;
		key = d;
		string k = key.a;
		reverse(k.begin(), k.end());
		B_Tree.path += "\\Machine-" + k;
		fs::create_directory(B_Tree.path);
		next = p;
		routingTable = new DoublyLinkedList<MachineNode>;

	}
	Bigint getData()
	{
		return this->key;
	}
	MachineNode* getNext()
	{
		return this->next;
	}
	void setData(Bigint d)
	{
		key = d;
	}
	void setNext(MachineNode* node)
	{
		next = node;
	}

};


