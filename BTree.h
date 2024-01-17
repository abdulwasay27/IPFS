#pragma once
#include "Bigint.h"
#include <iostream>
#include <string>
using namespace std;
const int ORDER = 6;

class BTreeNode {
public:
	Bigint* keys;
	BTreeNode** children;
	int num_keys;
	bool isLeaf;


	BTreeNode(bool leaf) {
		isLeaf = leaf;
		num_keys = 0;
		keys = new Bigint[ORDER - 1];
		children = new BTreeNode * [ORDER];
		for (int i = 0; i < ORDER; i++)
			children[i] = nullptr;
	}

	void insertNode(Bigint key) {
		int idx = num_keys - 1;

		if (isLeaf) {
			while (idx >= 0 && key < keys[idx]) {
				keys[idx + 1] = keys[idx];
				idx--;
			}
			keys[idx + 1] = key;
			num_keys++;
		}
		else {
			while (idx >= 0 && key < keys[idx])
				idx--;

			if (children[idx + 1]->num_keys == ORDER - 1) {
				splitNodes(children[idx + 1], idx + 1);

				if (key > keys[idx + 1])
					idx++;
			}
			children[idx + 1]->insertNode(key);
		}
	}

	void splitNodes(BTreeNode* toSplit, int i) {
		BTreeNode* x = new BTreeNode(toSplit->isLeaf);
		x->num_keys = ORDER / 2 - 1;

		int j = 0;
		while (j < ORDER / 2 - 1) {
			x->keys[j] = toSplit->keys[j + ORDER / 2];
			j++;
		}

		if (!toSplit->isLeaf) {
			j = 0;
			while (j < ORDER / 2) {
				x->children[j] = toSplit->children[j + ORDER / 2];
				j++;
			}
		}

		toSplit->num_keys = ORDER / 2 - 1;
		j = num_keys;

		while (j >= i + 1) {
			children[j + 1] = children[j];
			j--;
		}
		children[i + 1] = x;

		j = num_keys - 1;
		while (j >= i) {
			keys[j + 1] = keys[j];
			j--;
		}

		keys[i] = toSplit->keys[ORDER / 2 - 1];
		num_keys++;
	}

	BTreeNode* searchNode(Bigint key)
	{
		int idx = 0;
		while (idx < num_keys && keys[idx] < key) {
			idx++;
		}

		if (keys[idx] == key) {
			return this;
		}

		if (this->isLeaf == true) {
			return nullptr;
		}
		BTreeNode* node = children[idx]->searchNode(key);
		return node;
	}


	void traverse() {
		int i;
		for (i = 0; i < num_keys; i++) {
			if (!isLeaf)
				children[i]->traverse();
			cout << " " << keys[i];
		}

		if (!isLeaf)
			children[i]->traverse();
	}


	// Remove from the leaf
	void removeFromLeaf(int idx)
	{
		for (int i = idx + 1; i < num_keys; ++i)
			keys[i - 1] = keys[i];

		num_keys--;

		return;
	}

	Bigint getPredecessor(int idx)
	{
		BTreeNode* cur = children[idx];
		while (cur->isLeaf == false)
			cur = cur->children[cur->num_keys];

		return cur->keys[cur->num_keys - 1];
	}

	Bigint getSuccessor(int idx) {
		BTreeNode* cur = children[idx + 1];
		while (cur->isLeaf == false)
			cur = cur->children[0];

		return cur->keys[0];
	}

	void fill(int idx)
	{
		if (idx != 0 && ORDER / 2 <= children[idx - 1]->num_keys)
			borrowFromPrev(idx);

		else if (idx != num_keys && ORDER / 2 <= children[idx + 1]->num_keys)
			borrowFromNext(idx);

		else {
			if (idx != num_keys)
				merge(idx);
			else
				merge(idx - 1);
		}
		return;
	}

