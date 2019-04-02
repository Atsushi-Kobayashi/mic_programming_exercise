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
		Node();//デフォルトコンストラクタ,0でm_valueを初期化
		Node(double v);//入力されたdouble値でm_valueを初期化

		~Node() {};

		double m_value;
		Node* m_next_node_ptr;
		Node* m_prev_node_ptr;

		//インスタンスを作らなくても参照できるようstaticで関数定義
		static void link_to_next_node(Node *node, Node *next_node);

	};

private:
	Node* m_dummy;
	int m_size;

};