// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions for AVLTree

// ----------------------helper functions for insert---------------------

template<class K, class T>
void AVLTree<K, T>::balanceLeft(AVLNode*& node, bool& taller) {
    if (!node) return;

    switch (node->balance) {
    case LH: {
        // Trước đã LH, giờ chèn thêm bên trái nữa -> mất cân bằng
        AVLNode* leftChild = node->pLeft;
        if (leftChild->balance == LH) {
            // Case LL: xoay phải đơn
            node->balance = EH;
            leftChild->balance = EH;
            rotateRight(node);
        }
        else if (leftChild->balance == RH) {
            // Case LR: xoay trái con trái rồi xoay phải node
            AVLNode* RoL = leftChild->pRight;
            // cập nhật balance
            if (RoL->balance == LH) {
                node->balance = RH;
                leftChild->balance = EH;
            }
            else if (RoL->balance == EH) {
                node->balance = EH;
                leftChild->balance = EH;
            }
            else {
                node->balance = EH;
                leftChild->balance = LH;
            }
            RoL->balance = EH;

            rotateLeft(node->pLeft);
            rotateRight(node);
        }
        taller = false;
        break;
    }
    case EH:
        // Trước đang bằng, thêm trái -> nghiêng trái
        node->balance = LH;
        taller = true;
        break;
    case RH:
        // Trước nghiêng phải, thêm trái -> cân bằng
        node->balance = EH;
        taller = false;
        break;
    }
}

template<class K, class T>
void AVLTree<K, T>::balanceRight(AVLNode*& node, bool& taller) {
    if (!node) return;

    switch (node->balance) {
    case RH: {
        // Trước đã RH, giờ chèn thêm bên phải nữa -> mất cân bằng
        AVLNode* rightChild = node->pRight;
        if (rightChild->balance == RH) {
            // Case RR: xoay trái đơn
            node->balance = EH;
            rightChild->balance = EH;
            rotateLeft(node);
        }
        else if (rightChild->balance == LH) {
            // Case RL: xoay phải con phải rồi xoay trái node
            AVLNode* LoR = rightChild->pLeft;
            // cập nhật balance
            if (LoR->balance == RH) {
                node->balance = LH;
                rightChild->balance = EH;
            }
            else if (LoR->balance == EH) {
                node->balance = EH;
                rightChild->balance = EH;
            }
            else {
                node->balance = EH;
                rightChild->balance = RH;
            }
            LoR->balance = EH;

            rotateRight(node->pRight);
            rotateLeft(node);
        }
        taller = false;
        break;
    }
    case EH:
        // Trước đang bằng, thêm phải -> nghiêng phải
        node->balance = RH;
        taller = true;
        break;
    case LH:
        // Trước nghiêng trái, thêm phải -> cân bằng
        node->balance = EH;
        taller = false;
        break;
    }
}

template<class K, class T>
typename AVLTree<K, T>::AVLNode*
AVLTree<K, T>::insertRec(AVLNode* node, const K& key, const T& value, bool& taller) {
    if (node == nullptr) {
        // Chèn node mới vào lá
        node = new AVLNode(key, value);
        taller = true;
        return node;
    }

    if (key == node->key) {
        // Khóa đã tồn tại -> không chèn
        taller = false;
        return node;
    }
    if (key < node->key) {
        node->pLeft = insertRec(node->pLeft, key, value, taller);
        if (taller) balanceLeft(node, taller);
    }
    else {
        node->pRight = insertRec(node->pRight, key, value, taller);
        if (taller) balanceRight(node, taller);
    }
    return node;
}


// ----------------------helper functions for remove---------------------

template<class K, class T>
void AVLTree<K, T>::removeRightBalance(AVLNode*& root, bool& shorter) {
    if (root->balance == LH) root->balance = EH;
    else if (root->balance == EH) {
        root->balance = RH;
        shorter = false;
    }
    else { // root->balance == RH
        AVLNode* rightTree = root->pRight;
        if (rightTree->balance == LH) {
            AVLNode* leftTree = rightTree->pLeft;

            if (leftTree->balance == LH) {
                root->balance = EH;
                rightTree->balance = RH;
            }
            else if (leftTree->balance == EH) {
                root->balance = EH;
                rightTree->balance = EH;
            }
            else { // leftTree->balance == RH
                root->balance = LH;
                rightTree->balance = EH;
            }
            leftTree->balance = EH;

            rotateRight(root->pRight);
            rotateLeft(root);
        }
        else {
            if (rightTree->balance == EH) {
                root->balance = RH;
                rightTree->balance = LH;
                shorter = false;
            }
            else { // rightTree->balance==RH
                root->balance = EH;
                rightTree->balance = EH;
            }
            rotateLeft(root);
        }
    }
}

template<class K, class T>
void AVLTree<K, T>::removeLeftBalance(AVLNode*& root, bool& shorter) {
    if (root->balance == RH) root->balance = EH;
    else if (root->balance == EH) {
        root->balance = LH;
        shorter = false;
    }
    else { // root->balance == LH
        AVLNode* leftTree = root->pLeft;
        if (leftTree->balance == RH) {
            AVLNode* rightTree = leftTree->pRight;

            if (rightTree->balance == RH) {
                root->balance = EH;
                leftTree->balance = LH;
            }
            else if (rightTree->balance == EH) {
                root->balance = EH;
                leftTree->balance = EH;
            }
            else { // rightTree->balance == LH
                root->balance = RH;
                leftTree->balance = EH;
            }
            rightTree->balance = EH;

            rotateLeft(root->pLeft);
            rotateRight(root);
        }
        else {
            if (leftTree->balance == EH) {
                root->balance = LH;
                leftTree->balance = RH;
                shorter = false;
            }
            else { // leftTree->balance==LH
                root->balance = EH;
                leftTree->balance = EH;
            }
            rotateRight(root);
        }
    }
}

template<class K, class T>
typename AVLTree<K, T>::AVLNode*
AVLTree<K, T>::removeRec(AVLNode* root, const K& removeKey, bool& shorter, bool& success) {
    if (root == nullptr) {
        shorter = false;
        success = false;
        return nullptr;
    }

    if (removeKey < root->key) {
        root->pLeft = removeRec(root->pLeft, removeKey, shorter, success);
        if (shorter) removeRightBalance(root, shorter);
    }
    else if (removeKey > root->key) {
        root->pRight = removeRec(root->pRight, removeKey, shorter, success);
        if (shorter) removeLeftBalance(root, shorter);
    }
    else { // tìm thấy
        AVLNode* removeNode = root;

        if (root->pRight == nullptr) {
            AVLNode* newRoot = removeNode->pLeft;
            success = true;
            shorter = true;
            delete removeNode;
            return newRoot;
        }
        else if (root->pLeft == nullptr) {
            AVLNode* newRoot = removeNode->pRight;
            success = true;
            shorter = true;
            delete removeNode;
            return newRoot;
        }
        else { // 2 con
            AVLNode* exchNode = root->pLeft;
            while (exchNode->pRight != nullptr) exchNode = exchNode->pRight;

            root->key = exchNode->key;
            root->data = exchNode->data;

            root->pLeft = removeRec(root->pLeft, exchNode->key, shorter, success);
            if (shorter) removeRightBalance(root,shorter);
        }
    }
    return root;
}


