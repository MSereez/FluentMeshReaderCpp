#include "unsortedMesh.h"

unsortedMesh::unsortedMesh(fluent2DRead& b_)
: fluentMeshReader(&b_),
totalCells(fluentMeshReader->totalCells),
nodesofmesh(b_.totalNodes),
facesofmesh(b_.totalFaces),
cellsofmesh(b_.totalCells),
unsortedcells(b_.totalCells),
cellnodenumbers(totalCells,vector<int>(0)),
cellfacenumbers(totalCells,vector<int>(0))
{
    totalNodes = fluentMeshReader->totalNodes;
    totalFaces = fluentMeshReader->totalFaces;
    totalCells = fluentMeshReader->totalCells;
    
    cellnodenumbers[0].reserve(totalCells+10);
    cellnodenumbers[1].reserve(totalCells+10);
    cellnodenumbers[2].reserve(totalCells+10);
    cellnodenumbers[3].reserve(totalCells+10);

    inlet =  1;
    outlet = 2;
     wall = 3;
    fluid = 0;
    movingwall =4;
    cout << " Sorting mesh " << endl;
    
    forAll(fluentMeshReader->wallnames)
    {
      wallboundarynames.push_back(fluentMeshReader->wallnames[i]);
    }
    
    // 1  - create nodes
    createNodes();
    
    // 2  - create faces
    createFaces();
    
    // 3 - Find cells from facelist and adj c1, c2
    findcellsfromfaces();
    
    // 4 - Cell Neighbours
     setCellNeighbours();
     
     //5 - Cell Neighbours distance
     setCellNeighboursDistance();
     
     //6 - Face neighbours
     setFaceNeighbours();
     
     //7 - Face Interpolation
     setFaceInterpolationFac();
     
     //8 - Boundary Faces of mesh
     setboundaryfacesofmesh();
     totalBoundaryFaces = boundaryfacesofmesh.size();
     
     //9 - internal faces of mesh
     setinternalfacesofmesh();
     totalInternalFaces = internalfacesofmesh.size();
        
     //10 - Boundary Cells of mesh
     setboundarycellsofmesh();
     totalBoundaryCells = boundaryCellsofmesh.size();
     
     //11 - internal Cells of mesh
     setinternalcellsofmesh();
     totalInternalCells = internalCellsofmesh.size();
}

unsortedMesh::~unsortedMesh()
{
    
    clearMemory();
    
}
void unsortedMesh::clearMemory()
{
       nodelist().swap(nodesofmesh);
       facelist().swap(facesofmesh);
       celllist().swap(cellsofmesh);
     //  cout << " node and faces and cells vector swapped in destructor with emppty !! " << endl;
}


// 1 - SORT NODES
void unsortedMesh::createNodes()
{
    cout << " 1 -> creating mesh nodes" << endl;
    for(size_t i=0; i<totalNodes; i++)
    {
        size_t number = fluentMeshReader->NodeNo[i];
        const float xpos = fluentMeshReader->nodeX[i];
        const float ypos = fluentMeshReader->nodeY[i];
        
        Node N;
        N.x = xpos;
        N.y = ypos;
        N.number = number;
     //   cout << std::setprecision(6)  << "N, x, y " << N.number << " , "  << N.x << " , " << N.y << endl;
        nodesofmesh[i] = N;
        
    } 
    
}


// 2- SORT FACES
void unsortedMesh::createFaces()
{
    cout << " 2 -> creating mesh Faces" << endl;
    for(size_t i=0; i<totalFaces; i++)
    {
        Node Node1(nodesofmesh[fluentMeshReader->faceAllN1[i]]);
        Node Node2(nodesofmesh[fluentMeshReader->faceAllN2[i]]);
        Face F3(Node1,Node2);
        
        F3.faceNo = fluentMeshReader->faceNo[i];
        F3.tagCellNo1 = fluentMeshReader->faceAllC1[i];
        F3.tagCellNo2 = fluentMeshReader->faceAllC2[i];
        F3.facetype = fluentMeshReader->faceZoneType[i];
        F3.faceZoneName = fluentMeshReader->faceNames[i];
        if(F3.facetype != 0)
        {
            F3.isBoundary_ = true;
        }
        
        // finally construct
        facesofmesh[i] = F3;
    } 
    

    
}


