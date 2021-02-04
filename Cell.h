#ifndef CELL_H
#define CELL_H

#include "Face.h"
class Cell
{
public:
    Cell();
    Cell(Node&, Node&, Node&, Node&);
    Cell(Face&, Face&, Face&, Face&);
    Cell(Face&, Face&, Face&, Face&, Node&, Node&, Node&, Node&);
    virtual ~Cell();
    Face Fs,Fe,Fn,Fw;
    Node NodeSW,NodeSE,NodeNE,NodeNW;
 
    double xsw,xse,xne,xnw;
    double ysw,yse,yne,ynw;
    double xc,yc;
    double dToEastFace,dToNorthFace,dToSouthFace,dToWestFace;
    double absxlength,absylength;
    double cellVolume;
    
    unsigned int cellIndex,cellEastNeiIndex,cellWestNeiIndex,cellNorthNeiIndex,cellSouthNeiIndex;
    long double distToEastNeighbourCellCentre,distToWestNeighbourCellCentre,distToNorthNeighbourCellCentre,distToSouthNeighbourCellCentre;
    long double distToBoundaryFace;
    long double lengthofBoundaryFace;
    long double iFactorEastNei, iFactorNorthNei,iFactorBoundaryFace;
    
    unsigned int cellType,cellBNei, cellBNeiNei;
    // return an index to cell boundary face nei if cell is a boundary cell
    unsigned int cellBFace;
    unsigned int cellBFaceNei;
    std::string cellZoneName;
    vector<Face> cellFaceList;
    vector<unsigned int> nodenum,facenum;
    vector<unsigned int> cellNeilist;

private:


};

#endif // CELL_H