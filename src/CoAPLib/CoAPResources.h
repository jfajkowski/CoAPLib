#ifndef COAPLIB_COAPRESOURCES_H
#define COAPLIB_COAPRESOURCES_H

#include "Array.hpp"
#include "CoAPConstants.h"
#include "../Environment.h"

struct Node {
    String key;
    unsigned short value;
    Array<Node*> nodes;
};

class CoAPResources {
private:
    void destroy();
    void destroy(Node *leaf);
    void insert(String *begin, const String *end, Node *leaf, unsigned short value);
    Node *search(String *begin, const String *end, Node *leaf);

    Node *root;

public:
    CoAPResources();
    ~CoAPResources();

    void insert(const Array<String> &keys, unsigned short value);
    Node *search(const Array<String> &keys);
};

#endif //COAPLIB_COAPRESOURCES_H