// 3- SORT CELLS
void unsortedMesh::findcellsfromfaces()
{
    cout << "3 -> creating mesh cells " << endl;
    
      //  cout << "Face no " << ", " << "Face N1 " <<  ", " << "Face N2 " << ", " << "Face C1 "<< ", " << "Face C2 " << endl;
        Node NodeDummy(nodesofmesh[0]);
            
        Cell CellDummy(NodeDummy,NodeDummy,NodeDummy,NodeDummy);
        
        forAllCells(cellsofmesh)
        {
           unsortedcells[cellI] = CellDummy;        
        }
     
    for(size_t k=0; k<totalFaces; k++)
    {
        Face F = facesofmesh[k];
   //    cout <<F.faceNo << ", " <<F.Node1.number << ", " << F.Node2.number <<  ", " << F.tagCellNo1 << ", " <<F.tagCellNo2 << endl;     
    }       
    
     
    // nodes
    for(size_t k=0; k<totalFaces; k++)
    {
        Face F = facesofmesh[k];     
   
        if((std::find( unsortedcells[F.tagCellNo1].nodenum.begin(),  unsortedcells[F.tagCellNo1].nodenum.end(), F.Node1.number) !=  unsortedcells[F.tagCellNo1].nodenum.end())
        || ((std::find( unsortedcells[F.tagCellNo1].nodenum.begin(),  unsortedcells[F.tagCellNo1].nodenum.end(), F.Node2.number) !=  unsortedcells[F.tagCellNo1].nodenum.end())))
        {
            // ignore if it already contains this node
        } 
        else
        {
           unsortedcells[F.tagCellNo1].nodenum.push_back(F.Node1.number); 
           unsortedcells[F.tagCellNo1].nodenum.push_back(F.Node2.number); 

        }

    }
      
    for(size_t k=0; k<totalFaces; k++)
    {
        Face F = facesofmesh[k];     
        
        if( unsortedcells[F.tagCellNo2].nodenum.size() == 4)
        {
            //if 4 nodes are there already , why do anything? duh!
            // ignore
        }
        else
        {
        if((std::find( unsortedcells[F.tagCellNo2].nodenum.begin(),  unsortedcells[F.tagCellNo2].nodenum.end(), F.Node1.number) !=  unsortedcells[F.tagCellNo2].nodenum.end())
        || ((std::find( unsortedcells[F.tagCellNo2].nodenum.begin(),  unsortedcells[F.tagCellNo2].nodenum.end(), F.Node2.number) !=  unsortedcells[F.tagCellNo2].nodenum.end())))
        {
            // ignore  if it already contains this node
        } 
        else
        {
           unsortedcells[F.tagCellNo2].nodenum.push_back(F.Node1.number); 
           unsortedcells[F.tagCellNo2].nodenum.push_back(F.Node2.number); 

        }
        }
    }
              
    // faces
    for(size_t k=0; k<totalFaces; k++)
    {
        Face F = facesofmesh[k];     
   
        if(std::find( unsortedcells[F.tagCellNo1].facenum.begin(),  unsortedcells[F.tagCellNo1].facenum.end(), F.faceNo) !=  unsortedcells[F.tagCellNo1].facenum.end())
        {
            // ignore if it already contains this face
        } 
        else
        {
           unsortedcells[F.tagCellNo1].facenum.push_back(F.faceNo); 
        }

    }
      
    for(size_t k=0; k<totalFaces; k++)
    {
        Face F = facesofmesh[k];     
        
        if( unsortedcells[F.tagCellNo2].facenum.size() == 4)
        {
            //if 4 faces are there already , why do anything? duh!
            // ignore
        }
        else
        {
        if(std::find( unsortedcells[F.tagCellNo2].facenum.begin(),  unsortedcells[F.tagCellNo2].facenum.end(), F.faceNo) !=  unsortedcells[F.tagCellNo2].facenum.end())
        {
            // ignore  if it already contains this node
        } 
        else
        {
           unsortedcells[F.tagCellNo2].facenum.push_back(F.faceNo); 
        }
        }
    }
        



      
    forAllCells(unsortedcells)
    {
  //    cout << " Cell no: " << cellI << endl; 
        for(size_t k=0; k<unsortedcells[cellI].facenum.size(); k++)
        {
            //     cout << unsortedcells[cellI].facenum[k] << endl;  
        }
        
    }
    
     for(size_t k=0; k<totalCells; k++)
    {
        //   cout << " Cell No " << k << endl;
            unsigned int N1,N2,N3,N4;
            unsigned int F1,F2,F3,F4;
            
            N1 =  unsortedcells[k].nodenum[0]; N2 =  unsortedcells[k].nodenum[1]; N3 = unsortedcells[k].nodenum[2];  N4 =unsortedcells[k].nodenum[3];
            F1 =  unsortedcells[k].facenum[0]; F2 =  unsortedcells[k].facenum[1]; F3 = unsortedcells[k].facenum[2];  F4 =unsortedcells[k].facenum[3];
            Node Node1(nodesofmesh[N1]);
            Node Node2(nodesofmesh[N2]);
            Node Node3(nodesofmesh[N3]);
            Node Node4(nodesofmesh[N4]);
            
            vector<Node> nodesofcell(4);        

            nodesofcell[0] = Node1;             nodesofcell[1] = Node2; 
            nodesofcell[2] = Node3;             nodesofcell[3] = Node4;   
          
            Cell CellDummy(Node1,Node2,Node3,Node4);


            unsortedcells[k] = CellDummy;        
           
             
            Node SW = sWcornerNode(nodesofcell,CellDummy);
            Node SE = sEcornerNode(nodesofcell,CellDummy);
            Node NE = nEcornerNode(nodesofcell,CellDummy);
            Node NW = nWcornerNode(nodesofcell,CellDummy);
        
            Cell CellArranged(SW,SE,NE,NW);     

            vector<Face> facesofcell(4);  
            facesofcell[0] = facesofmesh[F1];      facesofcell[1] = facesofmesh[F2];
            facesofcell[2] = facesofmesh[F3];     facesofcell[3] = facesofmesh[F4];  
            Cell CellFDummy(facesofmesh[F1],facesofmesh[F2],facesofmesh[F3],facesofmesh[F4]);
            unsortedcells[k]= CellFDummy;
            
         //   cout << " CELL XC  " << CellArranged.xc << endl;;
  
  
            Face FS  = sFace(facesofcell,CellFDummy,CellArranged);
            Face FE  = eFace(facesofcell,CellFDummy,CellArranged);
            Face FN  = nFace(facesofcell,CellFDummy,CellArranged);
            Face FW  = wFace(facesofcell,CellFDummy,CellArranged);

        /*
            cout << "SOUTH " << FS.faceNo<< endl;;
           cout << "EAST " << FE.faceNo<< endl;;
            cout << "NORTH " << FN.faceNo << endl;
            cout << "WEST " << FW.faceNo << endl;
        */ 
            Cell CellI(facesofmesh[FS.faceNo],facesofmesh[FE.faceNo],facesofmesh[FN.faceNo],facesofmesh[FW.faceNo],
                        nodesofmesh[SW.number],  nodesofmesh[SE.number],  nodesofmesh[NE.number],  nodesofmesh[NW.number]);     
                
            CellI.cellIndex = k;
            
            forAllCellFaces(CellI.cellFaceList)
            {
                Face F=(CellI.cellFaceList[cF]);
                vector<string> walltypes(5);
                if(F.facetype != fluid)
                {
                //  cout << F.faceNo << endl;
                  CellI.cellType = F.facetype;
                  CellI.cellZoneName = F.faceZoneName;
                }

            }

            
            cellsofmesh[k] = CellI;
        
        }
  
}


