#include "CoAPResources.h"

CoAPResources::CoAPResources() {
    root = new Node;
    root->key = ".";

    Array<String> coreResource;
    coreResource.pushBack(RESOURCE_WELL_KNOWN);
    insert(coreResource, 0);
    coreResource.pushBack(RESOURCE_CORE);
    insert(coreResource, 0);
}

CoAPResources::~CoAPResources() {
    destroy();
}

void CoAPResources::destroy() {
    destroy(root);
}

void CoAPResources::destroy(Node *leaf) {
    if(leaf != nullptr) {
        for (int i = 0; i < leaf->nodes.size(); ++i) {
            destroy(leaf->nodes[i]);
        }
        delete leaf;
    }
}

void CoAPResources::insert(String *begin, const String *end, Node *leaf, unsigned short value) {
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

        DEBUG_PRINT("Created node: ");
        DEBUG_PRINTLN(node->key);
    }

    if (begin + 1 != end) {
        DEBUG_PRINT("Moving into: ");
        DEBUG_PRINTLN(node->key);
        insert(++begin, end, node, value);
    }
    else
        node->value=value;
}

Node *CoAPResources::search(String *begin, const String *end, Node *leaf) {
    Node* node = nullptr;

    DEBUG_PRINT("Looking for: ");
    DEBUG_PRINTLN(*begin);

    if(leaf != nullptr) {
        for (int i = 0; i < leaf->nodes.size(); ++i) {
            DEBUG_PRINT("Is ");
            DEBUG_PRINT(leaf->nodes[i]->key);
            DEBUG_PRINT(" a match? ");

            if (leaf->nodes[i]->key == *begin) {
                DEBUG_PRINTLN("Yes!");
                node = leaf->nodes[i];
                break;
            }

            DEBUG_PRINTLN("No!");
        }

        DEBUG_FUNCTION(
            if (node != nullptr) {
                DEBUG_PRINT("Found node: ");
                DEBUG_PRINTLN(node->key);
            }
        )
    }
    else {
        DEBUG_PRINT("Not found: ");
        DEBUG_PRINTLN(*begin);

        return nullptr;
    }

    if (begin + 1 != end) {
        return search(++begin, end, node);
    }
    else return node;
}

void CoAPResources::insert(const Array<String> &keys, const unsigned short value) {
    insert(keys.begin(), keys.end(), root, value);
}

Node *CoAPResources::search(const Array<String> &keys) {
    return search(keys.begin(), keys.end(), root);
}
unsigned short CoAPResources::valueAtPath(const Array<String> &keys){
    return search(keys.begin(), keys.end(), root)->value;
}