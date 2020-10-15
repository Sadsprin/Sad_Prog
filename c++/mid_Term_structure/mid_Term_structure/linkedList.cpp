#include <iostream>
using namespace std;
class LinkedList;
class ChainNode {
	friend LinkedList;
private:
	int data;
	ChainNode* link;
public:
	ChainNode(int element = 0, ChainNode* Next = 0) {
		data = element; link = Next;
	}

};

class LinkedList {
private:
	ChainNode* first;
	ChainNode* last;
	int size;

public:
	LinkedList() {
		first = NULL;
		last = NULL;
		size = 0;
	};
	~LinkedList() { };
	void AddNode(int value);
	void insertNode(int value, int n);
	void print();
	void DeleteNode(int n);

};

void LinkedList::insertNode(int value, int n) {
	ChainNode* tmp = new ChainNode;
	ChainNode* Node2 = new ChainNode;
	tmp = first;

	if (size == n) {
		last->link = Node2;
		Node2->data = value;
		last = last->link;
	}

	
	if ((size >= 1) && (size != n) && (n != 0) ) {
		for (int i = 1; i < n; i++) {
			tmp = tmp->link;
		}
		Node2->link = tmp->link;
  	    tmp->link = Node2;
		Node2->data = value;
	}
	size++;
}

void LinkedList::DeleteNode(int n) {

	ChainNode* search = new ChainNode;
	ChainNode* tmp = new ChainNode;
	search = first;
	if ((size >= 1) && (size != n) && (n != 0)) {
		for (int i = 1; i < n; i++) {
			search = search->link;
		}
		tmp = search;
		search = search->link;
		tmp->link = search->link;
		delete search;
	}
	size--;
}

void LinkedList::AddNode(int value) {
	ChainNode* tmp = new ChainNode;
	tmp->data = value;
	tmp->link = NULL;

	if (first == NULL) {
		first = tmp;
		last = tmp;
	}
	else {
		last->link = tmp;
		last = last->link;
	}
	size++;
}

void LinkedList::print() {
	ChainNode* tmp = new ChainNode;
	tmp = first;
	while (tmp != NULL) {
		cout << tmp->data<<" ";
		tmp = tmp->link;
	}
}
int main() {
	LinkedList A;

	A.AddNode(1);
	A.AddNode(2);
	A.AddNode(4);
	A.AddNode(5);
	A.AddNode(6);
	A.AddNode(7);
	A.AddNode(8);
	A.AddNode(9);
	A.AddNode(23);
	A.DeleteNode(4);





	A.print();

	return 0;

}


