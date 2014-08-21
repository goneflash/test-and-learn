#include <iostream>
#include <vector>

using namespace std;

//#define DEBUG
#define TEST_STRUCT_NODE
//#define TEST_DELETE_NODE
//#define TEST_BST_NODE

/*
In C++, struct could have constructor, as well as  multiple constructors
*/
template <class T>
struct StructNode{
	T value;
	StructNode *left, *right;
	StructNode(){value = 0; left = NULL; right = NULL;}
	StructNode(T v) : value(v){left = NULL; right = NULL;}
};

// Insert a node
template <class T>
void insertNode(StructNode<T> *root, T value){
	if (!root){
#ifdef DEBUG
		cout << "Error." << endl;
#endif
		return;
	}
	if (root->value > value){
		if (!root->left){
			root->left = new StructNode<T>;
			root->left->value = value;
#ifdef DEBUG
		cout << "Inserted to left subtree " << value << endl;
#endif
		}
		else{
#ifdef DEBUG
			cout << "Insert to left subtree " << value << endl;
#endif
			insertNode(root->left, value);
		}

	}
	else{ // root->value <= value
		if (!root->right){
			root->right = new StructNode<T>;
			root->right->value = value;
#ifdef DEBUG
		cout << "Inserted to right subtree " << value << endl;
#endif
		}
		else{
#ifdef DEBUG
			cout << "Insert to right subtree " << value << endl;
#endif
			insertNode(root->right, value);
		}
	}

	return;
}

// Init with a vector
template <class T>
void initWithVector(StructNode<T> **root, vector<T> value){
	for (typename vector<T>::iterator itr = value.begin(); itr != value.end(); itr++){
#ifdef DEBUG
		cout << "Now inserting " << *itr << endl;
#endif
		if (!(*root)){
			(*root) = new StructNode<T>;
			(*root)->value = *itr;
		}
		else{
			insertNode((*root), *itr);
		}
#ifdef DEBUG
		cout << "Now root " << (*root)->value << endl;
#endif
	}
}

// Find a node
template <class T>
StructNode<T>* findNode(StructNode<T> *root, T value){
	if (!root)
		return NULL;
	if (root->value == value)
		return root;
	if (root->value > value)
		return findNode(root->left, value);
	else
		return findNode(root->right, value);
}

// Find a node's parent
template <class T>
StructNode<T>* findNodeParent(StructNode<T> *root, StructNode<T> *node){
	if (!root){
		return NULL;
	}
	if (root->left == node || root->right == node)
		return root;
	if (root->value > node->value){
#ifdef DEBUG
		cout << "Parent is in the left." << endl;
#endif
		return findNodeParent(root->left, node);
	}
	else{
#ifdef DEBUG
		cout << "Parent is in the right." << endl;
#endif
		return findNodeParent(root->right, node);
	}
}

// Find max node in a BST tree
template <class T>
StructNode<T>* findMaxNode(StructNode<T> *root){
	if (!root)
		return NULL;
	if (!root->right)
		return root;
	else
		return findMaxNode(root->right);
}
// Find min node in a BST tree
template <class T>
StructNode<T>* findMinNode(StructNode<T> *root){
	if (!root)
		return NULL;
	if (!root->left)
		return root;
	else
		return findMinNode(root->left);
}