// ===== helper cho height/size/inorder =====

template<class K, class T>
int AVLTree<K, T>::height(AVLNode* node) const {
    if (node == nullptr) return 0;
    int lh = height(node->pLeft);
    int rh = height(node->pRight);
    return (lh > rh ? lh : rh) + 1;
}

template<class K, class T>
int AVLTree<K, T>::size(AVLNode* node) const {
    if (node == nullptr) return 0;
    return 1 + size(node->pLeft) + size(node->pRight);
}

template<class K, class T>
void AVLTree<K, T>::inorderTraversal(AVLNode* node, void(*action)(const T&)) const {
    if (node == nullptr) return;
    inorderTraversal(node->pLeft, action);
    action(node->data);
    inorderTraversal(node->pRight, action);
}


// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const {
    int height = this->getHeight();
		if (this->root == NULL)
		{
			cout << "NULL\n";
			return;
		}
		queue<AVLNode *> q;
		q.push(root);
		AVLNode *temp;
		int count = 0;
		int maxNode = 1;
		int level = 0;
		int space = pow(2, height);
		printNSpace(space / 2);
		while (!q.empty())
		{
			temp = q.front();
			q.pop();
			if (temp == NULL)
			{
				cout << " ";
				q.push(NULL);
				q.push(NULL);
			}
			else
			{
				cout << temp->data;
				q.push(temp->pLeft);
				q.push(temp->pRight);
			}
			printNSpace(space);
			count++;
			if (count == maxNode)
			{
				cout << endl;
				count = 0;
				maxNode *= 2;
				level++;
				space /= 2;
				printNSpace(space / 2);
			}
			if (level == height)
				return;
		}
}


//TODO: Implement all AVLTree<K, T> methods here



template<class K, class T>
typename AVLTree<K, T>::AVLNode*
AVLTree<K, T>::rotateRight(AVLNode*& node) {
    // Nếu không có con trái thì không xoay được
    if (node == nullptr || node->pLeft == nullptr) return node;

    AVLNode* leftChild = node->pLeft;

    // xoay
    node->pLeft = leftChild->pRight;
    leftChild->pRight = node;

    // cập nhật root của subtree (hoặc cả cây nếu xoay tại root)
    node = leftChild;
    return node;
}

template<class K, class T>
typename AVLTree<K, T>::AVLNode*
AVLTree<K, T>::rotateLeft(AVLNode*& node) {
    // Nếu không có con phải thì không xoay được
    if (node == nullptr || node->pRight == nullptr) return node;

    AVLNode* rightChild = node->pRight;

    // xoay
    node->pRight = rightChild->pLeft;
    rightChild->pLeft = node;

    // cập nhật root của subtree (hoặc cả cây nếu xoay tại root)
    node = rightChild;
    return node;
}

template<class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode* node) {
    if (node == nullptr) return;

    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template<class K, class T>
AVLTree<K, T>::AVLTree() : root(nullptr) {}

template<class K, class T>
AVLTree<K, T>::~AVLTree() {
    clearHelper(root);
    root = nullptr;
}



template<class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value) {
    bool taller = false;
    root = insertRec(root, key, value, taller);
}

template<class K, class T>
void AVLTree<K, T>::remove(const K& key) {
    bool shorter = false;
    bool success = false;
    root = removeRec(root, key, shorter, success);
    // Nếu không tìm thấy key thì cây giữ nguyên, success = false
}

template<class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    AVLNode* cur = root;
    while (cur != nullptr) {
        if (key < cur->key) cur = cur->pLeft;
        else if (key > cur->key) cur = cur->pRight;
        else return true;
    }
    return false;
}

template<class K, class T>
int AVLTree<K, T>::getHeight() const {
    return height(root);
}

template<class K, class T>
int AVLTree<K, T>::getSize() const {
    return size(root);
}


template<class K, class T>
bool AVLTree<K, T>::empty() const {
    return root == nullptr;
}

template <class K, class T>
void AVLTree<K, T>::clear() {
    clearHelper(root);
    root = nullptr;
}

template<class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
    inorderTraversal(root, action);
}



// =====================================
// Helper functions for RedBlackTree

// ===== helper cho size/clear =====

template<class K, class T>
int RedBlackTree<K, T>::sizeRec(RBTNode* node) const {
    if (!node) return 0;
    return 1 + sizeRec(node->left) + sizeRec(node->right);
}

template<class K, class T>
void RedBlackTree<K, T>::clearRec(RBTNode* node) {
    if (!node) return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}

// ===== helper cho insert =====
template<class K, class T>
void RedBlackTree<K, T>::fixInsert(RBTNode* N) {
    // N là node vừa chèn (đỏ)

    while (N != root && N->parent->color == RED) {
        RBTNode* P = N->parent;
        RBTNode* G = P->parent;

        if (P == G->left) {
            RBTNode* U = G->right; // uncle
            if(U && U->color==RED) {/* case 1 - change the colours */ 
                P->color = BLACK;
                U->color = BLACK;
                G->color = RED;
                N = G; /* Move x up the tree */ 
            }
            else { // uncle is black node or nullptr
                if (N == P->right) {
                    /* and x is to the right */
                    /* case 2 - move x up and rotate */
                    N = P;
                    rotateLeft(P);
                    P = N->parent;
                    G = P->parent;
                }
                /* case 3 */
                P->color = BLACK;
                G->color = RED;
                rotateRight(G);
            }
        }
        else { // P là con phải, cũng tương tự
            RBTNode* U = G->left;
            if (U && U->color == RED) {
                P->color = BLACK;
                U->color = BLACK;
                G->color = RED;
                N = G;
            }
            else {
                if (N == P->left) {
                    N = P;
                    rotateRight(P);
                    P = N->parent;
                    G = P->parent;
                }
                P->color = BLACK;
                G->color = RED;
                rotateLeft(G);
            }
        }
    }
    root->color = BLACK;
}


// ===== helper cho remove =====
template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::maximumNode(RBTNode* node) const {
    if (!node) return nullptr;
    while (node->right) node = node->right;
    return node;
}

