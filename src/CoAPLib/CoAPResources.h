#ifndef COAPLIB_COAPRESOURCES_H
#define COAPLIB_COAPRESOURCES_H

#include "Array.hpp"
#include "CoAPConstants.h"
#include "../Environment.h"

/**
 * Represents single node in resource tree
 */
class Node {
private:
    String key;
    unsigned short* value;
    Array<Node*> nodes;

public:
    Node(const String &key);
    ~Node();

    const String &getKey() const;
    unsigned short *getValue() const;
    Array<Node *> &getNodes();

    void setValue(unsigned short *value);
};

/**
 * Resource tree used to describe resources available from server
 */
class CoAPResources {
private:
    Node *root;

    void destroy();
    void destroy(Node *leaf);
    void insert(String *begin, const String *end, Node *leaf, unsigned short *value);
    Node *search(String *begin, const String *end, Node *leaf);
    void getUriPaths(String &result, Node *child) const;
public:
    CoAPResources();
    ~CoAPResources();

    void insert(const Array<String> &keys, unsigned short *value);
    Node *search(const Array<String> &keys);

    String toLinkFormat() const;
};

#endif //COAPLIB_COAPRESOURCES_H
