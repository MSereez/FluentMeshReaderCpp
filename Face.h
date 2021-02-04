#ifndef FACE_H
#define FACE_H
#include "Node.h"
#include <string>
#include <vector>
using namespace std;

class Face
{
public:
    Face();
    
    Face(Node&, Node&);
    virtual ~Face();

    Node Node1;
    Node Node2;
    
    unsigned int facetype;
    unsigned int faceNo;
    unsigned int tagCellNo1;
    unsigned int tagCellNo2;
    unsigned int faceNei,faceLhsNei;
    float x1,x2,y1,y2;
    float xn, yn;   // x and y normal
    float xun, yun;   // x and y unit normal
    float xfc,yfc;
    float interpolatetoNeiFac;
    float faceLength;
    float sArea;
    float absxlength,absylength;
    bool isverticallyDominant;
    bool ishorizontallyDominant;
    bool isBoundary_;
    
    
    string faceZoneName;
    
};



#endif // FACE_H