Node unsortedMesh::sWcornerNode(const vector<Node>& nodes, const Cell& cellD)
{
  Node N;
  for(size_t k=0; k<4; k++)
    {
    Node nodetotest = nodes[k];
    for(size_t i=0; i<nodes.size(); i++)
    {
        if(( nodetotest.x < cellD.xc) && (nodetotest.y < cellD.yc))
        {
            N = nodetotest;
     //      cout << "SW : " << N.number << endl;
        }
    }
    }
    return N;
    
}
Node unsortedMesh::nEcornerNode(const vector<Node>& nodes, const Cell& cellD)
{
  Node N;
  for(size_t k=0; k<4; k++)
    {
    Node nodetotest = nodes[k];
    for(size_t i=0; i<nodes.size(); i++)
    {
        if(( nodetotest.x > cellD.xc) && (nodetotest.y > cellD.yc))
        {
            N = nodetotest;
       //     cout << "NE : " << N.number << endl;
        }
    }
    }
    return N;
    
}

Node unsortedMesh::sEcornerNode(const vector<Node>& nodes, const Cell& cellD)
{
  Node N;
  for(size_t k=0; k<4; k++)
    {
    Node nodetotest = nodes[k];
    for(size_t i=0; i<nodes.size(); i++)
    {
        if(( nodetotest.x > cellD.xc) && (nodetotest.y < cellD.yc))
        {
            N = nodetotest;
       //     cout << "SE : " << N.number << endl;
        }
    }
    }
    return N;
    
}
Node unsortedMesh::nWcornerNode(const vector<Node>& nodes, const Cell& cellD)
{
  Node N;
  for(size_t k=0; k<4; k++)
    {
    Node nodetotest = nodes[k];
    for(size_t i=0; i<nodes.size(); i++)
    {
        if(( nodetotest.x < cellD.xc) && (nodetotest.y > cellD.yc))
        {
            N = nodetotest;
        //    cout << "NW : " << N.number << endl;
        }
    }
    }
    return N;
    
}

