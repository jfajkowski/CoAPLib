#ifndef COAPLIB_COAPRESOURCES_H
#define COAPLIB_COAPRESOURCES_H

#include "Array.hpp"
#include "../Environment.h"

struct Node {
    String key;
    String value;
    Array<Node*> nodes;
};

class CoAPResources {
private:
    void destroy();
    void destroy(Node *leaf);
    void insert(String *begin, const String *end, Node *leaf, const String &value);
    Node *search(String *begin, const String *end, Node *leaf);

    Node *root;

public:
    CoAPResources();
    ~CoAPResources();

    void insert(const Array<String> &keys, const String &value);
    Node *search(const Array<String> &keys);
};

#endif //COAPLIB_COAPRESOURCES_H
