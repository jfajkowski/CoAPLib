#include "CoAPResources.h"

Node::Node(const String &key) : key(key) {}

const String &Node::getKey() const {
    return key;
}

unsigned short *Node::getValue() const {
    return value;
}

void Node::setValue(unsigned short *value) {
    Node::value = value;
}

Array<Node *> &Node::getNodes() {
    return nodes;
}

Node::~Node() {
    if (value != nullptr)
        delete value;
}

CoAPResources::CoAPResources() {
    root = new Node(".");

    Array<String> coreResource;
    coreResource.pushBack(RESOURCE_WELL_KNOWN);
    insert(coreResource, 0);
    coreResource.pushBack(RESOURCE_CORE);
    insert(coreResource, 0);

    Array<String> remoteResource;
    remoteResource.pushBack(RESOURCE_REMOTE);
    insert(coreResource, 0);

    Array<String> localResource;
    localResource.pushBack(RESOURCE_LOCAL);
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
        for (int i = 0; i < leaf->getNodes().size(); ++i) {
            destroy(leaf->getNodes()[i]);
        }
        delete leaf;
    }
}

void CoAPResources::insert(String *begin, const String *end, Node *leaf, unsigned short *value) {
    Node* node = nullptr;
    for (int i = 0; i < leaf->getNodes().size(); ++i) {
        if (leaf->getNodes()[i]->getKey() == *begin) {
            node = leaf->getNodes()[i];
            break;
        }
    }

    if (node == nullptr) {
        node = new Node(*begin);
        leaf->getNodes().pushBack(node);

        DEBUG_PRINT("Created node: ");
        DEBUG_PRINTLN(node->getKey());
    }

    if (begin + 1 != end) {
        DEBUG_PRINT("Moving into: ");
        DEBUG_PRINTLN(node->getKey());
        insert(++begin, end, node, value);
    }
    else
        node->setValue(value);
}

Node *CoAPResources::search(String *begin, const String *end, Node *leaf) {
    Node* node = nullptr;

    DEBUG_PRINT("Looking for: ");
    DEBUG_PRINTLN(*begin);

    if(leaf != nullptr) {
        for (int i = 0; i < leaf->getNodes().size(); ++i) {
            DEBUG_PRINT("Is ");
            DEBUG_PRINT(leaf->getNodes()[i]->getKey());
            DEBUG_PRINT(" a match? ");

            if (leaf->getNodes()[i]->getKey() == *begin) {
                DEBUG_PRINTLN("Yes!");
                node = leaf->getNodes()[i];
                break;
            }

            DEBUG_PRINTLN("No!");
        }


        if (node != nullptr) {
            DEBUG_PRINT("Found node: ");
            DEBUG_PRINTLN(node->getKey());
        }
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

void CoAPResources::insert(const Array<String> &keys, unsigned short *value) {
    insert(keys.begin(), keys.end(), root, value);
}

Node *CoAPResources::search(const Array<String> &keys) {
    return search(keys.begin(), keys.end(), root);
}

String CoAPResources::toLinkFormat() const {
    String core_format;

    core_format += "<";
    for (int i = 1; i < root->getNodes().size(); ++i) {
        getUriPaths(core_format, root->getNodes()[i]);
    }

    return core_format.substr(0, core_format.length() - 2);
}

void CoAPResources::getUriPaths(String &result, Node *child) const {
    Node* node = nullptr;

    for (int i = 0; i < child->getNodes().size(); ++i) {
        result += "/";
        result += child->getKey();
        node = child->getNodes()[i];
        getUriPaths(result, node);
    }

    if (node == nullptr) {
        result += "/";
        result += child->getKey();
        result += ">";
        if (child->getValue() != nullptr) {
            result += ";value=";
            result += TO_STRING(*child->getValue());
        }
        result += ",<";
    }
}