//faces 
Face unsortedMesh::sFace(const vector<Face>& faces, const Cell& cellFD, const Cell& cellND)
{
 Face F;
  for(size_t k=0; k<4; k++)
    {
    Face Ftotest = faces[k];
  //  cout << Ftotest.faceNo << endl;
    for(size_t i=0; i<faces.size(); i++)
    {
        if(( Ftotest.yfc < cellND.yc) && (Ftotest.absxlength == cellND.Fs.absxlength))
        {
           F = Ftotest;
       //    cout << "S face  : " << Ftotest.faceNo << endl;
        }
        else
        {
          //  cout << " Face South not found " << endl;
        }
    }
    }
    return F;   
    
}

Face unsortedMesh::nFace(const vector<Face>& faces, const Cell& cellFD, const Cell& cellND)
{
 Face F;
  for(size_t k=0; k<4; k++)
    {
    Face Ftotest = faces[k];
  //  cout << Ftotest.faceNo << endl;
    for(size_t i=0; i<faces.size(); i++)
    {
        if(( Ftotest.yfc > cellND.yc) && (Ftotest.absxlength == cellND.Fn.absxlength))
        {
           F = Ftotest;
         //  cout << "N face  : " << Ftotest.faceNo << endl;
        }
        else
        {
          //  cout << " Face South not found " << endl;
        }
    }
    }
    return F;   
    
}

Face unsortedMesh::eFace(const vector<Face>& faces, const Cell& cellFD, const Cell& cellND)
{
 Face F;
  for(size_t k=0; k<4; k++)
    {
    Face Ftotest = faces[k];
  //  cout << Ftotest.faceNo << endl;
    for(size_t i=0; i<faces.size(); i++)
    {
        if(( Ftotest.xfc > cellND.xc) && (Ftotest.absylength == cellND.Fe.absylength))
        {
           F = Ftotest;
      //     cout << "E face  : " << Ftotest.faceNo << endl;
        }
        else
        {
        //    cout << " Face East not found " << endl;
        }
    }
    }
    return F;   
    
}

Face unsortedMesh::wFace(const vector<Face>& faces, const Cell& cellFD, const Cell& cellND)
{
 Face F;
  for(size_t k=0; k<4; k++)
    {
    Face Ftotest = faces[k];
  //  cout << Ftotest.faceNo << endl;
    for(size_t i=0; i<faces.size(); i++)
    {
        if(( Ftotest.xfc < cellND.xc) && (Ftotest.absylength == cellND.Fw.absylength))
        {
           F = Ftotest;
        //   cout << "W face  : " << Ftotest.faceNo << endl;
        }
        else
        {
          //  cout << " Face South not found " << endl;
        }
    }
    }
    return F;   
    
}