template<class K, class T>
void RedBlackTree<K, T>::fixDelete(RBTNode* x, RBTNode* parent) {
    // x có thể nullptr (NIL), parent là cha của x (sau khi nối lại cây)
    while (x != root && (x == nullptr || x->color == BLACK)) {
        if (!parent) break;

        // ============ X LÀ CON TRÁI ============
        if (x == parent->left) {
            RBTNode* s = parent->right;
            
            // (Case 1 là xóa node đỏ nên không cần sửa -> không gọi hàm fixDelete)
            // ----- Case 2: sibling đỏ -----  
            if (s&& s->color == RED) {
                s->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                // sibling mới sau khi xoay là con phải của parent
                s = parent->right;
            }

            // nếu không có sibling / không có con → coi như đen
            bool sLeftBlack = (!s || !s->left || s->left->color == BLACK);
            bool sRightBlack = (!s || !s->right || s->right->color == BLACK);

            // ----- Case 3: sibling đen, hai con đen -----
            if (sLeftBlack && sRightBlack) {
                if (s) s->color = RED;
                // double-black dồn lên parent
                x = parent;
                parent = x->parent;
            }
            else {
                // ----- Case 4: sibling đen, "con gần" đỏ, "con xa" đen -----
                // x là con trái => con gần là s->left, con xa là s->right
                if (s && sRightBlack && s->left && s->left->color==RED) {
                    s->color = RED;
                    s->left->color = BLACK;
                    // xoay phải tại sibling để biến con xa thành đỏ
                    rotateRight(s);
                    s = parent->right; // cập nhật sibling mới
                }
                
                // ----- Case 5: sibling đen, "con xa" đỏ -----
                if (s) {
                    s->color = parent->color;
                    parent->color = BLACK;
                    if (s->right) s->right->color = BLACK;
                }
                rotateLeft(parent);
                // sau bước này, double-black được xử lý xong
                break;
            }
        }
        // ============ X LÀ CON PHẢI ============
        else {
            RBTNode* s = parent->left;

            // ----- Case 2 đối xứng: sibling đỏ -----
            if (s && s->color == RED) {
                s->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                s = parent->left;
            }

            bool sLeftBlack = (!s || !s->left || s->left->color == BLACK);
            bool sRightBlack = (!s || !s->right || s->right->color == BLACK);

            // ----- Case 3 đối xứng: sibling đen, hai con đen -----
            if (sLeftBlack && sRightBlack) {
                if (s) s->color = RED;
                x = parent;
                parent = x->parent;
            }
            else {
                // ----- Case 4 đối xứng: sibling đen, "con gần" đỏ, "con xa" đen -----
                // x là con phải => con gần là s->right, con xa là s->left
                if (s && sLeftBlack && s->right && s->right->color == RED) {
                    s->right->color = BLACK;
                    s->color = RED;
                    rotateLeft(s);
                    s = parent->left;
                }

                // ----- Case 5 đối xứng: sibling đen, "con xa" đỏ -----
                if (s) {
                    s->color = parent->color;
                    parent->color = BLACK;
                    if (s->left) s->left->color = BLACK;
                }
                rotateRight(parent);
                break;
            }
        }
    }
    if (x) x->color = BLACK;
}





// =====================================


// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const {
    if (this->root == nullptr) {
        cout << "NULL\n";
        return;
    }
    
    queue<RBTNode*> q;
    q.push(root);
    RBTNode* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    
    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode*> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()) {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++) {
            RBTNode* node = heightQ.front();
            heightQ.pop();
            if (node->left) heightQ.push(node->left);
            if (node->right) heightQ.push(node->right);
        }
    }
    
    int space = pow(2, height);
    printNSpace(space / 2);
    
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        
        if (temp == nullptr) {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        } else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            } else {
                cout << "B)";
            }
            
            q.push(temp->left);
            q.push(temp->right);
        }
        
        printNSpace(space);
        count++;
        
        if (count == maxNode) {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        
        if (level == height) {
            return;
        }
    }
}

//TODO: Implement all other RedBlackTree<K, T> methods here


template<class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K& key, const T& value) : key(key), data(value), color(RED), 
left(nullptr), right(nullptr), parent(nullptr) {}

template<class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed() {
    this->color = RED;
}

template<class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack() {
    this->color = BLACK;
}



template<class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode* node) {
    if (!node) return;
    RBTNode* rightChild = node->right;
    if (!rightChild) return; // không xoay nếu không có con phải

    // chuyển y->left thành con phải của node
    node->right = rightChild->left;
    if (rightChild->left) rightChild->left->parent = node;

    // nối y với parent của node
    rightChild->parent = node->parent;
    if (!node->parent) root = rightChild; // node là root
    else if (node == node->parent->left) node->parent->left = rightChild; // node là con trái của parent
    else node->parent->right = rightChild; // node là con phải của parent

    // đặt node thành con trái của y
    rightChild->left = node;
    node->parent = rightChild;
}

template<class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode* node) {
    if (!node) return;
    RBTNode* leftChild = node->left;
    if (!leftChild) return;

    node->left = leftChild->right;
    if (leftChild->right) leftChild->right->parent = node;

    leftChild->parent = node->parent;
    if (!node->parent) root = leftChild;
    else if (node == node->parent->right) node->parent->right = leftChild;
    else node->parent->left = leftChild;

    leftChild->right = node;
    node->parent = leftChild;
}

template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::lowerBoundNode(const K& key) const {
    RBTNode* cur = root;
    while (cur) {
        if (cur->key >= key) return cur ;
        cur = cur->right;
    }
    return nullptr;
}

template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::upperBoundNode(const K& key) const {
    RBTNode* cur = root;
    while (cur) {
        if (cur->key > key) return cur;
        cur = cur->right;
    }
    return nullptr;
}




template<class K, class T>
RedBlackTree<K, T>::RedBlackTree() : root(nullptr) {}

template<class K, class T>
RedBlackTree<K, T>::~RedBlackTree() {
    clear();
}


template<class K, class T>
bool RedBlackTree<K, T>::empty() const {
    return root == nullptr;
}

template<class K, class T>
int RedBlackTree<K, T>::size() const {
    return sizeRec(root);
}

template<class K, class T>
void RedBlackTree<K, T>::clear() {
    clearRec(root);
    root = nullptr;
}

template<class K, class T>
void RedBlackTree<K, T>::insert(const K& key, const T& value) {
    // cây rỗng -> tạo root -> tô đen
    if (!root) {
        root = new RBTNode(key, value);
        root->color = BLACK;
        return;
    }

    // tìm vị trí chèn tương tự BST
    RBTNode* cur = root;
    RBTNode* parent = nullptr;
    while (cur) {
        parent = cur;
        if (cur->key > key) cur = cur->left;
        else if (cur->key < key) cur = cur->right;
        else return;
    }

    // tạo node mới màu đỏ
    RBTNode* newNode = new RBTNode(key, value);
    newNode->parent = parent;

    if (key < parent->key) parent->left = newNode;
    else parent->right = newNode;

    // Sửa màu + xoay nếu cần
    fixInsert(newNode);
}

