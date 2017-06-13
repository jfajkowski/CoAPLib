#include "CoAPResources.h"

CoAPResources::CoAPResources() {
    root = new Node;
    root->key = ".";
}

CoAPResources::~CoAPResources() {
    destroy();
    delete root;
}

void CoAPResources::destroy() {
    destroy(root);

    root = new Node;
    root->key = ".";
}

void CoAPResources::destroy(Node *leaf) {
    if(leaf != nullptr) {
        for (int i = 0; i < leaf->nodes.size(); ++i) {
            destroy(leaf->nodes[i]);
        }
        delete leaf;
    }
}

void CoAPResources::insert(String *begin, const String *end, Node *leaf) {
    Node* node = nullptr;
    for (int i = 0; i < leaf->nodes.size(); ++i) {
        if (leaf->nodes[i]->key == *begin) {
            node = leaf->nodes[i];
            break;
        }
    }

    if (node == nullptr) {
        node = new Node;
        node->key = *begin;
        leaf->nodes.pushBack(node);
    }

    if (begin + 1 != end) {
        insert(++begin, end, node);
    }
}

Node *CoAPResources::search(String *begin, const String *end, Node *leaf) {
    Node* node = nullptr;

    if(leaf != nullptr) {
        for (int i = 0; i < leaf->nodes.size(); ++i) {
            if (leaf->nodes[i]->key == *begin) {
                node = leaf->nodes[i];
                break;
            }
        }
    }
    else return nullptr;

    if (begin + 1 != end) {
        return search(++begin, end, node);
    }
    else return node;
}

void CoAPResources::insert(const Array<String> &keys) {
    insert(keys.begin(), keys.end(), root);
}

Node *CoAPResources::search(const Array<String> &keys) {
    return search(keys.begin(), keys.end(), root);
}