	// Borrow from previous
	void borrowFromPrev(int idx)
	{
		BTreeNode* child = children[idx];
		BTreeNode* sibling = children[idx - 1];

		int i = child->num_keys - 1;
		while (i >= 0)
		{
			child->keys[i + 1] = child->keys[i];
			i--;
		}

		if (child->isLeaf == false) {
			int i = child->num_keys;
			while (i >= 0)
			{
				child->children[i + 1] = child->children[i];
				i--;
			}

		}

		child->keys[0] = keys[idx - 1];

		if (child->isLeaf == false)
			child->children[0] = sibling->children[sibling->num_keys];

		keys[idx - 1] = sibling->keys[sibling->num_keys - 1];

		child->num_keys = child->num_keys + 1;
		sibling->num_keys = sibling->num_keys - 1;

		return;
	}

	// Borrow from the next
	void borrowFromNext(int idx)
	{
		BTreeNode* child = children[idx];
		BTreeNode* sibling = children[idx + 1];
		int mid = child->num_keys;
		child->keys[mid] = keys[idx];

		if (child->isLeaf == false)
			child->children[mid + 1] = sibling->children[0];

		keys[idx] = sibling->keys[0];
		int i = 1;
		while (i < sibling->num_keys)
		{
			sibling->keys[i - 1] = sibling->keys[i];
			i++;
		}


		if (sibling->isLeaf == false) {
			int i = 1;
			while (i <= sibling->num_keys)
			{
				sibling->children[i - 1] = sibling->children[i];
				i++;
			}
		}

		child->num_keys = child->num_keys + 1;
		sibling->num_keys = sibling->num_keys - 1;

		return;
	}

	// Merge
	void merge(int idx) {
		BTreeNode* child = children[idx];
		BTreeNode* sibling = children[idx + 1];

		child->keys[ORDER / 2 - 1] = keys[idx];

		int i = 0;
			while (i < sibling->num_keys)
			{
				child->keys[i + ORDER / 2] = sibling->keys[i];
				i++;
			}


		if (child->isLeaf == false) {
			int i = 0;
			while (i <= sibling->num_keys)
			{
				child->children[i + ORDER / 2] = sibling->children[i];
				i++;
			}

		}
		 i = idx + 1;
			while (i < num_keys)
			{
				keys[i - 1] = keys[i];
				i++;
			}

		i = idx + 2;
		while (i <= num_keys)
		{
			children[i - 1] = children[i];
			i++;
		}


		child->num_keys = child->num_keys + sibling->num_keys + 1;
		num_keys = num_keys - 1;

		delete (sibling);
		return;
	}

	// Delete from non leaf node
	void removeFromNonLeaf(int idx)
	{
		Bigint k = keys[idx];

		if (children[idx]->num_keys >= ORDER / 2) {
			Bigint pred = getPredecessor(idx);
			keys[idx] = pred;
			children[idx]->deletion(pred);
		}

		else if (children[idx + 1]->num_keys >= ORDER / 2) {
			Bigint succ = getSuccessor(idx);
			keys[idx] = succ;
			children[idx + 1]->deletion(succ);
		}

		else {
			merge(idx);
			children[idx]->deletion(k);
		}
		return;
	}

	// Find the key
	int findKey(Bigint k)
	{
		int idx = 0;
		while (idx < num_keys && keys[idx] < k)
			++idx;
		return idx;
	}

	// Deletion operation
	void deletion(Bigint k) {
		int idx = findKey(k);

		if (num_keys > idx && k == keys[idx]) {
			if (isLeaf == true)
				removeFromLeaf(idx);
			else
				removeFromNonLeaf(idx);
		}
		else {
			if (isLeaf == true) {
				cout << "The key " << k << " does not exist in the tree\n";
				return;
			}

			bool flag = false;
			if (idx == num_keys)
				flag = true;

			if (children[idx]->num_keys < ORDER / 2)
				fill(idx);

			if (flag && num_keys < idx)
				children[idx - 1]->deletion(k);
			else
				children[idx]->deletion(k);
		}
		return;
	}

	friend class BTree;
};

class BTree {
	BTreeNode* root;
public:
	string path;

	BTree() : root(nullptr) {}