template<class K, class T>
void RedBlackTree<K, T>::remove(const K& key) {
    RBTNode* z = find(key);
    if (!z) return;

    RBTNode* y = z; // y là node thực sự sẽ rời khỏi cây
    Color yOriginalColor = y->color;

    // Nếu z có con trái -> dùng node lớn nhất bên trái
    if (z->left != nullptr) {
        RBTNode* pred = maximumNode(z->left); // largest in left subtree

        // copy dữ liệu của pred vào z
        z->key = pred->key;
        z->data = pred->data;

        // xóa pred thay cho z
        y = pred;
        yOriginalColor = y->color;
    }

    // y có tối đa 1 con
    RBTNode* child = (y->left != nullptr) ? y->left : y->right;
    RBTNode* x = child;
    RBTNode* xParent = y->parent;

    if (child) child->parent = y->parent;

    if (!y->parent) { // y là root
        root = child;
    }
    else if (y == y->parent->left) y->parent->left = child;
    else y->parent->right = child;

    delete y;

    if (yOriginalColor == BLACK) fixDelete(x, xParent);
}

template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::find(const K& key) const {
    RBTNode* cur = root;
    while (cur) {
        if (key < cur->key) cur = cur->left;
        else if (key > cur->key) cur = cur->right;
        else return cur;
    }
    return nullptr;
}

template<class K,class T>
bool RedBlackTree<K, T>::contains(const K& key) const {
    return find(key) != nullptr;
}



template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::lowerBound(const K& key, bool& found) const {
    RBTNode* cur = root;
    RBTNode* ans = nullptr;
    while (cur) {
        if (cur->key >= key) {
            ans = cur;
            cur = cur->left;
        }
        else cur = cur->right;
    }
    found = ans != nullptr;
    return ans;
}

template<class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::upperBound(const K& key, bool& found) const {
    RBTNode* cur = root;
    RBTNode* ans = nullptr;
    while (cur) {
        if (cur->key > key) {
            ans = cur;
            cur = cur->left;
        }
        else cur = cur->right;
    }
    found = ans != nullptr;
    return ans;
}





// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream& operator<<(std::ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// Helper functions forVectorStore

static void freeEmbeddingVector(const VectorRecord& rec) {
    if (rec.vector != nullptr) {
        delete rec.vector;
        // không gán nullptr vì rec là bản copy; node sẽ bị xoá ngay sau khi clear()
    }
}


// Helper: tìm node thứ targetIndex (0-based) khi duyệt inorder

static AVLTree<double, VectorRecord>::AVLNode*
getNodeByInorderIndex(AVLTree<double, VectorRecord>::AVLNode* node, int targetIndex, int& currentIndex) {

    if (!node) return nullptr;

    // Duyệt trái
    // auto ở đây là kiểu dữ liệu trả về, cụ thể là AVLTree<double, VectorRecord>::AVLNode*
    auto leftNode = getNodeByInorderIndex(node->pLeft, targetIndex, currentIndex); 
    if (leftNode) return leftNode;

    if (targetIndex == currentIndex) return node;

    ++currentIndex;

    // Duyệt phải
    auto rightNode=getNodeByInorderIndex(node->pRight, targetIndex, currentIndex);
    return rightNode;
}




// =====================================







// =====================================
// VectorStore implementation
// =====================================

//TODO: Implement all VectorStore methods here

VectorStore::VectorStore(int dimension, vector<float>* (*embeddingFunction)(const string&), const vector<float>& referenceVector)
    : vectorStore(new AVLTree<double, VectorRecord>()), normIndex(new RedBlackTree<double, VectorRecord>()),
    referenceVector(new vector<float>(referenceVector)), rootVector(nullptr), dimension(dimension), count(0), 
    averageDistance(0.0), embeddingFunction(embeddingFunction), nextId(1) {
}

VectorStore::~VectorStore() {
    clear();

    delete vectorStore;
    delete normIndex;
    delete referenceVector;
}

int VectorStore::size() {
    return count;
}

bool VectorStore::empty() {
    return count == 0;
}

void VectorStore::clear() {
    // 1. Giải phóng mọi vector<float>* được cấp phát cho các bản ghi
    //    Duyệt in-order trên cây AVL (phần 3.1) để gọi freeEmbeddingVector cho từng record.

    if (vectorStore && !vectorStore->empty()) {
        vectorStore->inorderTraversal(freeEmbeddingVector);
    }

    // 2. Xoá toàn bộ node trên 2 cây, dùng lại clear() của AVLTree & RedBlackTree (3.1, 3.2)
    if (vectorStore) vectorStore->clear();
    if (normIndex) normIndex->clear();

    // 3. Reset trạng thái thống kê, nhưng KHÔNG đụng referenceVector
    if (rootVector) {
        delete rootVector;
        rootVector = nullptr;
    }
    count = 0;
    averageDistance = 0.0;
    nextId = 1;
}

vector<float>* VectorStore::preprocessing(string rawText) {
    if (!embeddingFunction) return nullptr;

    vector<float>* vec = embeddingFunction(rawText);
    if (!vec) return nullptr;

    int curDim = static_cast<int>(vec->size());

    if (curDim > dimension) vec->resize(dimension);
    else if (curDim < dimension) vec->resize(dimension, 0.0f);

    return vec;
}

void VectorStore::addText(string rawText) {
    // 1. Preprocess: text -> embedding vector (đã chuẩn hóa kích thước)
    vector<float>* vec = preprocessing(rawText);
    if (!vec) return;

    bool wasEmpty = (count == 0);

    // 2. Tính khoảng cách L2 từ referenceVector tới vec
    double dist = l2Distance(*referenceVector, *vec );

    // 3. Cập nhật averageDistance (dùng count cũ)
    if (wasEmpty) averageDistance = dist;
    else averageDistance = (averageDistance * count + dist) / (static_cast<double>(count + 1));

    // 4. Tính Euclidean norm của chính vector
    double norm = computeNorm(vec);

    // 5. Tạo VectorRecord mới – id theo quy tắc “maxId + 1”
    int newId = nextId++;
    VectorRecord record(newId, rawText, vec, dist);

    // 6. Chèn vào AVL và Red-Black tree
    //    - AVL: key = distanceFromReference
    //    - RBT: key = Euclidean norm
    vectorStore->insert(dist, record);
    normIndex->insert(norm, record);

    // 7. Tăng count
    ++count;
   
    // 8. Xử lý root vector
    if (wasEmpty) {
        if (rootVector != nullptr) delete rootVector;
        rootVector = new VectorRecord(record);
    }
    else { 
        // Sai số root hiện tại so với averageDistance
        double oldDiff = 0.0;
        if (rootVector) {
            oldDiff = fabs(rootVector->distanceFromReference - averageDistance);
        }
        // Sai số của vector mới so với averageDistance
        double newDiff = fabs(dist - averageDistance);

        if (newDiff < oldDiff) {
            rebuildTreeWithNewRoot(&record);
        }
        else { 
            // Không rebuild toàn bộ, chỉ cập nhật rootVector logic nếu cần
            // có trường hợp vector mới không phải tốt nhất, root cũ không còn tốt nhất, mà 1 vector khác trong kho trở thành “gần average nhất”.
            rebuildRootIfNeeded(); 
        }
    }
}

