#ifndef SORTMESH_H
#define SORTMESH_H
#pragma once
#include "fluent2dRead.h"
#include "Cell.h"
#include "forAllOperations.h"
class unsortedMesh
{
        
private: 
     fluent2DRead *fluentMeshReader;
     typedef vector<Node> nodelist;
     typedef vector<Face> facelist;
     typedef vector<Cell> celllist;
     

    
     void createNodes();
     void createFaces();
     void findcellsfromfaces();


     
     void clearMemory();

     
public:
    unsortedMesh(fluent2DRead&);
    virtual ~unsortedMesh();
    unsigned int totalNodes,totalFaces,totalCells;
    unsigned int totalBoundaryFaces, totalInternalFaces;
    unsigned int totalBoundaryCells, totalInternalCells;
    nodelist nodesofmesh;
    facelist facesofmesh;
    celllist cellsofmesh;
    celllist unsortedcells;
    
    vector<string> wallboundarynames;
    
    facelist boundaryfacesofmesh;
    facelist internalfacesofmesh;


    celllist boundaryCellsofmesh;
    celllist internalCellsofmesh;
    
    
    vector<vector<int> > cellnodenumbers;
    vector<vector<int> > cellfacenumbers;
    unsigned int inlet,outlet, wall,fluid, movingwall;
    
    Node sWcornerNode(const vector<Node>&, const Cell&);
    Node sEcornerNode(const vector<Node>&, const Cell&);
    Node nEcornerNode(const vector<Node>&, const Cell&);
    Node nWcornerNode(const vector<Node>&, const Cell&);
  

    Face sFace(const vector<Face>&, const Cell&,const Cell&);
    Face eFace(const vector<Face>&, const Cell&,const Cell&);
    Face nFace(const vector<Face>&, const Cell&,const Cell&);
    Face wFace(const vector<Face>&, const Cell&,const Cell&);

  
    void setCellNeighbours();
    void setCellNeighboursDistance();
    void setFaceNeighbours();
    void setFaceInterpolationFac();
    
    void setboundaryfacesofmesh();
    void setinternalfacesofmesh();
    void setboundarycellsofmesh();
    void setinternalcellsofmesh();
    
};

#endif // SORTMESH_H