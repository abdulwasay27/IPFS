#pragma once
#include <iostream>

#include "machineNode.h"


using namespace std;
class CircularLinkedList
{
private:
	MachineNode* head;
public:
	MachineNode* tail;
	CircularLinkedList()
	{
		head = NULL;
		tail = NULL;
	}
	void insert(Bigint data)
	{
		MachineNode* newNode = new MachineNode(data, NULL);

		if (head == NULL)
		{
			// If the list is empty, insert the new node and make it circular
			newNode->setNext(newNode);
			head = newNode;
		}
		else if (data <= head->getData())
		{
			// If the data is less than or equal to the head's data, insert at the beginning
			MachineNode* current = head;
			while (current->getNext() != head)
				current = current->getNext();

			current->setNext(newNode);
			newNode->setNext(head);
			head = newNode;
		}
		else
		{
			// Insert the new node at the appropriate position in the sorted order
			MachineNode* current = head;
			while (current->getNext() != head && current->getNext()->getData() < newNode->getData())
			{
				current = current->getNext();
			}

			newNode->setNext(current->getNext());
			current->setNext(newNode);
		}

		MachineNode* temp = head;
		while (temp->next != head) {
			temp = temp->getNext();
		}

		tail = temp;
	}

	void makeRoutingTables(int identifierBit, Bigint identifierSpace) {
		MachineNode* current = head;
		do {
			Bigint p = current->key;
			Bigint successor;
			MachineNode* successorNode = NULL;
			for (int i = 1; i <= identifierBit; i++) {
				successor = (p + pow(2, i - 1));
				successor = successor % identifierSpace;
				while (successorNode == NULL) {
					successorNode = searchNode(successor);
					successor++;
					successor = successor % identifierSpace;
				}

				current->routingTable->insert(successorNode);
				successorNode = NULL;
			}
			current = current->getNext();
		} while (current != head);
	}

	void deleteRoutingTables() {
		MachineNode* current = head;
		do {
			current->routingTable->setHead(NULL);
			current = current->getNext();
		} while (current != head);
	}

	void printRoutingTable() {
		Bigint i = 1;
		MachineNode* temp = head;
		if (temp == NULL) {
			cout << RED << "\nNo Machines Present!" << endl << RESET;

			return;
		}

		do
		{
			cout << "\nRouting Table for";
			string k = temp->key.a;
			reverse(k.begin(), k.end());
			cout << CYAN << " < MachineNode " <<RESET  << i << CYAN << " with id " <<RESET<< k << CYAN << " > " << RESET << endl;
			i++;
			temp->routingTable->print();
			cout << endl;
			temp = temp->getNext();
		} while (temp && temp != head);
	}
	bool search(Bigint d)
	{
		MachineNode* temp = head;
		do
		{
			if (temp == NULL)
			{
				break;
			}
			if (temp->getData() == d)
			{
				return true;
			}
			temp = temp->getNext();
		} while (temp && temp != head);
		return false;
	}

	MachineNode* searchNode(Bigint d)
	{
		MachineNode* temp = head;
		do
		{
			if (temp == NULL)
			{
				break;
			}
			if (temp->getData() == d)
			{
				return temp;
			}
			temp = temp->getNext();
		} while (temp && temp != head);

		return NULL;
	}

	void update(Bigint d, Bigint newnumber)
	{
		MachineNode* temp = head;
		do
		{
			if (temp == NULL)
			{
				break;
			}
			if (temp->getData() == d)
			{
				temp->setData(newnumber);
			}
			temp = temp->getNext();
		} while (temp && temp != head);
	}
	void insertAtIndex(Bigint d, Bigint index)
	{
		if (!isEmpty())
		{
			Bigint i = 0;
			MachineNode* temp = head;
			do
			{
				i++;
				if (index == i)
				{
					break;
				}
				temp = temp->getNext();
			} while (temp && temp != head);

			if (temp->getNext())
			{
				MachineNode* newMachineNode = new MachineNode(d, NULL);
				newMachineNode->setNext(temp->getNext());
				temp->setNext(newMachineNode);
			}
			else
			{
				MachineNode* newMachineNode = new MachineNode(d, NULL);
				temp->setNext(newMachineNode);
				newMachineNode->setNext(head);
			}
		}
		else if (index == 0)
		{
			MachineNode* newMachineNode = new MachineNode(d, NULL);
			head = newMachineNode;
			newMachineNode->setNext(head);
		}
	}
	void deleteData(Bigint d)
	{
		MachineNode* temp = head;
		bool check = false;
		bool check2 = false;
		do
		{
			if (temp == NULL)
			{
				check = true;
				break;
			}
			if (temp->getData() == d)
			{
				check2 = true;
				break;
			}
			if (temp->getNext()->getData() == d)
			{
				break;
			}
			temp = temp->getNext();
		} while (temp->getNext() && temp->getNext() != head);
		if (check)
		{
			return;
		}
		if (check2)
		{
			MachineNode* temp2 = head;
			while (temp2->getNext()->getData() != d)
			{
				temp2 = temp2->getNext();
			}
			if (temp2->getNext() == temp2) {
				head = NULL;
				return;
			}
			temp2->setNext(temp2->getNext()->getNext());
			temp2 = temp2->getNext();
			head = temp2;
			return;
		}
		MachineNode* temp1 = temp->getNext()->getNext();
		temp->getNext()->setNext(NULL);
		temp->setNext(temp1);

		MachineNode* temp3 = head;
		while (temp3->next != head) {
			temp3 = temp3->getNext();
		}

		tail = temp3;
	}
	bool isEmpty()
	{
		if (head == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	MachineNode* getHead()
	{
		return head;
	}
	void print()
	{
		Bigint i = 1;
		MachineNode* temp = head;
		if (temp == NULL) {
			cout << RED<< "\nNo Machines Present!" << endl << RESET;
			return;
		}
		do
		{
			string a = temp->getData().a;
			reverse(a.begin(), a.end());
			cout << CYAN << " <MachineNode " << RESET <<  i << CYAN << " with Id " << RESET <<  a  <<CYAN<< " >"<<RESET << endl;
			i++;
			temp = temp->getNext();
		} while (temp && temp != head);
	}
};
