#include <iostream>
#include <string>
#include <queue>
#include "BSTree.h"

using namespace std;

#define SPACE_WIDTH 5
#define NUMBER_WIDTH 2

BSTree::BSTree()
{
	this->_root = NULL;
}


BSTree::~BSTree()
{
	delete this->_root;
	this->_root = NULL;
}

Node* BSTree::root() {
	return this->_root;
}

void BSTree::add(const int data) {
	Node* root = this->root();
	Node* n = new Node(data);
	if (root == NULL) {		
		this->_setRoot(n);
	} else {
		this->_add(n, root);
	}
}

void BSTree::remove(const int data) {
	Node* location = NULL;
	Node* parent = NULL;
	this->find(data, location, parent);
	this->_remove(location, parent);	
}

void BSTree::find(const int data, Node*& location, Node*& parent)
{
	location = NULL;
	parent = NULL;
	Node* startNode = this->root();
	Node* parentOfStartNode = NULL;
	if (startNode) {
		this->_find(data, startNode, parentOfStartNode, location, parent);
	}
}

void  BSTree::findSmallest(Node*& location, Node*& parent) {
	location = NULL;
	parent = NULL;
	Node* startNode = this->root();
	Node* parentOfStartNode = NULL;
	if (startNode) {
		this->_findSmallest(startNode, parentOfStartNode, location, parent);
	}
}

void  BSTree::findLargest(Node*& location, Node*& parent) {
	location = NULL;
	parent = NULL;
	Node* startNode = this->root();
	Node* parentOfStartNode = NULL;
	if (startNode) {
		this->_findLargest(startNode, parentOfStartNode, location, parent);
	}
}

int BSTree::height() {
	return this->_height(this->root());
}

std::vector<Node*> BSTree::traverse(const TraverseType traverseType) {
	std::vector<Node*> nodesList;
	Node* root = this->root();
	if (root == NULL) {
		return nodesList;
	}
	int type = (int)traverseType;
	switch (type) {
		case (TraverseType::DFSInOrder):
		{
			this->_traverse_DFSInOrder(root, nodesList);
			break;
		}
		case(TraverseType::DFSPreOrder):
		{
			this->_traverse_DFSPreOrder(root, nodesList);
			break;
		}
		case(TraverseType::DFSPostOrder):
		{
			this->_traverse_DFSPostOrder(root, nodesList);
			break;
		}
		case(TraverseType::BFS):
		{
			this->_traverse_BFS(root, nodesList);
			break;
		}
	}
	return nodesList;
}

void BSTree::_setRoot(Node* root) {
	this->_root = root;
}

// private functions
void BSTree::_add(Node*& nodeToBeInserted, Node*& atNode) {
	if ((nodeToBeInserted == NULL) || (atNode == NULL)) {
		return;
	}
	if (atNode->getData() > nodeToBeInserted->getData()) {
		Node* left = atNode->getLeft();
		if (left != NULL) {
			this->_add(nodeToBeInserted, left);
		}
		else {
			atNode->setLeft(nodeToBeInserted);
		}
	} else {
		Node* right = atNode->getRight();
		if (right != NULL) {
			this->_add(nodeToBeInserted, right);
		}
		else {
			atNode->setRight(nodeToBeInserted);
		}
	}
}

void  BSTree::_find(const int data, Node* startNode, Node* parentOfStartNode, Node*& location, Node*& parent) {
	if (startNode == NULL) {
		return;
	}
	int _data = startNode->getData();
	if (_data == data) {
		location = startNode;
		parent = parentOfStartNode;
	} else if (_data > data) {
		Node* left = startNode->getLeft();
		if (left) {
			this->_find(data, left, startNode, location, parent);
		}
	} else {
		Node* right = startNode->getRight();
		if (right) {
			this->_find(data, right, startNode, location, parent);
		}
	}
}

