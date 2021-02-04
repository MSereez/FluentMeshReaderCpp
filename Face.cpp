#include "Face.h"
#include <cmath>

Face::Face():
 facetype(0),faceNo(0),tagCellNo1(0),tagCellNo2(0),
 x1(0.0),x2(0.0),y1(0.0),y2(0.0),
 xn(0.0), yn(0.0), xun(0.0), yun(0.0),xfc(0.0),yfc(0.0),
 interpolatetoNeiFac(0.0),
 faceLength(0.0),sArea(0.0),absxlength(0.0),absylength(0.0),
 isverticallyDominant(false),ishorizontallyDominant(false),
 isBoundary_(false)
{
   

}

Face::Face(Node& N1, Node& N2)
: Node1(N1), Node2(N2),  
 facetype(0),faceNo(0),tagCellNo1(0),tagCellNo2(0),
 x1(0.0),x2(0.0),y1(0.0),y2(0.0),
 xn(0.0), yn(0.0), xun(0.0), yun(0.0),xfc(0.0),yfc(0.0),
 interpolatetoNeiFac(0.0),
 faceLength(0.0),sArea(0.0),absxlength(0.0),absylength(0.0),
 isverticallyDominant(false),ishorizontallyDominant(false),
  isBoundary_(false)
{

    x1 = N1.x;
    x2 = N2.x;
    y1 = N1.y;
    y2 = N2.y;
    
    absxlength = abs(x2-x1);
    absylength = abs(y2-y1);
    faceLength = sqrt( pow(absxlength,2.0) + pow(absylength,2.0));  
    
          
    xfc=min(x1,x2) + abs((x2-x1)/2.0);
    yfc=min(y1,y2) + abs((y2-y1)/2.0);
    
    xn =absylength;
    yn =absxlength;
    
    xun = xn/(faceLength);
    yun = yn/(faceLength);
    
    sArea = faceLength;
    
    

}



Face::~Face()
{
}