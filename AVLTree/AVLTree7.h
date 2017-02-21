/*
*�ļ�˵��:AVLTree���������ʵ��
*����:��С��
*����:2017-02-13
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#pragma once
#include<assert.h>
#include<stack>
template<typename K,typename V>
struct AVLTreeNode{
	AVLTreeNode *_parent;
	AVLTreeNode *_left;
	AVLTreeNode *_right;
	const K _key;
	V _value;
	int _bf;

	AVLTreeNode(const K &k,const V &v)
		:_parent(NULL)
		,_left(NULL)
		,_right(NULL)
		,_key(k)
		,_value(v)
		,_bf(0){}
};

template<typename K,typename V>
class AVLTree{
public:
	typedef AVLTreeNode<K,V> Node;
	AVLTree():_root(NULL){}
	~AVLTree(){}
public:
	
	bool Insert(const K &k,const V &v){
		if(_root==NULL){
			_root = new Node(k,v);
		}else{
			Node *parent = NULL;
			Node *cur = _root;
			//1.�Ҳ���λ��
			while(cur!=NULL){
				if(cur->_key > k){
					parent = cur;
					cur = cur->_left;
				}else if(cur->_key < k){
					parent = cur;
					cur = cur->_right;
				}else{
					//�ظ�keyֵ,���ؼ�
					return false;
				}
			}
			//2.��������
			cur = new Node(k,v);
			if(parent->_key > k){
				parent->_left = cur;
			}else{
				parent->_right = cur;
			}
			cur->_parent = parent;
			//3.����ƽ������

			while(parent!=NULL){
				if(parent->_left == cur){
					parent->_bf -= 1;
				}else{
					parent->_bf += 1;
				}
				if(parent->_bf == -1 ||
					parent->_bf == 1){
					//����������ϵ���
						cur = parent;
						parent = parent->_parent;
				}else if(parent->_bf == 0){
					//����Ҫ����
					break;
				}else if(parent->_bf == 2||
					parent->_bf == -2){
					//��Ҫ������ת����
						if(parent->_bf == -2&&
							cur->_bf == -1){
							//����
							RotateR(parent);
						}else if(parent->_bf == -2&&
							cur->_bf == 1){
							//����˫��
							RotateLR(parent);
						}else if(parent->_bf == 2 &&
							cur->_bf == -1){
							//����˫��
							RotateRL(parent);
						}else{
							//����
							RotateL(parent);
						}
						break;
				}else{
					//ƽ�����ӳ���
					std::cout<<"ƽ�����ӳ��� key = "<<parent->_key<<std::endl;
					assert(false);
				}

			}
		}
		return true;
	}
	
	void InOrderNonR(){
		stack<Node *> s;
		Node *cur = _root;
		while(cur!=NULL || !s.empty()){
			while(cur!=NULL){
				s.push(cur);
				cur = cur->_left;
			}

			Node *top = s.top();
			s.pop();
			cout<<top->_key<<" ";

			cur = top->_right;
		
		}
		cout<<endl;
	}
	/*
	void InOrderNonR(){
		stack<Node *> s;
		Node *cur = _root;

		while(cur!=NULL || !s.empty()){
			while(cur!=NULL){
				s.push(cur);
				cur = cur->_left;
			}

			Node *top = s.top();
			s.pop();
			std::cout<<top->_key<<" ";

			cur = top->_right;
		}
		std::cout<<std::endl;
	}
	*/
	size_t Depth(){
		return _Depth(_root);
	}
	bool IsBalance(){
		return _IsBalance(_root);
	}
	bool IsBalanceOP(){
		size_t depth = 0;
		return _IsBalanceOP(_root,depth);
	}
