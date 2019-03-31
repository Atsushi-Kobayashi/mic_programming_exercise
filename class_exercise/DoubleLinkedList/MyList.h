#pragma once

struct Node
{
public:
	Node();//�f�t�H���g�R���X�g���N�^,0��m_value��������
	Node(double v);//���͂��ꂽdouble�l��m_value��������
	
	~Node() {};

	double m_value;
	Node* m_next_node_ptr;
	Node* m_prev_node_ptr;

	//static�ɂ��Ȃ���,
	//"�ÓI�łȂ������o�[�Q�Ƃ͓���̃I�u�W�F�N�g����Ƃ��鑊�ΎQ�Ƃł���K�v������܂�"
	//���Ăł�Bstatic��class�ŗL�ŁC�I�u�W�F�N�g�����Ȃ��Ă��Q�Ƃł�����̂炵��
	static void link_to_next_node(Node *node, Node *next_node);
	
};

class MyList 
{
public:
	MyList();
	~MyList();
	void getSize();
	void addSize();
	void reduceSize();
	void display();
	void pushBackNode(double value);
	int replaceNode(double value, int index);
	int insertNode(double value, int index);
	void popBackNode();
	int deleteNode(int index);

private:
	Node* m_dummy;
	int m_size;

};