void BSTree::_remove(Node*& location, Node*& parent) {
	if (location == NULL) {
		return;
	}	
	Node* left = location->getLeft();
	Node* right = location->getRight();
	// Case 1:  No child
	if (left == NULL && right == NULL) {
		if (parent->getLeft() == location) {
			parent->setLeft(NULL);
		}
		if (parent->getRight() == location) {
			parent->setRight(NULL);
		}
		delete location;
		location = NULL;
	}
	//Case 2a: One child - left is null
	else if (left == NULL) {
		if (parent->getLeft() == location) {
			parent->setLeft(location->getRight());
		}
		if (parent->getRight() == location) {
			parent->setRight(location->getRight());
		}
		location->setRight(NULL);
		delete location;
		location = NULL;
	}
	//Case 2b: One child - right is null
	else if (right == NULL) {
		if (parent->getLeft() == location) {
			parent->setLeft(location->getLeft());
		}
		if (parent->getRight() == location) {
			parent->setRight(location->getLeft());
		}
		location->setLeft(NULL);
		delete location;
		location = NULL;
	}
	// case 3: Two children
	else {
		Node* smallestInRightTree = NULL;
		Node* parentOfSmallest = NULL;
		// find the smallest in the right side of the tree
		this->_findSmallest(location->getRight(), parent, smallestInRightTree, parentOfSmallest);
		location->setData(smallestInRightTree->getData());
		this->_remove(smallestInRightTree, parentOfSmallest);
	}
}

void  BSTree::_findSmallest(Node* startNode, Node* startParentNode, Node*& location, Node*& parent) {
	if (startNode == NULL) {
		return;
	}
	Node* left = startNode->getLeft();
	if (left == NULL) {
		location = startNode;
		parent = startParentNode;
	} else {
		_findSmallest(left, startNode, location, parent);
	}
}

void  BSTree::_findLargest(Node* startNode, Node* startParentNode, Node*& location, Node*& parent) {
	if (startNode == NULL) {
		return;
	}
	Node* right = startNode->getRight();
	if (right == NULL) {
		location = startNode;
		parent = startParentNode;
	} else {
		_findLargest(right, startNode, location, parent);
	}
}

int BSTree::_height(Node* node) {
	int height = 0;
	if (node != NULL) {
		int l = 0, r = 0;
		Node* lNode = node->getLeft();
		Node* rNode = node->getRight();
		if (lNode != NULL) {
			height = 1;
			l = this->_height(lNode);
		}
		if (rNode != NULL) {
			height = 1;
			r = this->_height(rNode);
		}
		height = height + ((l > r) ? l : r);
	}
	return height;
}

// DFSInOrder: LNR(Left-Node-Right)
void BSTree::_traverse_DFSInOrder(Node* node, std::vector<Node*>& nodesList) {	
	if (node == NULL) {
		return;
	}
	Node* left = node->getLeft();
	Node* right = node->getRight();
	if (left) {
		this->_traverse_DFSInOrder(left, nodesList);
	}
	nodesList.push_back(node);
	if(right){
		this->_traverse_DFSInOrder(right, nodesList);
	}
}

// DFSPreOrder: NLR(Node-Left-Right)
void BSTree::_traverse_DFSPreOrder(Node* node, std::vector<Node*>& nodesList) {
	if (node == NULL) {
		return;
	}
	Node* left = node->getLeft();
	Node* right = node->getRight();
	nodesList.push_back(node);
	if (left) {
		this->_traverse_DFSPreOrder(left, nodesList);
	}
	if (right) {
		this->_traverse_DFSPreOrder(right, nodesList);
	}
}

// DFSPostOrder: LRN(Left-Right-Node)
void BSTree::_traverse_DFSPostOrder(Node* node, std::vector<Node*>& nodesList) {
	if (node == NULL) {
		return;
	}
	Node* left = node->getLeft();
	Node* right = node->getRight();
	if (left) {
		this->_traverse_DFSPostOrder(left, nodesList);
	}
	if (right) {
		this->_traverse_DFSPostOrder(right, nodesList);
	}
	nodesList.push_back(node);
}

void BSTree::_traverse_BFS(Node* node, std::vector<Node*>& nodesList) {
	std::queue<Node*> tempQ;
	if (node == NULL) {
		return;
	}
	tempQ.push(node);
	while (!(tempQ.empty())) {			
		Node* tempNode = tempQ.front();
		nodesList.push_back(tempNode);
		Node* left = tempNode->getLeft();
		Node* right = tempNode->getRight();
		if (left) {
			tempQ.push(left);
		}
		if (right) {
			tempQ.push(right);
		}
		tempQ.pop();
	}
}
