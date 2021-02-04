#ifndef FLUENT2DREAD_H
#define FLUENT2DREAD_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <locale>
#include <algorithm>
#include <iterator>
#include <vector>
#define _countof(array) (sizeof(array) / sizeof(array[0]))

using namespace std;

class fluent2DRead
{
public:
    fluent2DRead();
    virtual ~fluent2DRead();
    
    unsigned int noOfIntFaces;
    unsigned int noOfNodes;
    unsigned int noOfWalls;
    
    unsigned int totalCells,totalNodes,totalBoundaryFaces, totalFaces;

    std::vector<string> fileinstring;
    std::vector<string> zonenames;

    std::vector<unsigned int> wallInfoPos;
    std::vector<unsigned int> wallFaceCount;
    std::vector<string> wallnames;


    // Below vector goes - >   pos 0 - Node X pos 1 - Node Y
    std::vector<double> nodeX; 
    std::vector<double> nodeY;   
    std::vector<int> NodeNo;  

    
    
    // Below vector goes for interior faces N1, N2, adj c1, adj c2
    std::vector<int> faceN1;   
    std::vector<int> faceN2;   
    std::vector<int> faceC1;  
    std::vector<int> faceC2;  

    std::vector<int> wallFaceN1;   
    std::vector<int> wallFaceN2;    
    std::vector<int> wallFaceC1;   
    std::vector<int> wallFaceC2;  
    std::vector<int> faceNo;   
    std::vector<string> wallFaceZoneName;    // de-allocate de-allocate this 
    std::vector<int> wallFaceZoneType;    // de-allocate de-allocate this 
    // a little note faceZoneType = 0 -> interior, 1 -> inlet, 2->outlet, 3 - wall

    std::vector<int> faceAllN1;       // cant de-allocate this until it goes to next class
    std::vector<int> faceAllN2;       // cant de-allocate this until it goes to next class   
    std::vector<int> faceAllC1;       // cant de-allocate this until it goes to next class
    std::vector<int> faceAllC2;       // cant de-allocate this until it goes to next class  
    std::vector<string> faceNames;       // cant de-allocate this until it goes to next class  
    std::vector<int> faceZoneType;       // cant de-allocate this until it goes to next class  
    // a little note faceZoneType = 0 -> interior, 1 -> inlet, 2->outlet, 3 - wall
    
    std::vector<vector<int> > cellAll;       // possiblyCan de-allocate this until it goes to next class  
    std::vector<vector<int> > cellAllSorted;       // cant de-allocate this until it goes to next class  

    // Vector goes-> CellNo, N1, N2, N3, N4, F1, F2, F3, F4

    
    void readbasicStructure(std::string name);
    void checkStringFile();
    void getNodes();
    void getInteriorFaces();
    void getBoundaryFaces();
    void sortallvectors();

    
    void arrangeCellNodesFaces();
    void delloacte()
    {
         // the below method deallocates memory if no longer needed
        // vector<int>().swap(wallFaceN2);
         vector<int>().swap(faceN1);
         vector<int>().swap(faceN2);
         vector<int>().swap(faceC1);
         vector<int>().swap(faceC2);        
         vector<int>().swap(wallFaceN1);
         vector<int>().swap(wallFaceN2);
         vector<int>().swap(wallFaceC1);
         vector<int>().swap(wallFaceC2);
         vector<int>().swap(faceNo);
         vector<string>().swap(wallFaceZoneName);
         vector<int>().swap(wallFaceZoneType);
         vector<int>().swap(faceAllN1);
         vector<int>().swap(faceAllN2);
         vector<int>().swap(faceAllC1);
         vector<int>().swap(faceAllC2);
         vector<string>().swap(faceNames);
         vector<int>().swap(faceZoneType);
    }

private: 

    unsigned int nodeinformationLine1;
    unsigned int interiorFaceInformationLine1;
    unsigned int zonecount;
    
    


};

#endif // FLUENT2DREAD_H