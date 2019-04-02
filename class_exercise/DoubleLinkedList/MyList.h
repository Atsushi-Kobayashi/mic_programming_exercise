#pragma once

class MyList 
{
public:
	MyList();
	~MyList();
	void getSize();
	void display();
	void pushBackNode(double value);
	int replaceNode(double value, int index);
	int insertNode(double value, int index);
	void popBackNode();
	int deleteNode(int index);


	struct Node
	{
	public:
		Node();//�f�t�H���g�R���X�g���N�^,0��m_value��������
		Node(double v);//���͂��ꂽdouble�l��m_value��������

		~Node() {};

		double m_value;
		Node* m_next_node_ptr;
		Node* m_prev_node_ptr;

		//�C���X�^���X�����Ȃ��Ă��Q�Ƃł���悤static�Ŋ֐���`
		static void link_to_next_node(Node *node, Node *next_node);

	};

private:
	Node* m_dummy;
	int m_size;

};