protected:

	/*
	bool _IsBalanceOP(Node *root,size_t &depth){
		if(root==NULL){
			depth = 0;
			return true;
		}
		size_t leftDepth = 0;
		size_t rightDepth = 0;
		bool left = _IsBalanceOP(root->_left,leftDepth);
		bool right = _IsBalanceOP(root->_right,rightDepth);
		if(left && right){
			int sub = leftDepth - rightDepth;
			if(sub <= 1 || sub>= -1){
				depth = leftDepth>rightDepth?leftDepth+1:rightDepth+1;
				return true;
			}
		}
		return false;
	}
	bool _IsBalanceOP(Node *root,size_t &depth){
		if(root==NULL){
			depth = 0;
			return true;
		}
		size_t ld = 0;	//leftdepth ���������
		size_t rd = 0;	//rightdepth ���������
		bool l = _IsBalanceOP(root->left,ld);	//l,�������Ƿ�ƽ��
		bool r = _IsBalanceOP(root->_right,rd);	//r,�������Ƿ�ƽ��
		if(l && r){
			int sub = ld - rd;
			if(sub<=1 || sub>=-1){
				depth = ld>rd?ld+1:rd+1;
				return true;
			}
		}
		return false;
	}
	*/
	bool _IsBalanceOP(Node* root,size_t &depth){
		if(root == NULL){
			depth = 0;
			return true;
		}
		size_t ld = 0;
		size_t rd = 0;
		bool l = _IsBalanceOP(root->_left,ld);
		bool r = _IsBalanceOP(root->_right,rd);
		if(l && r){
			int sub = ld - rd;
			if(sub<=1 || sub>=-1){
				depth = ld>rd?ld+1:rd+1;
				return true;
			}
		}
		return false;
	}
	
	bool _IsBalance(Node *root){
		if(root==NULL){
			return true;
		}
		size_t leftD = _Depth(root->_left);
		size_t rightD = _Depth(root->_right);
		int sub = rightD - leftD;
		if(sub!=root->_bf){
			std::cout<<"ƽ�������쳣 key="<<root->_key<<std::endl;
		}
		if(sub > 1 || sub < -1){
			return false;
		}
		return _IsBalance(root->_left)&&_IsBalance(root->_right);
	}
	
	size_t _Depth(Node * root){
		if(root==NULL){
			return 0;
		}
		size_t leftD = _Depth(root->_left);
		size_t rightD = _Depth(root->_right);
		return leftD>rightD?leftD+1:rightD+1;
	}

	void RotateR(Node *parent){
		Node *subL = parent->_left;
		Node *subLR = subL->_right;

		parent->_left = subLR;
		if(subLR!=NULL){
			subLR->_parent = parent;
		}

		Node *pparent = parent->_parent;
		if(pparent==NULL){
			_root = subL;
			_root->_parent = NULL;
		}else{
			if(pparent->_left == parent){
				pparent->_left = subL;
			}else{
				pparent->_right = subL;
			}
			subL->_parent = pparent;
		}

		subL->_right = parent;
		parent->_parent = subL;

		parent->_bf = 0;
		subL->_bf = 0;
	}

	void RotateL(Node *parent){
		Node *subR = parent->_right;
		Node *subRL = subR->_left;

		parent->_right = subRL;
		if(subRL!=NULL){
			subRL->_parent = parent;
		}

		Node *pparent = parent->_parent;
		if(pparent==NULL){
			_root = subR;
			_root->_parent = NULL;
		}else{
			if(pparent->_left == parent){
				pparent->_left = subR;
			}else{
				pparent->_right = subR;
			}
			subR->_parent = pparent;
		}

		subR->_left = parent;
		parent->_parent = subR;

		parent->_bf = 0;
		subR->_bf = 0;
	}
	
	void RotateRL(Node *parent){
		Node *subR = parent->_right;
		Node *subRL = subR->_left;	
		int bf = subRL->_bf;

		RotateR(subR);
		RotateL(parent);


		//        subRL ���ڵ�
		//parent ����    subR �Һ���
		if(bf == -1){
			parent->_bf = 0;
			subR->_bf = 1;
		}else if(bf == 0){
			subR->_bf = parent->_bf = 0;
		}else{
			parent->_bf = -1;
			subR->_bf = 0;
		}

		subRL->_bf = 0;
	}
	void RotateLR(Node *parent){
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		int bf = subLR->_bf;
		
		RotateL(subL);
		RotateR(parent);

		//		 subLR ���ڵ�
		//subL ����    parent �Һ���
		if(bf == -1){
			subL->_bf = 0;
			parent->_bf = 1;
		}else if(bf == 0){
			subL->_bf = parent->_bf = 0;
		}else{
			subL->_bf = -1;
			parent->_bf = 0;
		}
		subLR->_bf = 0;
	}
private:
	Node *_root;
};