// 4 - cell neighbours
void unsortedMesh::setCellNeighbours()
{
    
    // 4a - set east, west, north south cell neighbours
    forAllCells(cellsofmesh) //   //access =  cellI
    {
    Cell& CellI = cellsofmesh[cellI];
        
      if(CellI.Fe.facetype != fluid)
        {
        CellI.cellEastNeiIndex = CellI.cellIndex;
        }
     else if(CellI.Fe.facetype == fluid) 
        {
        unsigned int index;
        if(CellI.Fe.tagCellNo1 != CellI.cellIndex)
        {
            index = CellI.Fe.tagCellNo1;
        }
        else 
        {
            index = CellI.Fe.tagCellNo2;
        }
        CellI.cellEastNeiIndex =index;
        }
        
        if(CellI.Fw.facetype != fluid)
        {
        CellI.cellWestNeiIndex = CellI.cellIndex;
        }
        else if(CellI.Fw.facetype == fluid) 
        {
        unsigned int index;
        if(CellI.Fw.tagCellNo1 != CellI.cellIndex)
        {
            index = CellI.Fw.tagCellNo1;
        }
        else 
        {
            index = CellI.Fw.tagCellNo2;
        }
        CellI.cellWestNeiIndex =index;
        }
        
         if(CellI.Fs.facetype != fluid)
        {
        CellI.cellSouthNeiIndex = CellI.cellIndex;
        }
        else if(CellI.Fs.facetype == fluid) 
        {
        unsigned int index;
        if(CellI.Fs.tagCellNo1 != CellI.cellIndex)
        {
            index = CellI.Fs.tagCellNo1;
        }
        else 
        {
            index = CellI.Fs.tagCellNo2;
        }
        CellI.cellSouthNeiIndex =index;
        }
        
        if(CellI.Fn.facetype != fluid)
        {
        CellI.cellNorthNeiIndex = CellI.cellIndex;
        }
        else if(CellI.Fn.facetype == fluid) 
        {
        unsigned int index;
        if(CellI.Fn.tagCellNo1 != CellI.cellIndex)
        {
            index = CellI.Fn.tagCellNo1;
        }
        else 
        {
            index = CellI.Fn.tagCellNo2;
        }
        CellI.cellNorthNeiIndex =index;
        }
    }
    
        forAllCells(cellsofmesh) //   //access =  cellI
    {
    Cell& CellI = cellsofmesh[cellI];
    Cell CellEeast = cellsofmesh[CellI.cellEastNeiIndex];
    Cell CellNorth = cellsofmesh[CellI.cellNorthNeiIndex];

//    cout << " Cell Index " << CellI.cellIndex << endl;

           unsigned int eastnei = CellI.cellEastNeiIndex;
            unsigned int northnei = CellI.cellNorthNeiIndex;     

              CellI.iFactorEastNei = (CellI.Fe.xfc - CellI.xc)/(cellsofmesh[eastnei].xc - CellI.xc);
              CellI.iFactorNorthNei = (CellI.Fn.yfc - CellI.yc)/(cellsofmesh[northnei].yc - CellI.yc);
      

            if(CellEeast.cellIndex == CellEeast.cellEastNeiIndex)
            {
                CellI.iFactorEastNei = 1.0;
            }
            
            if(CellNorth.cellIndex == CellNorth.cellNorthNeiIndex)
            {
                CellI.iFactorNorthNei = 1.0;
            }
            if(CellI.cellIndex == CellI.cellEastNeiIndex)
            {
                CellI.cellBFace = CellI.Fe.faceNo;
                CellI.cellBFaceNei = CellI.Fw.faceNo;
                CellI.cellBNei = CellI.cellWestNeiIndex;
                CellI.cellBNeiNei = cellsofmesh[CellI.cellBNei].cellWestNeiIndex;
                CellI.distToBoundaryFace = abs(CellI.xc - CellI.Fe.xfc);
                CellI.lengthofBoundaryFace = CellI.Fe.faceLength;
                CellI.iFactorEastNei = CellI.distToBoundaryFace / (CellI.Fe.xfc - CellI.xc );
                CellI.iFactorNorthNei = (CellI.Fn.yfc - CellI.yc)/(cellsofmesh[northnei].yc - CellI.yc);
                    // nw corner
                    if(CellI.cellIndex == CellI.cellNorthNeiIndex)
                    {
                            CellI.iFactorNorthNei = CellI.distToBoundaryFace/(CellI.Fn.yfc - CellI.yc );
                    }
            }
            else  if(CellI.cellIndex == CellI.cellWestNeiIndex)
            {
                CellI.cellBFace = CellI.Fw.faceNo;
                CellI.cellBFaceNei = CellI.Fe.faceNo;
                CellI.cellBNei = CellI.cellEastNeiIndex;
                CellI.cellBNeiNei = cellsofmesh[CellI.cellBNei].cellEastNeiIndex;
                CellI.distToBoundaryFace = abs(CellI.xc - CellI.Fw.xfc);
                CellI.lengthofBoundaryFace = CellI.Fw.faceLength;
                CellI.iFactorEastNei =(CellI.Fe.xfc - CellI.xc)/(cellsofmesh[eastnei].xc - CellI.xc);
               CellI.iFactorNorthNei = (CellI.Fn.yfc - CellI.yc)/(cellsofmesh[northnei].yc - CellI.yc);

            }
             else  if(CellI.cellIndex == CellI.cellSouthNeiIndex)
            {
                CellI.cellBFace = CellI.Fs.faceNo;
                CellI.cellBFaceNei = CellI.Fn.faceNo;
                CellI.cellBNei = CellI.cellNorthNeiIndex;
                CellI.cellBNeiNei = cellsofmesh[CellI.cellBNei].cellNorthNeiIndex;
                CellI.distToBoundaryFace = abs(CellI.yc - CellI.Fs.yfc);
                CellI.lengthofBoundaryFace = CellI.Fs.faceLength;
                CellI.iFactorEastNei = (CellI.Fe.xfc - CellI.xc)/(cellsofmesh[eastnei].xc - CellI.xc);
               CellI.iFactorNorthNei =(CellI.Fn.yfc - CellI.yc)/(cellsofmesh[northnei].yc - CellI.yc);

            }
            else  if(CellI.cellIndex == CellI.cellNorthNeiIndex)
            {
               //     cout << " Cell Zone " << CellI.cellZoneName << endl;

                // find which face has same name
                CellI.cellBFace = CellI.Fn.faceNo;
                CellI.cellBFaceNei = CellI.Fs.faceNo;
                CellI.cellBNei = CellI.cellSouthNeiIndex;
                CellI.cellBNeiNei = cellsofmesh[CellI.cellBNei].cellSouthNeiIndex;
                CellI.distToBoundaryFace = abs(CellI.yc - CellI.Fn.yfc);
                CellI.lengthofBoundaryFace = CellI.Fn.faceLength;
                CellI.iFactorEastNei = (CellI.Fe.xfc - CellI.xc)/(cellsofmesh[eastnei].xc - CellI.xc);
                CellI.iFactorNorthNei = (CellI.Fn.yfc -CellI.yc) / (CellI.Fn.yfc - CellI.yc );
            }

            
    }
    
}


