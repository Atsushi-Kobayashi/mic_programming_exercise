#include"MyList.h"
#include<iostream>

MyList::Node::Node() : m_value(0) {}
MyList::Node::Node(double v) : m_value(v) {}

void MyList::Node::link_to_next_node(Node *node, Node *next_node) {
	node->m_next_node_ptr = next_node;
	next_node->m_prev_node_ptr = node;
};



MyList::MyList() :
	m_dummy(new Node), m_size(0)
{
	Node::link_to_next_node(m_dummy, m_dummy);
}
MyList::~MyList() {
	Node *ptr = m_dummy->m_next_node_ptr;
	while (1) {
		Node *next = ptr->m_next_node_ptr;
		if (ptr == m_dummy) {
			delete ptr;
			break;
		}
		delete ptr;
		ptr = next;
	}
}


void MyList::getSize() {
	std::cout << "List size: " << m_size << "\n";
}

void MyList::display() {
	std::cout << "[";
	Node *ptr = m_dummy->m_next_node_ptr;
	while (1) {
		Node *next = ptr->m_next_node_ptr;

		std::cout << ptr->m_value;
		ptr = next;
		if (ptr == m_dummy) {
			std::cout << "]\n";
			break;
		}
		std::cout << ", ";
	}
}

void MyList::pushBackNode(double value) {
	//注：newによって宣言しないとこの変数のスコープがこの関数内のみとなってしまう
	Node *back_node = new Node(value);

	Node::link_to_next_node(m_dummy->m_prev_node_ptr, back_node);
	Node::link_to_next_node(back_node, m_dummy);

	m_size++;
}

int MyList::replaceNode(double value, int index) {
	if (index <= 0 || index > m_size) {
		std::cout << "Index error in function 'replaceNode'\n";
		return -1;
	}

	Node *ptr = m_dummy;
	for (int i = 0; i < index; ++i) {
		Node *next = ptr->m_next_node_ptr;
		ptr = next;
	}

	ptr->m_value = value;
	return 1;
}

int MyList::insertNode(double value, int index) {

	if (index != 0) {

		Node *inserted_node = new Node(value);
		inserted_node->m_value = value;

		if (index > 0) {
			Node *next_node = m_dummy;
			for (int i = 0; i < index&&i < m_size+1; ++i) {
				Node *next_tmp = next_node->m_next_node_ptr;
				next_node = next_tmp;
			}

			Node *prev_node = next_node->m_prev_node_ptr;
			Node::link_to_next_node(inserted_node, next_node);
			Node::link_to_next_node(prev_node, inserted_node);

			m_size++;
			return 1;
		}
		else if (index < 0) {
			Node *prev_node = m_dummy;
			for (int i = 0; i > index&&i > -m_size - 1; --i) {
				Node *prev_tmp = prev_node->m_prev_node_ptr;
				prev_node = prev_tmp;
			}

			Node *next_node = prev_node->m_next_node_ptr;
			Node::link_to_next_node(inserted_node, next_node);
			Node::link_to_next_node(prev_node, inserted_node);

			m_size++;
			return 1;
		}
	}

	else {
		std::cout << "Index error in function 'insertNode'\n";
		return -1;
	}
}

void MyList::popBackNode() {

	Node *back_node = m_dummy->m_prev_node_ptr;
	Node *prev_back_node = back_node->m_prev_node_ptr;

	Node::link_to_next_node(prev_back_node, m_dummy);

	delete back_node;

	m_size--;
}

int MyList::deleteNode(int index) {

	Node *target_node = m_dummy;
	if (index > 0 && index <= m_size) {
		for (int i = 0; i < index; ++i) {
			Node *next_tmp = target_node->m_next_node_ptr;
			target_node = next_tmp;
		}

		Node *target_next_node = target_node->m_next_node_ptr;
		Node *target_prev_node = target_node->m_prev_node_ptr;

		Node::link_to_next_node(target_prev_node, target_next_node);

		delete target_node;

		m_size--;
		return 1;
	}

	else if (index < 0 && index>=-m_size) {
		for (int i = 0; i > index; --i) {
			Node *prev_tmp = target_node->m_prev_node_ptr;
			target_node = prev_tmp;
		}

		Node *target_next_node = target_node->m_next_node_ptr;
		Node *target_prev_node = target_node->m_prev_node_ptr;

		Node::link_to_next_node(target_prev_node, target_next_node);

		delete target_node;

		m_size--;
		return 1;
	}

	else {
		std::cout << "Index error in function 'deleteNode'\n";
		return -1;
	}
}