	BTreeNode* getRoot()
	{
		return root;
	}
	void traverse(BTreeNode* node)
	{
		if (node != nullptr)
		{
			cout << endl;
			int i;
			for (i = 0; i < node->num_keys; i++) {
				if (node->isLeaf == false) {
					traverse(node->children[i]);
				}
				cout << " " << node->keys[i];
			}
			if (node->isLeaf == false) {
				traverse(node->children[i]);
			}
			cout << endl;
		}
		else
		{
			cout << "The B-Tree is Empty!" << endl;
		}
	}
	Bigint splitKeysHelperInsert(Bigint key,bool isHead,Bigint k) {
		return splitKeysHelperInsert(root, key,isHead,k);
	}

	Bigint splitKeysHelperInsert(BTreeNode* node, Bigint key, bool isHead, Bigint k) {
		if (node != nullptr)
		{
			int i;
			for (i = 0; i < node->num_keys; i++) {
				if (node->isLeaf == false) {
					splitKeysHelperInsert(node->children[i], key,isHead,k);
				}
				if (isHead) {
					if (node->keys[i] > k) {
						return node->keys[i];
					}
				}
				if (node->keys[i] <= key) {
					return node->keys[i];
				}
			}
			if (node->isLeaf == false) {
				splitKeysHelperInsert(node->children[i], key,isHead,k);
			}
		}

		return -1;

	}

	Bigint splitKeysHelperDelete() {
		return splitKeysHelperDelete(root);
	}

	Bigint splitKeysHelperDelete(BTreeNode* node) {
		if (node != nullptr)
		{
			int i;
			for (i = 0; i < node->num_keys; i++) {
				if (node->isLeaf == false) {
					splitKeysHelperDelete(node->children[i]);
				}
				return node->keys[i];

			}
			if (node->isLeaf == false) {
				splitKeysHelperDelete(node->children[i]);
			}
		}

		return -1;

	}

	void insert(Bigint key)
	{
		if (root == nullptr) {
			root = new BTreeNode(true);
			root->keys[0] = key;
			root->num_keys++;
		}
		else {
			if (root->num_keys != ORDER - 1) {
				root->insertNode(key);
			}
			else {
				BTreeNode* new_root = new BTreeNode(false);
				new_root->children[0] = root;
				new_root->splitNodes(root, 0);

				int idx = 0;
				if (key > new_root->keys[0])
					idx++;

				new_root->children[idx]->insertNode(key);
				root = new_root;
			}
		}
	}

	// Delete Operation
	void deletion(Bigint k)
	{
		if (root == nullptr) {
			cout << "The tree is empty.\n";
			return;
		}

		root->deletion(k);

		if (root->num_keys == 0) {
			BTreeNode* tmp = root;
			if (root->isLeaf == true)
				root = NULL;
			else
				root = root->children[0];

			delete tmp;
		}
		return;
	}
};

//int main() {
//	cout << "Enter Order: ";
//	cin >> ORDER;
//	if (ORDER % 2 != 0) {
//		ORDER++;
//	}
//	BTree tree;
//	tree.insert(8);
//	tree.insert(9);
//	tree.insert(10);
//	tree.insert(11);
//	tree.insert(15);
//	tree.insert(16);
//	tree.insert(17);
//	tree.insert(18);
//	tree.insert(20);
//	tree.insert(23);
//	tree.insert(12);
//	tree.insert(14);
//	tree.insert(57);
//	tree.insert(8);
//	tree.insert(29);
//	tree.insert(100);
//	tree.insert(171);
//
//	cout << "The B-tree is: ";
//	tree.traverse(tree.getRoot());
//
//	/*BTreeNode* node = tree.getRoot()->searchNode(154);
//	if (node != nullptr)
//		cout << endl << "Found" << endl;
//	else
//		cout << "Not found" << endl;*/
//	tree.deletion(8);
//	tree.deletion(9);
//	tree.deletion(10);
//	tree.deletion(11);
//	tree.deletion(15);
//	tree.deletion(16);
//	tree.deletion(17);
//	tree.deletion(18);
//	tree.deletion(20);
//	tree.deletion(23);
//	tree.deletion(12);
//	tree.deletion(14);
//	tree.deletion(57);
//	tree.deletion(8);
//	tree.deletion(29);
//	tree.deletion(100);
//	tree.deletion(171);
//
//	cout << "The B-tree is: ";
//	tree.traverse(tree.getRoot());
//}