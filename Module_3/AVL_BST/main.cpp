#include <iostream>

template <class T>
struct Node {
	T key{}; // Data contained in Node
	size_t level{1}; // Level on which Node is located

	Node* left{}; // Pointer to left child
	Node* right{}; // Pointer to right child

	// Constructors
	Node() = default;
	Node(T key) :
		key{key}, left{nullptr}, right{nullptr} {}
};

template <class T>
class AVLBinaryTree {
	private:
		// Root of tree
		Node<T>* _root{};

	public:
		// Constructors & destructor
		explicit AVLBinaryTree() = default; // No need to define specific parameters
		~AVLBinaryTree(); // Destructor - destroys tree after finishing working with it

		// Adds Node to tree with priority
		void insert(const T&);
		// Removes Node from tree saving tree properties
		void remove(const T&);
		// Finds a Node with a specific key
		const Node<T>* search(const T&) const;

	private:
		// Clears tree in postorder
		void clear_tree(Node<T>*);

		// Inserts Node into a tree if key is unique
		Node<T>* insert(Node<T>*, const T&);
		// Removes Node with specified key from tree
		Node<T>* find_min(Node<T>*);
		Node<T>* remove_min(Node<T>*);
		Node<T>* remove(Node<T>*, const T&);
		// Finds Node with a specific key in a tree
		const Node<T>* search(const Node<T>*, const T&) const;

		// Helpful methods for balancing
		// Returs the level on which Node is located
		const size_t& node_level(Node<T>*) const;
		// Computes difference between two branches
		const int& bfactor(Node<T>*) const;
		// Fixes height between two branches
		void fix_height(Node<T>*);
		// Rotates tree onto right side if right branch is heavier
		Node<T>* rotate_right(Node<T>*);
		// Rotates tree onto left side if left branch is heavier
		Node<T>* rotate_left(Node<T>*);
		// Restores balance between branches
		Node<T>* balance(Node<T>*);
};

// Implementation of AVL-tree class
//----------------------------------------------------------------------------------------------------------------------
template <class T>
void AVLBinaryTree<T>::clear_tree(Node<T>* tree) {
	if (tree != nullptr) {
		clear_tree(tree->left);
		clear_tree(tree->right);
		delete tree;
	}
}

template <class T>
AVLBinaryTree<T>::~AVLBinaryTree() {
	clear_tree(_root);
	_root = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
template <class T>
const size_t& AVLBinaryTree<T>::node_level(Node<T>* node) const {
	return node ? node->level : 0;
}

template <class T>
const int& AVLBinaryTree<T>::bfactor(Node<T>* node) const {
	return static_cast<int>(node_level(node->right) - node_level(node->left));
}

template <class T>
void AVLBinaryTree<T>::fix_height(Node<T>* node) {
	size_t left_level{node_level(node->left)};
	size_t right_level{node_level(node->right)};
	node->level = (left_level > right_level ? left_level : right_level) + 1;
}

template <class T>
Node<T>* AVLBinaryTree<T>::rotate_right(Node<T>* node) {
	Node<T>* left(node->left);
	node->left = left->right;
	left->right = node;

	fix_height(node);
	fix_height(left);

	return left;
}

template <class T>
Node<T>* AVLBinaryTree<T>::rotate_left(Node<T>* node) {
	Node<T>* right(node->right);
	node->right = right->left;
	right->left = node;

	fix_height(node);
	fix_height(right);

	return right;
}

template <class T>
Node<T>* AVLBinaryTree<T>::balance(Node<T>* node) {
	fix_height(node);

	if (bfactor(node) == 2) {

		if (bfactor(node->right) < 0) {
			node->right = rotate_right(node->right);
		}

		return rotate_left(node);
	}

	if (bfactor(node) == -2) {

		if (bfactor(node->left) > 0) {
			node->left = rotate_left(node->left);
		}

		return rotate_right(node);
	}

	return node;
}

//----------------------------------------------------------------------------------------------------------------------
template <class T>
Node<T>* AVLBinaryTree<T>::insert(Node<T>* root, const T& value) {
	if (!root) {
		std::cout << "OK" << std::endl;
		return new Node<T>(value);
	} else if (value == root->key){
		std::cout << "FAIL" << std::endl;
	} else if (value < root->key) {
		root->left = insert(root->left, value);
	} else if (value > root->key) {
		root->right = insert(root->right, value);
	}

	return balance(root);
}

template <class T>
void AVLBinaryTree<T>::insert(const T& value) {
	_root = insert(_root, value);
}

//----------------------------------------------------------------------------------------------------------------------
template <class T>
Node<T>* AVLBinaryTree<T>::find_min(Node<T>* node) {
	return node->left ? find_min(node->left) : node;
}

template <class T>
Node<T>* AVLBinaryTree<T>::remove_min(Node<T>* node) {
	if (!node->left) {
		return node->right;
	}

	node->left = remove_min(node->left);
	return balance(node);
}

template <class T>
Node<T>* AVLBinaryTree<T>::remove(Node<T>* root, const T& value) {
	if (!root) {
		std::cout << "FAIL" << std::endl;
		return nullptr;
	}

	if (value < root->key) {
		root->left = remove(root->left, value);
	} else if (value > root->key) {
		root->right = remove(root->right, value);
	} else {
		Node<T>* left(root->left);
		Node<T>* right(root->right);

		delete root;

		std::cout << "OK" << std::endl;

		if (!right) {
			return left;
		}

		Node<T>* min = find_min(right);
		min->right = remove_min(right);
		min->left = left;

		return balance(min);
	}

	return balance(root);
}

template <class T>
void AVLBinaryTree<T>::remove(const T& value) {
	_root = remove(_root, value);
}

//----------------------------------------------------------------------------------------------------------------------
template <class T>
const Node<T>* AVLBinaryTree<T>::search(const Node<T>* root, const T& value) const {
	while (root != nullptr) {

		if (root->key == value) {
			return root;
		} else if (root->key < value) {
			root = root->right;
		} else {
			root = root->left;
		}
	}

	return nullptr;
}

template <class T>
const Node<T>* AVLBinaryTree<T>::search(const T& value) const {
	return search(_root, value);
}

//----------------------------------------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
	AVLBinaryTree<std::string> AVL_BST;

	while (true) {
		char command = 0;
		std::string value;
		std::cin >> command >> value;

		if (std::cin.fail()) {
			break;
		}

		switch(command) {

			case '?': std::cout << (AVL_BST.search(value) != nullptr ? "OK" : "FAIL") << std::endl;
				break;

			case '+': AVL_BST.insert(value);
				break;

			case '-': AVL_BST.remove(value);
				break;
		}
	}

	return 0;
}
