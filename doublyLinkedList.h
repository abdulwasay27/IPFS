#pragma once
#include <iostream>
using namespace std;
template <typename T>
class DoublyNode {
public:
	T* data;
	DoublyNode* prev;
	DoublyNode* next;

	DoublyNode() {
		data = NULL;
		prev = NULL;
		next = NULL;
	}

	DoublyNode(T* d, DoublyNode* previous, DoublyNode* next) {
		this->data = d;
		this->prev = previous;
		this->next = next;
	}

	T* getData()  {
		return data;
	}

	DoublyNode* getNext()  {
		return next;
	}

	DoublyNode* getPrev()  {
		return prev;
	}

	void setData(T* d) {
		data = d;
	}

	void setNext(DoublyNode* x) {
		next = x;
	}

	void setPrev(DoublyNode* x) {
		prev = x;
	}

};

template <typename T>
class DoublyLinkedList {
	DoublyNode<T>* Head;
public:
	int size = 0;
	DoublyLinkedList() {
		Head = NULL;
	}

	bool isEmpty() {
		return (Head == NULL);
	}

	void print() {
		if (!isEmpty()) {
			DoublyNode<T>* temp = Head;
			Bigint data_num = 1;
			while (temp != NULL) {
				cout << GREEN << "\nIndex " << data_num << ": " << temp->getData()->key << RESET;
				data_num++;
				temp = temp->getNext();
			}
		}
	}

	DoublyNode<T>* getHead() {
		return Head;
	}

	void setHead(DoublyNode<T>* x) {
		Head = x ;
	}

	void insertToHead(T* d) {
		if (Head == NULL) {
			Head = new DoublyNode<T>(d, NULL, NULL);
		}

		else {
			DoublyNode<T>* new_node = new DoublyNode<T>(d, NULL, NULL);
			new_node->setNext(Head);
			Head->setPrev(new_node);
			Head = new_node;
		}
	}

	void insert(T* d) {
		if (!isEmpty()) {
			DoublyNode<T>* temp = Head;
			DoublyNode<T>* previous = NULL;
			while (temp != NULL) {
				previous = temp;
				temp = temp->getNext();
			}

			temp = new DoublyNode<T>(d, previous, NULL);
			previous->setNext(temp);

		}

		else {
			insertToHead(d);
		}


	}

	bool search(int d) {
		if (!isEmpty()) {
			DoublyNode<T>* temp = Head;

			while (temp != NULL) {
				if (temp->getData()->key == d) {
					return true;
				}

				temp = temp->getNext();
			}
		}

		return false;
	}

	T* searchMachineNode(int d) {
		if (!isEmpty()) {
			DoublyNode<T>* temp = Head;

			while (temp != NULL) {
				if (temp->getData()->key == d) {
					return temp->getData();
				}

				temp = temp->getNext();
			}
		}

		return NULL;
	}

	DoublyNode<T>* search_node(T* d) {
		if (!isEmpty()) {
			DoublyNode<T>* temp = Head;

			while (temp != NULL) {
				if (temp->getData() == d) {
					return temp;
				}

				temp = temp->getNext();
			}
		}

		return NULL;
	}

	void insertAtIndex(T* d, Bigint index) {
		DoublyNode<T>* temp = Head;
		while (index != 0) {
			temp = temp->getNext();
			index--;
		}

		DoublyNode<T>* new_node = new DoublyNode<T>(d, NULL, NULL);
		new_node->setPrev(temp->getPrev());
		new_node->setNext(temp);
		temp->getPrev()->setNext(new_node);
		temp->setPrev(new_node);

	}

	void deleteData(T* d) {
		DoublyNode<T>* del_node = search_node(d);
		DoublyNode<T>* previous = del_node->getPrev();
		DoublyNode<T>* next = del_node->getNext();
		previous->setNext(next);
		next->setPrev(previous);

		del_node->setNext(NULL);
		del_node->setPrev(NULL);
		delete del_node;
		del_node = NULL;
	}

	~DoublyLinkedList() {
		if (Head != NULL) {
			Head->setNext(NULL);
			delete Head;
			Head = NULL;
		}
	}
};