VectorRecord* VectorStore::getVector(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");

    if (!vectorStore || vectorStore->empty()) throw out_of_range("Index is invalid!");
    
    int currentIndex = 0;
    auto rootNode = vectorStore->getRoot();

    auto targetNode = getNodeByInorderIndex(rootNode, index, currentIndex);
    if (!targetNode) throw out_of_range("Index is invalid!");

    return &(targetNode->data);
}

string VectorStore::getRawText(int index) {
    VectorRecord* rec = getVector(index);
    return rec->rawText;
}

int VectorStore::getId(int index) {
    VectorRecord* rec = getVector(index);
    return rec->id;
}


bool VectorStore::removeAt(int index) {
    // 1. Kiểm tra index hợp lệ
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    if (!vectorStore || vectorStore->empty()) throw out_of_range("Index is invalid!");

    // 2. Tìm node theo in-order index
    int currentIndex = 0;
    auto* rootNode = vectorStore->getRoot();
    auto* targetNode = getNodeByInorderIndex(rootNode, index, currentIndex);

    if (!targetNode) throw out_of_range("Index is invalid!");

    // 3. Copy record ra để dùng sau khi xóa node
    VectorRecord rec = targetNode->data;

    // 4. Tính norm để xóa trong Red-Black tree 
    double norm = computeNorm(rec.vector);

    int oldCount = count;

    
    // 5. Xóa khỏi AVL (key = distanceFromReference)
    vectorStore->remove(rec.distanceFromReference);

    // 6. Xóa khỏi Red-Black tree (key = norm)
    if (normIndex) normIndex->remove(norm);

    // 7. Giải phóng bộ nhớ vector<float>*
    if (rec.vector) {
        delete rec.vector;
        rec.vector = nullptr;
    }

    // 8. Cập nhật count, averageDistance và xác định đúng nextId
    if (oldCount <= 1) {
        count = 0;
        averageDistance = 0.0;

        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        return true;
    }
    // oldCount >= 2
    count = oldCount - 1;
    averageDistance = (averageDistance * oldCount - rec.distanceFromReference) / (static_cast<double>(count));

    if (rec.id == nextId - 1) nextId = nextId - 1;
    // 9. Nếu cây vẫn còn phần tử -> xây lại root vector
    rebuildRootIfNeeded();
    return true;
}










VectorRecord* VectorStore::findVectorNearestToDistance(double targetDistance) const {
    if (!vectorStore || vectorStore->empty()) return nullptr;

    auto* node = vectorStore->getRoot();
    if (!node) return nullptr;

    AVLTree<double, VectorRecord>::AVLNode* bestNode = nullptr;
    double bestDiff = INFINITY;

    auto* cur = node;
    while (cur) {
        double diff = abs(cur->key - targetDistance);
        if (diff < bestDiff) {
            bestDiff = diff;
            bestNode = cur;
        }

        if (targetDistance < cur->key) cur = cur->pLeft;
        else if (targetDistance > cur->key) cur = cur->pRight;
        else break; // khoảng cách trùng khớp -> tốt nhất rồi
    }

    if (!bestNode) return nullptr;
    return &(bestNode->data);
}

void VectorStore::rebuildRootIfNeeded() {
    if (!vectorStore || vectorStore->empty() || count == 0) {
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        return;
    }

    VectorRecord* nearest = findVectorNearestToDistance(averageDistance);
    if (!nearest) return;

    if (!rootVector || rootVector->id != nearest->id) {
        delete rootVector;
        rootVector = new VectorRecord(*nearest);
    }
}

void VectorStore::rebuildTreeWithNewRoot(VectorRecord* newRoot) {
    bool hasNewRoot = false;
    VectorRecord newRootCopy;
    if (newRoot) {
        newRootCopy = *newRoot; // copy toàn bộ record (con trỏ vector bên trong được copy lại)
        hasNewRoot = true;
    }

    // Nếu kho rỗng hoặc cây trống: chỉ cần reset rootVector
    if (!vectorStore || vectorStore->empty() || count == 0) {
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        return;
    }

    // 1. Thu thập toàn bộ VectorRecord trong cây AVL bằng duyệt in-order
    vector<VectorRecord> records;
    records.reserve(count);

    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;
    vector<AVLNode*>st;
    AVLNode* cur = vectorStore->getRoot();
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        // copy dữ liệu record ra ngoài
        records.push_back(cur->data);

        cur = cur->pRight;
    }
    
    if (records.empty()) {
        vectorStore->clear();
        averageDistance = 0.0;
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        count = 0;
        return;
    }
    
    // 2. Tính lại distanceFromReference & averageDistance dựa trên referenceVector hiện tại
    double sumDist = 0.0;

    if (referenceVector) {
        for (VectorRecord& rec : records) {
            if (rec.vector) {
                double dist = l2Distance(*referenceVector, *rec.vector);
                rec.distanceFromReference = dist;
                sumDist += dist;
            }
            else {
                rec.distanceFromReference = 0.0;
            }
        }
        averageDistance = sumDist / static_cast<double>(records.size());
    }
    else { // Không có referenceVector: đặt khoảng cách = 0
        for (VectorRecord& rec : records) rec.distanceFromReference = 0.0;
        averageDistance = 0.0;
    }

    // 3. Xây lại cây AVL với key mới = distanceFromReference
    vectorStore->clear();
    for (const VectorRecord& rec : records) {
        vectorStore->insert(rec.distanceFromReference, rec);
    }

    // 4. Cập nhật rootVector:
    //    - Nếu caller truyền newRoot thì dùng nó
    //    - Nếu không, chọn lại root theo averageDistance (hàm đang có sẵn)
    if (hasNewRoot) {
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        rootVector = new VectorRecord(newRootCopy);
    }
    else rebuildRootIfNeeded();
}

