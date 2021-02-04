#ifndef FORALLOPERATIONS_H
#define FORALLOPERATIONS_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;

template<class T>
class foralloperations
{
public:
    foralloperations<T>(){};
    virtual ~foralloperations<T>(){};
    vector<vector<T> > tempvector;
    
}; 
#define forAllCells(tempvector) \
    for(unsigned int cellI=0; cellI<tempvector.size(); cellI++) 
#define forAllFaces(tempvector) \
    for(unsigned int patchI=0; patchI<tempvector.size(); patchI++) 
#define forAllCellFaces(tempvector) \
    for(unsigned int cF=0; cF<4; cF++) 
#define forAll(tempvector) \
    for(unsigned int i=0; i<tempvector.size(); i++) 

         
 
        
#endif // FORALLOPERATIONS_H