void unsortedMesh::setCellNeighboursDistance()
{
   forAllCells(cellsofmesh) //   //access =  cellI
    {
    Cell& CellI = cellsofmesh[cellI];
    CellI.distToEastNeighbourCellCentre = abs(CellI.xc - cellsofmesh[CellI.cellEastNeiIndex].xc);
    CellI.distToNorthNeighbourCellCentre= abs(CellI.yc - cellsofmesh[CellI.cellNorthNeiIndex].yc);

    CellI.distToWestNeighbourCellCentre = abs(cellsofmesh[CellI.cellWestNeiIndex].xc - CellI.xc );
    CellI.distToSouthNeighbourCellCentre= abs(cellsofmesh[CellI.cellSouthNeiIndex].yc - CellI.yc);

    }
    
    
}

// UP TO HERE OKAY!!!
void unsortedMesh::setFaceNeighbours()
{
    forAllCells(cellsofmesh)    //access =  cellI
    {
       Cell& CellI = cellsofmesh[cellI];
        Cell CellW = cellsofmesh[CellI.cellWestNeiIndex];
          Cell CellS = cellsofmesh[CellI.cellSouthNeiIndex];

            if(CellI.cellEastNeiIndex != CellI.cellIndex)
            {
                forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                    Face& F = CellI.cellFaceList[cF];
                    if(F.faceNo == CellI.Fw.faceNo)
                    {
                        facesofmesh[CellI.Fw.faceNo].faceNei = CellI.Fe.faceNo;     
                        facesofmesh[CellI.Fw.faceNo].faceLhsNei = CellW.Fw.faceNo;     
 
                    }
                }
            }
            if(CellI.cellEastNeiIndex == CellI.cellIndex)
            {
                forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                    Face& F = CellI.cellFaceList[cF];
                    if(F.faceNo == CellI.Fw.faceNo)
                    {
                        facesofmesh[CellI.Fw.faceNo].faceNei = CellI.Fe.faceNo; 
                        facesofmesh[CellI.Fw.faceNo].faceLhsNei = CellW.Fw.faceNo;     

                    }
                    else if (F.faceNo == CellI.Fe.faceNo)
                    {
                      facesofmesh[CellI.Fe.faceNo].faceNei = CellI.Fw.faceNo;      
                    }
                }
            }
 
 
            if(CellI.cellNorthNeiIndex != CellI.cellIndex)
            {
                forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                    Face& F = CellI.cellFaceList[cF];
                    if(F.faceNo == CellI.Fs.faceNo)
                    {
                        facesofmesh[CellI.Fs.faceNo].faceNei = CellI.Fn.faceNo;
                        facesofmesh[CellI.Fs.faceNo].faceLhsNei = CellS.Fs.faceNo;     

                    }
                }
            }
            if(CellI.cellNorthNeiIndex == CellI.cellIndex)
            {
                forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                    Face& F = CellI.cellFaceList[cF];
                    if(F.faceNo == CellI.Fs.faceNo)
                    {
                        facesofmesh[CellI.Fs.faceNo].faceNei = CellI.Fn.faceNo;
                        facesofmesh[CellI.Fs.faceNo].faceLhsNei = CellS.Fs.faceNo;     

                    }
                    else if (F.faceNo == CellI.Fn.faceNo)
                    {
                      facesofmesh[CellI.Fn.faceNo].faceNei = CellI.Fs.faceNo;
                    }
                }
            }

    }
    
     forAllCells(cellsofmesh)    //access =  cellI
    {
       Cell& CellI = cellsofmesh[cellI];
        
        Face& FEast = CellI.Fe;
        Face& FWest = CellI.Fw;
        Face& FNorth = CellI.Fn;
        Face& FSouth = CellI.Fs;
        
        FEast.faceNei = facesofmesh[FEast.faceNo].faceNei;
        FWest.faceNei = facesofmesh[FWest.faceNo].faceNei;
        FNorth.faceNei = facesofmesh[FNorth.faceNo].faceNei;
        FSouth.faceNei = facesofmesh[FSouth.faceNo].faceNei;
    
        FEast.faceLhsNei = facesofmesh[FEast.faceNo].faceLhsNei;
        FWest.faceLhsNei = facesofmesh[FWest.faceNo].faceLhsNei;
        FNorth.faceLhsNei = facesofmesh[FNorth.faceNo].faceLhsNei;
        FSouth.faceLhsNei = facesofmesh[FSouth.faceNo].faceLhsNei;
        
    
        
        forAllCellFaces(CellI.cellFaceList)
        {
            Face& F = CellI.cellFaceList[cF];
            F.faceNei = facesofmesh[F.faceNo].faceNei;
            F.faceLhsNei = facesofmesh[F.faceNo].faceLhsNei;
        }
        
    }
}

