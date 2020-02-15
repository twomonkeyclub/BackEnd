//二叉树的前序，中序和后序遍历是根据根结点的位置来判断
//前序：根左右
//中序：左根右
//后序：左右根
//叶子结点也需要判断左右节点，只不过左右节点都是空


//非递归前序遍历，根左右
//(1) 从根结点开始，向左遍历压栈并输出
//(2) 一直找到二叉树最左边的结点，将最左侧的叶子结点压入栈
//(3) 出栈，指向该结点的右孩子
//(4) 将右孩子作为根节点重复(1)(2)(3)
void Pretravel(BiNode* root)
{
	if (!root)
	{
		return;
	}

	stack<BiNode*> st;
	BiNode* p = root;

	while (p || !st.empty())
	{
		while (p){
			cout << p->data;
			st.push(p);
			p = p->lchild;
		}
		

		if (!st.empty())
		{
			p = st.top();
			st.pop();
			p = p->rchild;
		}
	}
}

//非递归中序遍历，左根右
//先输出最左侧叶子结点
//(1) 从根结点开始，往左遍历压栈
//(2) 找到最左侧的叶子结点,也将其压栈
//(3) 出栈，输出结点值，并指向该结点的右孩子
//(4) 将右孩子作为根结点继续(1)(2)(3)
void Intravel(BiNode* root){
	if (root == NULL)
		return;
	stack<BiNode*>st;
	BiNode *p = root;

	while (!st.empty() || p){
		while (p){
			st.push(p);
			p = p->lchild;
		}

		if (!st.empty()){
			p = st.top();
			st.pop();
			cout << p->data;
			p = p->rchild;
		}
	}
}

//非递归后序遍历，左右根
// 维护一个pre结点
//(1) 从根结点开始，往左遍历压栈
//(2) 找到最左侧的叶子结点,也将其压栈
//(3) 出栈，判断当前的结点是不是叶子结点或是不是根结点(上一次访问的是右孩子)
//(4) 若是，输出结点值，更新pre指针
//(5) 若不是，指向右孩子，重复(1)(2)(3)(4)
void behtravel(BiNode* root){
	if (NULL == root)
		return;
	stack<BiNode *> st;
	BiNode * p = root;
	BiNode * pre = NULL;
	while (!st.empty || p){
		while (p){
			st.push(p);
			p = p->lchild;
		}
		if (!st.empty()){
			p = st.top();
			st.pop();

			//右孩子为空(左叶子结点和右叶子结点) 或 刚刚访问的是该结点的右孩子(根结点)
			if (!p->rchild || pre == p->rchild){
				cout << p->data;
				pre = p;
			}
			//右孩子不为空，则将刚刚出栈的结点重新压入，指向结点的右孩子
			else{
				st.push(p);
				p = p->rchild;
			}
		}
	}
}