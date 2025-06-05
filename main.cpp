#include <iostream>
#include <vector>
#include <queue>

class Node {
private:
    Node* parent;
    Node* leftChild;
    Node* rightChild;
    int value;
public:
    Node();
    Node(Node* parent, Node* leftChild, Node* rightChild, const int &value);
    Node(const Node& copiedNode);
    Node& operator=(const Node& copiedNode);
    ~Node();

    Node* getParent() const;
    Node* getLeftChild() const;
    Node* getRightChild() const;
    int getValue() const;
    void setParent(Node* newParent);
    void setLeftChild(Node* newLeftChild);
    void setRightChild(Node* newRightChild);
    void setValue(const int& newValue);
};

Node::Node() {
    this->parent = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->value = 0;
}

Node::Node(Node* parent, Node* leftChild, Node* rightChild, const int &value) {
    this->parent = parent;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
    this->value = value;
}

Node::Node(const Node& copiedNode) {
    this->parent = copiedNode.parent;
    this->leftChild = copiedNode.leftChild;
    this->rightChild = copiedNode.rightChild;
    this->value = copiedNode.value;
}

Node &Node::operator=(const Node &copiedNode) = default;

Node::~Node() {
    this->parent = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->value = 0;
}

Node * Node::getParent() const {
    return this->parent;
}

Node * Node::getLeftChild() const {
    return this->leftChild;
}

Node * Node::getRightChild() const {
    return this->rightChild;
}

int Node::getValue() const {
    return this->value;
}

void Node::setParent(Node* newParent) {
    this->parent = newParent;
}

void Node::setLeftChild(Node *newLeftChild) {
    this->leftChild = newLeftChild;
}

void Node::setRightChild(Node *newRightChild) {
    this->rightChild = newRightChild;
}

void Node::setValue(const int &newValue) {
    this->value = newValue;
}


class SegmentTree {
private:
    int nodeCount;
    Node* root;

    Node* buildRecursion(const std::vector<int>& values, const int& left, const int& right);
    int sumRecursion(Node* currentRoot, const int &left, const int &right, const int &leftBoundary, const int &rightBoundary) const;
    void updateRecursion(Node* currentRoot, const int& left, const int& right, const int& position, const int& value) const;
public:
    SegmentTree();
    SegmentTree(const int& newNodeCount, Node* newRoot);
    SegmentTree& operator=(const SegmentTree& copiedSegmentTree);
    SegmentTree(const std::vector<int>& values);
    ~SegmentTree();

    void display() const;
    int sum(const int& leftBoundary, const int& rightBoundary) const;
    void update(const int& position, const int& value) const;
};

Node* SegmentTree::buildRecursion(const std::vector<int>& values, const int& left, const int& right) {
    Node* root = new Node();
    if (left == right) {
        root->setValue(values[left]);
        return root;
    }
    const int mid = (left + right) / 2;
    root->setLeftChild(buildRecursion(values, left, mid));
    root->setRightChild(buildRecursion(values, mid + 1, right));
    root->setValue(root->getLeftChild()->getValue() + root->getRightChild()->getValue());
    return root;
}

int SegmentTree::sumRecursion(Node* currentRoot, const int &left, const int &right, const int &leftBoundary, const int &rightBoundary) const {
    if (currentRoot == nullptr) {
        return 0;
    }
    if (leftBoundary > rightBoundary) {
        return 0;
    }
    if (left == leftBoundary && right == rightBoundary) {
        return currentRoot->getValue();
    }
    const int mid = (left + right) / 2;
    return (this->sumRecursion(currentRoot->getLeftChild(), left, mid, leftBoundary, std::min(rightBoundary, mid))
          + this->sumRecursion(currentRoot->getRightChild(), mid + 1, right, std::max(leftBoundary, mid + 1), rightBoundary));
}

void SegmentTree::updateRecursion(Node *currentRoot, const int &left, const int &right, const int &position, const int &value) const {
    if (currentRoot == nullptr) {
        return;
    }
    if (left == right) {
        currentRoot->setValue(value);
        return;
    }
    const int mid = (left + right) / 2;
    if (position <= mid) {
        this->updateRecursion(currentRoot->getLeftChild(), left, mid, position, value);
    }
    else {
        this->updateRecursion(currentRoot->getRightChild(), mid + 1, right, position, value);
    }
    currentRoot->setValue(currentRoot->getLeftChild()->getValue() + currentRoot->getRightChild()->getValue());
}

SegmentTree::SegmentTree() {
    this->nodeCount = 0;
    this->root = nullptr;
}

SegmentTree::SegmentTree(const int& newNodeCount, Node* newRoot) {
    this->nodeCount = newNodeCount;
    this->root = newRoot;
}

SegmentTree& SegmentTree::operator=(const SegmentTree& copiedSegmentTree) = default;

SegmentTree::SegmentTree(const std::vector<int>& values) {
    if (values.empty()) {
        this->nodeCount = 0;
        this->root = nullptr;
    }
    else {
        this->nodeCount = static_cast<int>(values.size());
        this->root = this->buildRecursion(values, 0, this->nodeCount - 1);
    }
}

SegmentTree::~SegmentTree() {
    this->root = nullptr;
}

void SegmentTree::display() const {
    if (this->root == nullptr) {
        std::cout << "Empty segment tree!" << std::endl;
        return;
    }
    std::cout << "Values in the tree: ";
    std::queue<Node*> nodeQueue;
    nodeQueue.push(this->root);
    while (!nodeQueue.empty()) {
        Node* currentRoot = nodeQueue.front();
        nodeQueue.pop();
        std::cout << currentRoot->getValue() << " ";
        Node* currentLeftChild = currentRoot->getLeftChild();
        if (currentLeftChild != nullptr) {
            nodeQueue.push(currentLeftChild);
        }
        Node* currentRightChild = currentRoot->getRightChild();
        if (currentRightChild != nullptr) {
            nodeQueue.push(currentRightChild);
        }
    }
}

int SegmentTree::sum(const int& leftBoundary, const int& rightBoundary) const {
    if (this->root == nullptr) {
        return 0;
    }
    return this->sumRecursion(this->root, 0, this->nodeCount - 1, leftBoundary, rightBoundary);
}

void SegmentTree::update(const int &position, const int &value) const {
    if (this->root == nullptr) {
        return;
    }
    if (position < 0 || position >= this->nodeCount) {
        return;
    }
    updateRecursion(this->root, 0, this->nodeCount - 1, position, value);
}


int main() {
    // SegmentTree segmentTree = SegmentTree();
    SegmentTree segmentTree = SegmentTree({1, 3, -2, 8, -7});
    segmentTree.display();
    std::cout << std::endl;
    std::cout << segmentTree.sum(1, 4) << std::endl;
    segmentTree.update(2, -5);
    std::cout << segmentTree.sum(1, 4) << std::endl;
    segmentTree.update(1, 1);
    segmentTree.update(3, -3);
    std::cout << segmentTree.sum(0, 3) << std::endl;

    return 0;
}