void VectorStore::setReferenceVector(const vector<float>& newReference) {
    // 1. Cập nhật referenceVector (chuẩn hoá dimension)
    if (!referenceVector) referenceVector = new vector<float>();

    *referenceVector = newReference;

    int curDim = static_cast<int>(referenceVector->size());
    if (curDim > dimension) referenceVector->resize(dimension);
    else if (curDim < dimension) referenceVector->resize(dimension, 0.0f);

    // 2. Nếu store đang rỗng thì chỉ cần reset thống kê & rootVector
    if (!vectorStore || vectorStore->empty() || count == 0) {
        averageDistance = 0.0;
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        return;
    }

    // 3. Thu thập toàn bộ VectorRecord trong AVL bằng duyệt in-order (iterative)
    vector<VectorRecord> records;
    records.reserve(count);

    vector<AVLTree<double, VectorRecord>::AVLNode*>st;
    auto* cur = vectorStore->getRoot();

    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur=cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        // lấy bản ghi (copy)
        records.push_back(cur->data);
        cur = cur->pRight;
    }

    if (records.empty()) {
        vectorStore->clear();
        averageDistance = 0.0;
        if (rootVector) {
            delete rootVector;
            rootVector = nullptr;
        }
        count = 0;
        return;
    }

    // 4. Tính lại distanceFromReference & averageDistance với reference mới
    double sumDist = 0.0;
    for (auto& rec : records) {
        if (rec.vector) {
            double dist = l2Distance(*referenceVector, *rec.vector);
            rec.distanceFromReference = dist;
            sumDist += dist;
        }
        else rec.distanceFromReference = 0.0;
    }

    averageDistance = sumDist / static_cast<double>(records.size());

    // 5. Rebuild lại cây AVL với key mới = distanceFromReference
    vectorStore->clear();
    for (const auto& rec : records) vectorStore->insert(rec.distanceFromReference, rec);

    // 6. Cập nhật distanceFromReference bên RBT (normIndex) cho đồng nhất
    if (normIndex && !normIndex->empty()) {
        vector<RedBlackTree<double, VectorRecord>::RBTNode*> stk;
        RedBlackTree<double, VectorRecord>::RBTNode* node = normIndex->root; // VectorStore là friend nên truy cập được
        while (node || !stk.empty()) {
            while (node) {
                stk.push_back(node);
                node = node->left;
            }
            node = stk.back();
            stk.pop_back();

            VectorRecord& rec = node->data;
            if (rec.vector) {
                double Dist = l2Distance(*referenceVector, *rec.vector);
                rec.distanceFromReference = Dist;
            }
            else rec.distanceFromReference = 0.0;

            node = node->right;
        }
    }

    // 7. Chọn lại rootVector dựa trên averageDistance (dùng hàm có sẵn)
    rebuildRootIfNeeded();
}

vector<float>* VectorStore::getReferenceVector() const{
    return referenceVector;
}

VectorRecord* VectorStore::getRootVector()const {
    return rootVector;
}

double VectorStore::getAverageDistance()const {
    return averageDistance;
}

void VectorStore::setEmbeddingFunction(vector<float>* (*newEmbeddingFunction)(const string&)) {
    embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEach(void(*action) (vector<float>&, int, string&)) {
    if (!action) return;
    if (!vectorStore || vectorStore->empty() || count == 0) return;

    vector <AVLTree<double, VectorRecord>::AVLNode*> st;
    auto* cur = vectorStore->getRoot();

    // Duyệt in-order để bảo đảm thứ tự khoảng cách tăng dần
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        VectorRecord& rec = cur->data;

        if (rec.vector) {
            // gọi action trên (vector, id, rawText)
            action(*(rec.vector), rec.id, rec.rawText);
        }

        cur = cur->pRight;
    }
}

vector<int> VectorStore::getAllIdsSortedByDistance() const {
    vector<int> ids;
    if (!vectorStore || vectorStore->empty() || count == 0) return ids;

    vector < AVLTree<double, VectorRecord>::AVLNode*> st;
    auto* cur = vectorStore->getRoot();
    // In-order traversal: id theo khoảng cách tăng dần
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        const VectorRecord& rec = cur->data;
        ids.push_back(rec.id);

        cur = cur->pRight;
    }
    return ids; // Không sort lại, chỉ duyệt in-order → O(n)
}

vector<VectorRecord*>VectorStore::getAllVectorsSortedByDistance() const {
    vector<VectorRecord*> result;
    if (!vectorStore || vectorStore->empty() || count == 0) return result;

    vector < AVLTree<double, VectorRecord>::AVLNode*> st;
    auto* cur = vectorStore->getRoot();
    // In-order traversal: id theo khoảng cách tăng dần
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        // lưu con trỏ tới bản ghi trong node
        result.push_back(&(cur->data));

        cur = cur->pRight;
    }
    return result; // O(n) phần tử, đã sorted theo distanceFromReference
}


//  Ước lượng ngưỡng D từ k

double VectorStore::estimateD_Linear(const vector<float>& query, int k, double averageDistance, 
    const vector<float>&reference, double c0_bias, double c1_slope) {

    double dr = l2Distance(query, reference);

    double D = fabs(dr - averageDistance) + c1_slope * averageDistance * static_cast<double>(k) + c0_bias;

    if (D <= 0.0) D = c0_bias;

    return D;
}


//  Tìm kiếm láng giềng gần nhấ

int VectorStore::findNearest(const vector<float>& query, string metric) {
    // 1. Kiểm tra metric hợp lệ
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") throw invalid_metric();

    // 2. Nếu kho rỗng thì không có vector nào để so
    if (!vectorStore || vectorStore->empty() || count == 0) return -1;

    vector<AVLTree<double, VectorRecord>::AVLNode*> st;
    auto* cur = vectorStore->getRoot();

    bool first = true;
    int bestId = -1;
    double bestValue = 0.0; // cosine: max; euclidean/manhattan: min

    // 3. Duyệt toàn bộ cây AVL theo thứ tự in-order (O(n))
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        VectorRecord& rec = cur->data;
        if (rec.vector) {
            double val = 0.0;

            if (metric == "cosine") {
                val = cosineSimilarity(query, *rec.vector);
                if (first || val > bestValue) {
                    bestValue = val;
                    bestId = rec.id;
                    first = false;
                }
            }
            else if (metric == "euclidean") {
                val = l2Distance(query, *rec.vector);
                if (first || val < bestValue) {
                    bestValue = val;
                    bestId = rec.id;
                    first = false;
                }
            }
            else {
                val = l1Distance(query, *rec.vector);
                if (first || val < bestValue) {
                    bestValue = val;
                    bestId = rec.id;
                    first = false;
                }
            }
        }
        cur = cur->pRight;
    }
    return bestId;
}