void unsortedMesh::setFaceInterpolationFac()
{
     // 4c - Interpolation x
   forAllCells(cellsofmesh)    //access =  cellI
    {
       Cell CellI = cellsofmesh[cellI];
            
            forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                     Face F = CellI.cellFaceList[cF];
                    
                    // vertically aligned faces mostly
                    if(F.xun > 0.55f && F.yun < 0.45f)
                    {
                    if (!F.isBoundary_)
                    {
 
                        if (CellI.cellEastNeiIndex !=  CellI.cellIndex)
                        {
                            unsigned int cellNei = CellI.cellEastNeiIndex;
                            facesofmesh[F.faceNo].interpolatetoNeiFac = (CellI.Fe.xfc  - cellsofmesh[cellI].xc)/
                                                            (cellsofmesh[cellNei].xc - cellsofmesh[cellI].xc);     
                        }
                        else if(CellI.cellEastNeiIndex ==  CellI.cellIndex)
                        {
                            unsigned int cellNei = CellI.cellWestNeiIndex;
                            unsigned int faceToCopy = cellsofmesh[cellNei].Fw.faceNo;
                            facesofmesh[F.faceNo].interpolatetoNeiFac = facesofmesh[faceToCopy].interpolatetoNeiFac;     
                        }
                 
                    }
                    if(F.isBoundary_ && CellI.cellWestNeiIndex ==  CellI.cellIndex)
                    {
                     facesofmesh[CellI.Fw.faceNo].interpolatetoNeiFac = 0.0;         
                    }
                    else if (F.isBoundary_ && CellI.cellEastNeiIndex ==  CellI.cellIndex)
                    {
                     facesofmesh[CellI.Fe.faceNo].interpolatetoNeiFac = 1.0;         
                    }
               }
            }
    } 
    
    //interpolation in y direction 
    forAllCells(cellsofmesh)    //access =  cellI
    {
       Cell CellI = cellsofmesh[cellI];
            
            forAllCellFaces(CellI.cellFaceList)  //access = cF
                {
                     Face F = CellI.cellFaceList[cF];
                    
                    // horizontally aligned faces mostly
                    if(F.yun > 0.55f && F.xun < 0.45f)
                    {
                        
                    if (!F.isBoundary_)
                    {
                    
                        if (CellI.cellNorthNeiIndex !=  CellI.cellIndex)
                        {
                            unsigned int cellNei = CellI.cellNorthNeiIndex;
                            facesofmesh[F.faceNo].interpolatetoNeiFac = (CellI.Fn.yfc  - cellsofmesh[cellI].yc)/
                                                            (cellsofmesh[cellNei].yc - cellsofmesh[cellI].yc);     
                        }
                      else if(CellI.cellNorthNeiIndex ==  CellI.cellIndex)
                        {
                            unsigned int cellNei = CellI.cellWestNeiIndex;
                            unsigned int faceToCopy = cellsofmesh[cellNei].Fs.faceNo;
                            facesofmesh[F.faceNo].interpolatetoNeiFac = facesofmesh[faceToCopy].interpolatetoNeiFac;      
                        }
                    }
                    
                      if(F.isBoundary_ && CellI.cellSouthNeiIndex ==  CellI.cellIndex)
                      {
                      facesofmesh[CellI.Fs.faceNo].interpolatetoNeiFac = 0.0;         
                      }
                      else if (F.isBoundary_ && CellI.cellNorthNeiIndex ==  CellI.cellIndex)
                      {
                      facesofmesh[CellI.Fn.faceNo].interpolatetoNeiFac = 1.0;         
                      }
                        
                    }
            }
            
    }
    
      forAllCells(cellsofmesh)    //access =  cellI
    {
       Cell& CellI = cellsofmesh[cellI];
        Face& FEast = CellI.Fe;
        Face& FWest = CellI.Fw;
        Face& FNorth = CellI.Fn;
        Face& FSouth = CellI.Fs;
        
        FEast.interpolatetoNeiFac = facesofmesh[FEast.faceNo].interpolatetoNeiFac;
        FWest.interpolatetoNeiFac = facesofmesh[FWest.faceNo].interpolatetoNeiFac;
        FNorth.interpolatetoNeiFac = facesofmesh[FNorth.faceNo].interpolatetoNeiFac;
        FSouth.interpolatetoNeiFac = facesofmesh[FSouth.faceNo].interpolatetoNeiFac;
    
        forAllCellFaces(CellI.cellFaceList)
        {
            Face& F = CellI.cellFaceList[cF];
            F.interpolatetoNeiFac = facesofmesh[F.faceNo].interpolatetoNeiFac;
        }
    }
    
    
}

