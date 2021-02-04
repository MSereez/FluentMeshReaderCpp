#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node();
    Node(double&, double&);
    virtual ~Node();
    
    float x,y;
    int nodetype, number;
};

#endif // NODE_H