int* VectorStore::topKNearest(const vector<float>& query, int k, string metric) {
    // ===== Kiểm tra tham số =====
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") throw invalid_metric();
    if (k <= 0 || k > count) throw invalid_k_value();
    if (!normIndex || normIndex->empty()) throw invalid_k_value();

    // ===== Bước 1: chuẩn Euclidean của query: nq = ||q|| =====
    double nqSq = 0.0;
    for (int i = 0;i < dimension;++i) {
        double v = static_cast<double>(query[i]);
        nqSq += v * v;
    }
    double nq = sqrt(nqSq);

    // ===== Bước 2: ước lượng bán kính D từ k =====
    double D = estimateD_Linear(query, k, averageDistance, *referenceVector);

    using RBTNode = RedBlackTree<double, VectorRecord>::RBTNode;

    // ===== Bước 3: lọc bằng red-black tree theo norm =====
    double minNorm = nq - D;
    double maxNorm = nq + D;
    if (minNorm < 0.0) minNorm = 0.0;

    bool foundLow = false;
    bool foundHigh = false;
    // auto là RedBlackTree<double, VectorRecord>::RBTNode;
    RBTNode* lowNode = normIndex->lowerBound(minNorm, foundLow); // ≥ minNorm
    RBTNode* highNode = normIndex->upperBound(maxNorm, foundHigh); // > maxNorm

    // Hàm tìm predecessor in-order trong RBT
    /*
    auto = kiểu lambda nội bộ
    Không phải pointer, không phải struct
    */
    auto predecessor = [](RBTNode* node)->RBTNode* {
        if (!node) return nullptr;

        // Nếu có nhánh trái: đi tới node phải nhất của nhánh trái
        if (node->left) {
            RBTNode* cur = node->left;
            while (cur->right) cur = cur->right;
            return cur;
        }
        
        // Nếu không: đi lên cha cho tới khi node là con phải
        RBTNode* cur = node;
        RBTNode* parent = cur->parent;
        while (parent && cur == parent->left) {
            cur = parent;
            parent = parent->parent;
        }
        return parent;
        };

    // Tìm điểm bắt đầu: node có norm ≤ maxNorm và gần maxNorm nhất
    RBTNode* node = nullptr;
    if (highNode) node = predecessor(highNode);
    else {
        // Không có node > maxNorm → lấy node lớn nhất cây
        node = normIndex->root;
        if (node) while (node->right) node = node->right;
    }

    int m = 0;
    priority_queue<pair<double, int>> heap;

    // Duyệt giảm dần norm cho tới khi < minNorm
    while (node) {
        double normVal = node->key; // key của RBT là norm (double)
        if (normVal < minNorm) break;

        VectorRecord& rec = node->data;
        if (rec.vector) {
            double score = 0.0;
            if (metric == "cosine") {
                double cosSim = cosineSimilarity(query, *rec.vector);
                // priority queue là max heap, đang cần tìm min nên đảo lại (cos càng lớn càng nhỏ -> gần)
                // biến bài toán “giữ top - k giá trị nhỏ nhất” → thành “giữ top - k giá trị lớn nhất trong dữ liệu đã đảo dấu”.
                score = 1 - cosSim;
            }
            else if (metric == "euclidean") score = l2Distance(query, *rec.vector);
            else score = l1Distance(query,*rec.vector);


            ++m;

            if ((int)heap.size() < k) {
                heap.emplace(score, rec.id);
            }
            else if (score < heap.top().first) {
                heap.pop();
                heap.emplace(score, rec.id);
            }
        }
        node = predecessor(node);
    }

    // In ra m theo yêu cầu đề
    cout << "Value m: " << m;

    // ===== Bước 5: trích top-k từ heap và sắp xếp theo khoảng cách tăng dần =====
    int actualK = static_cast<int>(heap.size()); // có thể < k nếu m < k
    vector<pair<double, int>> tmp;
    tmp.reserve(actualK);

    while (!heap.empty()) { tmp.push_back(heap.top()); heap.pop(); } 
    
    // Đảo lại để có thứ tự khoảng cách tăng dần (gần nhất trước) 
    for (int i = 0, j = actualK-1; i < j;++i,--j) swap(tmp[i], tmp[j]);

    // Cấp phát mảng động size = k như đề mô tả
    int* result = new int[k];
    for (int i = 0;i < k;++i) result[i] = -1; // mặc định -1 cho các slot không dùng

    for (int i = 0;i < actualK;++i) result[i] = tmp[i].second;

    // Lưu ý: nếu m < k, chỉ actualK phần tử đầu có id hợp lệ, các phần còn lại là -1.
    return result;
}


//  Truy vấn phạm vi

int* VectorStore::rangeQueryFromRoot(double minDist, double maxDist) const {
    // Nếu kho rỗng → trả về mảng [-1] (chắc cũng không xảy ra)
    if (!vectorStore || vectorStore->empty() || count == 0) {
        int* res = new int[1];
        res[0] = -1;
        return res;
    }
    
    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;

    vector<int>ids;
    ids.reserve(count); // upper bound

    // Range search trên BST theo key = distanceFromReference
    vector<AVLNode*>st;
    AVLNode* cur = vectorStore->getRoot();

    while (cur || !st.empty()) {
        // Đi xuống trái, nhưng bỏ qua nhánh chắc chắn < minDist
        while (cur) {
            if (cur->key >= minDist) {
                st.push_back(cur);
                cur = cur->pLeft;
            }
            else cur = cur->pRight;
        }
        if (st.empty()) break;
        cur = st.back();
        st.pop_back();

        // Nếu key > maxDist thì node này và toàn bộ nhánh phải đều > maxDist
        if (cur->key > maxDist) {
            cur = nullptr;
            continue;
        }

        // Lúc này cur->key >= minDist (do điều kiện khi push) và <= maxDist (vì không return ở trên)
        ids.push_back(cur->data.id);

        // Bên phải chỉ cần đi tiếp nếu còn khả năng <= maxDist
        if (cur->key < maxDist) cur = cur->pRight; // nếu bằng thì cũng không cần đi nữa
        else cur = nullptr;
    }

    // convert to dynamic array
    int* result = new int[ids.size()];
    for (int i = 0;i < ids.size();++i) {
        result[i] = ids[i];
    }
    return result;
}