// Delete a node
// *** Have to find a node then to delete
// If there are duplicates, have to find all nodes
template<class T>
void deleteNode(StructNode<T> *root, T value){
	StructNode<T> *node;
	while (node = findNode(root, value)){
#ifdef DEBUG
		cout << "Delete Process: ";
		preOrder(root);
		cout << endl;
#endif
		// if it has only one child
		if ((node->left != NULL) ^ (node->right != NULL)){
			StructNode<T>* tmp;
			if (!node->left)
				tmp = node->right;
			else
				tmp = node->left;
			node->value = tmp->value;
			node->left = tmp->left;
			node->right = tmp->right;
			delete tmp;
			continue;
		}
		// if it is root without children
		if (node == root && !node->left && !node->right){
			delete root;
			root = NULL;
			continue;
		}
		// if it is leaf
		if (!node->left && !node->right){
			StructNode<T> *parent = findNodeParent(root, node);
#ifdef DEBUG
			cout << "Parent is " << parent->value << endl;
#endif
			// if parent == NULL, then error
			if (parent->left == node)
				parent->left = NULL;
			else
				parent->right = NULL;
			delete node;
			continue;
		}
		// if it has two children
		StructNode<T> *tmp = findMaxNode(node->left);
#ifdef DEBUG
		cout << "Root is " << root->value << " Now node is " << node->value << endl;
		cout << "Max in left subtree is " << tmp->value << endl;
#endif
		StructNode<T> *parent = findNodeParent(root, tmp);
		node->value = tmp->value;
#ifdef DEBUG
		cout << "Parent is " << parent->value << endl;
#endif
		StructNode<T> **tmp_ptr;
		if (parent->left == tmp)
			tmp_ptr = &(parent->left);
		else
			tmp_ptr = &(parent->right);
		if (!tmp->left)
			*tmp_ptr = NULL;
		else
			*tmp_ptr = tmp->left;
		delete tmp;
	}
}

// Traverse use in-order
template <class T>
void inOrder(StructNode<T> *root){
	if (!root)
		return;
	cout << root->value << " ";
	inOrder(root->left);
	inOrder(root->right);
}

// Traverse use pre-order
template <class T>
void preOrder(StructNode<T> *root){
	if (!root)
		return;
	preOrder(root->left);
	cout << root->value << " ";
	preOrder(root->right);
}


// Traverse use poste-order
template <class T>
void postOrder(StructNode<T> *root){
	if (!root)
		return;
	postOrder(root->right);
	cout << root->value << " ";
	postOrder(root->left);
}

// Find next in post-order
template <class T>
StructNode<T>* findNextNodePostOrder(StructNode<T> *root, StructNode<T> *node){
	// speical judge
	if (!root){
#ifdef DEBUG
		cout << "Empty Tree." << endl;
#endif
		return NULL;
	}

	StructNode<T> *parent;
	parent = findNodeParent(root, node);
	// if it is root
	if (!parent){
#ifdef DEBUG
		cout << "This is the last node of the tree in post order traverse." << endl;
#endif
		return NULL;
	}
	// if it is the right child of its parent
	if (parent->right == node){
#ifdef DEBUG
		cout << "It is the right child of its parent." << endl;
#endif
		return parent;
	}
	// if it is the left child of its parent
	if (parent->left == node){
		// parent has no right child
		if (!parent->right){
#ifdef DEBUG
		cout << "It is the right child of its parent which has no right child" << endl;
#endif
			return parent;
		}
		StructNode<T> *next_node = parent->right;

		while(true){
			while (next_node->left){
#ifdef DEBUG
				cout << "Go to " << next_node->value << "'s left child ";
#endif
				next_node = next_node->left;
#ifdef DEBUG
				cout << next_node->value << endl;
#endif
			}
			if (!next_node->right)
				break;
			next_node = next_node->right;
		}
		return next_node;
	}
	// Error
#ifdef DEBUG
	cout << "Error. Couldn't find parent information." << endl;
	return NULL;
#endif
}

// Find next in in-order
template <class T>
StructNode<T>* findNextNodeInOrder(StructNode<T> *root, StructNode<T> *node){
	// speical judge
	if (!root){
#ifdef DEBUG
		cout << "Empty Tree." << endl;
#endif
		return NULL;
	}
	if (node->left){
#ifdef DEBUG
		cout << "Next is its left child" << endl;
#endif
		return node->left;
	}
	if (node->right){
#ifdef DEBUG
		cout << "Next is its right child" << endl;
#endif
		return node->right;
	}

	StructNode<T> *parent;
	parent = findNodeParent(root, node);

	while (parent){
		if (parent->right && parent->right != node)
			break;
		node = parent;
		parent = findNodeParent(root, parent);
	}

	if (!parent){
#ifdef DEBUG
		cout << "It is root and the tree only has root, or the node has no acestors." << endl;
#endif
		return NULL;
	}

	if (!parent->right){
#ifdef DEBUG
		cout << "It is the last node." << endl;
#endif
		return NULL;
	}

	return parent->right;
}