void unsortedMesh::setboundaryfacesofmesh()
{
    for(size_t f=0; f<facesofmesh.size(); f++)
    {
//        cout << " face no " << facesofmesh[f].faceNo << endl;
         if(facesofmesh[f].facetype != fluid)
         {
            // cout << "bf " << facesofmesh[f].faceNo << endl;
            boundaryfacesofmesh.push_back(facesofmesh[f]);
         }
   
        
    }
    
}
  
void unsortedMesh::setinternalfacesofmesh()
{
    for(size_t f=0; f<facesofmesh.size(); f++)
    {
//        cout << " face no " << facesofmesh[f].faceNo << endl;
         if(facesofmesh[f].facetype == fluid)
         {
            // cout << "bf " << facesofmesh[f].faceNo << endl;
            internalfacesofmesh.push_back(facesofmesh[f]);
         }
   
        
    }
    
}
   
    
     

void unsortedMesh::setboundarycellsofmesh()
{
    for(size_t f=0; f<cellsofmesh.size(); f++)
    {
         if(cellsofmesh[f].cellZoneName != "FLUID")
         {
            boundaryCellsofmesh.push_back(cellsofmesh[f]);
         }
   
        
    }
    
}
   
 
 
 void unsortedMesh::setinternalcellsofmesh()
{
    for(size_t f=0; f<cellsofmesh.size(); f++)
    {
         if(cellsofmesh[f].cellZoneName == "FLUID")
         {
            internalCellsofmesh.push_back(cellsofmesh[f]);
         }
   
        
    }
    
}
   
 