int* VectorStore::rangeQuery(const vector<float>& query, double radius, string metric) const {
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") throw invalid_metric();
    if (!vectorStore || vectorStore->empty() || count == 0)
        return nullptr;
    
    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;

    vector<int>ids;
    ids.reserve(count);

    AVLNode* cur = vectorStore->getRoot();
    vector<AVLTree<double, VectorRecord>::AVLNode*>st;

    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        VectorRecord& rec = cur->data;
        if (rec.vector) {
            bool ok = false;

            if (metric == "cosine") {
                double cs = cosineSimilarity(query, *rec.vector);
                // bán kính cho similarity: giữ những vector có dist < radius
                if (1-cs < radius) ok = true;
            }
            else if (metric == "euclidean") {
                double d = l2Distance(query, *rec.vector);
                if (d < radius) ok = true;
            }
            else {
                double d = l1Distance(query, *rec.vector);
                if (d < radius) ok = true;
            }
            if (ok) ids.push_back(rec.id);
        }
        cur = cur->pRight;
    }
    
    int* result = new int[ids.size()];
    for (int i = 0;i < ids.size();++i) {
        result[i] = ids[i];
    }
    return result;
}

int* VectorStore::boundingBoxQuery(const vector<float>& minBound, const vector<float>& maxBound) const {
    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;

    vector<int>ids;
    ids.reserve(count);

    vector<AVLNode*>st;
    AVLNode* cur = vectorStore->getRoot();

    // Duyệt toàn bộ cây AVL theo in-order: O(n)
    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        VectorRecord& rec = cur->data;
        if (rec.vector) {
            bool inside = true;

            // Kiểm tra từng chiều trong hộp [minBound, maxBound]
            for (int i = 0;i < dimension;++i) {
                double v = static_cast<double>((*rec.vector)[i]);
                double lo = static_cast<double>(minBound[i]);
                double hi = static_cast<double>(maxBound[i]);

                if (v<=lo || v>=hi) {
                    inside = false;
                    break;
                }
            }
            if (inside) ids.push_back(rec.id);
        }
        cur = cur->pRight;
    }

    // Chuyển vector -> mảng int*
    int* result = new int[ids.size()];
    for (int i = 0;i < ids.size();++i) {
        result[i] = ids[i];
    }
    return result;
}



// KEY không nhỏ lại vì bạn không bao giờ cập nhật key sau khi insert.

double VectorStore::getMaxDistance() const {
    if (!vectorStore || vectorStore->empty() || count == 0) {
        return 0.0;
    }

    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;

    vector<AVLNode*>st;
    AVLNode* cur = vectorStore->getRoot();

    double maxDist = 0.0;
    bool initialized = false;

    while (cur || !st.empty()) {
        while (cur) {
            st.push_back(cur);
            cur = cur->pLeft;
        }
        cur = st.back();
        st.pop_back();

        const VectorRecord& rec = cur->data;
        double d = rec.distanceFromReference;

        if (!initialized) {
            maxDist = d;
            initialized = true;
        }
        else if (d > maxDist) maxDist = d;

        cur = cur->pRight;
    }
    return maxDist;
}

double VectorStore::getMinDistance() const {
    if (!vectorStore || vectorStore->empty() || count == 0) {
        return 0.0;
    }

    using AVLNode = AVLTree<double, VectorRecord>::AVLNode;

    AVLNode* cur = vectorStore->getRoot();

    while (cur->pLeft) cur = cur->pLeft;

    // key == distanceFromReference, nhưng dùng field trong record cho rõ nghĩa
    /* key = dist = khoảng cách từ referenceVector
    data = VectorRecord(có field distanceFromReference lưu cùng giá trị dist) */
    return cur->data.distanceFromReference; // tương đương cur->key
}

VectorRecord VectorStore::computeCentroid(const vector<VectorRecord*>& records) const {
    // Khởi tạo vector centroid với dimension chiều, toàn 0.0f
    vector<float>*centroidVec = new vector<float>(dimension, 0.0f);

    int validCount = 0;

    // Cộng dồn theo từng chiều
    for (VectorRecord* recPtr : records) {
        if (!recPtr || !recPtr->vector) continue; // bỏ qua record không có dữ liệu

        const vector<float>& v = *(recPtr -> vector);
        for (int i = 0;i < dimension;++i) (*centroidVec)[i] += v[i];
        ++validCount;
    }
    // Nếu không có vector hợp lệ nào
    if (validCount == 0) {
        delete centroidVec;
        return VectorRecord();
    }

    // Chia trung bình cho từng chiều
    for (int i = 0; i < dimension;++i) (*centroidVec)[i] /= static_cast<float>(validCount);

    // Tính distanceFromReference cho centroid
    double dist = 0.0;
    if (referenceVector && !referenceVector->empty()) dist = l2Distance(*referenceVector, *centroidVec);

    // Tạo VectorRecord kết quả tạm thời, id = -1, rawText = "centroid"
    VectorRecord centroidRecord(-1, "centroid", centroidVec, dist);
    
    return centroidRecord;
}








// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;



// độ đo khoảng cách và độ chuẩn
double VectorStore::cosineSimilarity(const vector<float>& v1, const vector<float>& v2) const {
    double dot = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;

    for (int i = 0;i < dimension;++i) {
        double a = static_cast<double>(v1[i]);
        double b = static_cast<double>(v2[i]);
        dot += a * b;
        norm1 += a * a;
        norm2 += b * b;
    }

    double denom = sqrt(norm1) * sqrt(norm2);
    if (denom == 0.0) return 0.0; // tránh chia cho 0

    return dot / denom;
}

double VectorStore::l1Distance(const vector<float>& v1, const vector<float>& v2) const {
    double sum = 0.0;
    for (int i = 0;i < dimension;++i) {
        double a = static_cast<double>(v1[i]);
        double b = static_cast<double>(v2[i]);
        sum += std::abs(a - b);
    }
    return sum;
}

double VectorStore::l2Distance(const vector<float>& v1, const vector<float>& v2) const {
    double distSq = 0.0;
    for (int i = 0;i < dimension;++i) {
        double diff = static_cast<double>(v1[i]) - static_cast<double>(v2[i]);
        distSq += diff * diff;
    }
    return sqrt(distSq);
}


// helper tính norm
double VectorStore::computeNorm(const vector<float>* vec) const {
    if (!vec) return 0.0;

    double normSq = 0.0;
    for (int i = 0;i < dimension;++i) {
        double val = static_cast<double>((*vec)[i]);
        normSq += val * val;
    }
    return sqrt(normSq);
}

double VectorStore::distanceByMetric(const vector<float>& a, const vector<float>& b, const string& metric) const {
    if (metric == "cosine") {
        double cosSim = cosineSimilarity(a, b);
        return 1.0 - cosSim;
    }
    else if (metric == "euclidean") return l2Distance(a, b);
    else if (metric == "manhattan") return l1Distance(a, b);
    else throw invalid_metric();
}