// Find next in pre-order
template <class T>
StructNode<T>* findNextNodePreOrder(StructNode<T> *root, StructNode<T> *node){
	// speical judge
	if (!root){
#ifdef DEBUG
		cout << "Empty Tree." << endl;
#endif
		return NULL;
	}
	if (node->right){
#ifdef DEBUG
		cout << "Next is its right subtree's first node" << endl;
#endif
		node = node->right;
		while (node->left)
			node = node->left;
		return node;
	}

	if (root == node){
#ifdef DEBUG
		cout << "It is root and the last node" << endl;
#endif
		return NULL;
	}

	StructNode<T> *parent;
	parent = findNodeParent(root, node);

	while (parent && parent->right == node){
		node = parent;
		parent = findNodeParent(root, parent);
	}

	return parent;
}

template <class T>
class BST_Node{
public:
	BST_Node(){left = NULL; right = NULL;}
	BST_Node(T v) : value(v){}
	//~BST_Node(){}

	T value;
	BST_Node *left, *right;
};


int main(){

#ifdef TEST_STRUCT_NODE
	// Test struct based node
	StructNode<int> struct_node;
	StructNode<int> struct_node1(2);

	cout << struct_node.value << endl;
	cout << struct_node1.value << endl;

	// test init tree, insert node
	/*
	   7
	|     \
	3	   9	
	| \    | \
	1  4   8 10
	 \  \  |\  
	  2  6 7 8 
	  	 |
	  	 5   
	*/
	StructNode<int>* root = NULL;
	vector<int> v{7, 9, 3, 4, 1, 8, 2, 10, 7, 8, 6, 5};
	initWithVector(&root, v);
	preOrder(root);
	cout << endl;
	inOrder(root);
	cout << endl;

#ifdef TEST_DELETE_NODE
	// test delete node
	deleteNode(root, 4);
	preOrder(root);
	cout << endl;
	inOrder(root);
	cout << endl;
	deleteNode(root, 10);
	preOrder(root);
	cout << endl;
	inOrder(root);
	cout << endl;
	deleteNode(root, 7);
	preOrder(root);
	cout << endl;
	inOrder(root);
	cout << endl;
#endif

	// test find next post-order node
	StructNode<int> *node;

	for (int i = 1; i <= 10; i++){
		node = findNode(root, i);
		cout << "Node is " << node->value << endl;
		StructNode<int> *next_node;
		next_node = findNextNodePostOrder(root, node);
		if (next_node)
			cout << "Next node is " << next_node->value << " in post-order" << endl;	
	}

	// test find next in-order node
	for (int i = 1; i <= 10; i++){
		node = findNode(root, i);
		cout << "Node is " << node->value << endl;
		StructNode<int> *next_node;
		next_node = findNextNodeInOrder(root, node);
		if (next_node)
			cout << "Next node is " << next_node->value << " in in-order" << endl;	
	}

	// test find next pre-order node
	for (int i = 1; i <= 10; i++){
		node = findNode(root, i);
		cout << "Node is " << node->value << endl;
		StructNode<int> *next_node;
		next_node = findNextNodePreOrder(root, node);
		if (next_node)
			cout << "Next node is " << next_node->value << " in pre-order" << endl;	
	}

	preOrder(root);
	cout << endl;
	inOrder(root);
	cout << endl;


#endif

#ifdef TEST_BST_NODE
	BST_Node<int> *root;


#endif


	return 0;
}