#include "Cell.h"

Cell::Cell()
{
    
}

Cell::Cell(Node& NodeSW, Node& NodeSE, Node& NodeNE, Node& NodeNW):
Fs(NodeSW,NodeSE),Fe(NodeSE,NodeNE),Fn(NodeNE,NodeNW),Fw(NodeNW,NodeSW),
cellType(0),cellZoneName("FLUID"),
cellFaceList(4),cellNeilist(4)
{
     xsw = NodeSW.x;    xse = NodeSE.x;  xne = NodeNE.x; xnw = NodeNW.x;
     ysw = NodeSW.y;    yse = NodeSE.y;  yne = NodeNE.y; ynw = NodeNW.y;

    double xc1 = (xse-xsw)/2.0;
    double xc2 = (xne-xnw)/2.0;
    xc = Fw.xfc + (xc1+xc2)/2.0;

    double yc1 = (ynw-ysw)/2.0;
    double yc2 = (yne-yse)/2.0;
    yc= Fs.yfc + (yc1+yc2)/2.0;
    
   dToEastFace = abs( Fe.xfc - xc);
   dToNorthFace = abs( Fn.yfc - yc);
   
   dToWestFace = abs( xc - Fw.xfc);
   dToSouthFace =  abs( yc - Fs.yfc);
   
   absxlength = abs( Fe.xfc - Fw.xfc);
   absylength =  abs( Fn.yfc - Fs.yfc);
   cellVolume = (absylength * absxlength);
   
   cellFaceList[0] = Fs;
   cellFaceList[1] = Fe;
   cellFaceList[2] = Fn;
   cellFaceList[3] = Fw;
}


Cell::Cell(Face& SF, Face& EF, Face& NF, Face& WF):
Fs(SF),Fe(EF),Fn(NF),Fw(WF),
cellType(0),cellZoneName("FLUID"),
cellFaceList(4),cellNeilist(4)
{
     xsw = NodeSW.x;    xse = NodeSE.x;  xne = NodeNE.x; xnw = NodeNW.x;
     ysw = NodeSW.y;    yse = NodeSE.y;  yne = NodeNE.y; ynw = NodeNW.y;
    
    double xc1 = (xse-xsw)/2.0;
    double xc2 = (xne-xnw)/2.0;
    xc = Fw.xfc + (xc1+xc2)/2.0;

    double yc1 = (ynw-ysw)/2.0;
    double yc2 = (yne-yse)/2.0;
    yc= Fs.yfc + (yc1+yc2)/2.0;
    
   dToEastFace = abs( Fe.xfc - xc);
   dToNorthFace = abs( Fn.yfc - yc);
   
   dToWestFace = abs( xc - Fw.xfc);
   dToSouthFace =  abs( yc - Fs.yfc);
   
   absxlength = abs( Fe.xfc - Fw.xfc);
   absylength =  abs( Fn.yfc - Fs.yfc);
   cellVolume = (absylength * absxlength);
   
   cellFaceList[0] = Fs;
   cellFaceList[1] = Fe;
   cellFaceList[2] = Fn;
   cellFaceList[3] = Fw;
}

Cell::Cell(Face& SF, Face& EF, Face& NF, Face& WF, Node& SW, Node& SE, Node& NE, Node& NW):
Fs(SF),Fe(EF),Fn(NF),Fw(WF), NodeSW(SW), NodeSE(SE),NodeNE(NE),NodeNW(NW),
cellType(0),cellZoneName("FLUID"),
cellFaceList(4),cellNeilist(4)
{
     xsw = NodeSW.x;    xse = NodeSE.x;  xne = NodeNE.x; xnw = NodeNW.x;
     ysw = NodeSW.y;    yse = NodeSE.y;  yne = NodeNE.y; ynw = NodeNW.y;
    

    
    double xc1 = (xse-xsw)/2.0;
    double xc2 = (xne-xnw)/2.0;
    xc = Fw.xfc + (xc1+xc2)/2.0;

    double yc1 = (ynw-ysw)/2.0;
    double yc2 = (yne-yse)/2.0;
    yc= Fs.yfc + (yc1+yc2)/2.0;
    
   dToEastFace = abs( Fe.xfc - xc);
   dToNorthFace = abs( Fn.yfc - yc);
   
   dToWestFace = abs( xc - Fw.xfc);
   dToSouthFace =  abs( yc - Fs.yfc);
   
   absxlength = abs( Fe.xfc - Fw.xfc);
   absylength =  abs( Fn.yfc - Fs.yfc);
   cellVolume = (absylength * absxlength);
   
   cellFaceList[0] = Fs;
   cellFaceList[1] = Fe;
   cellFaceList[2] = Fn;
   cellFaceList[3] = Fw;
}
Cell::~Cell()
{
}
