#include "fluent2dRead.h"

fluent2DRead::fluent2DRead()
{
    noOfIntFaces = 0;
    noOfNodes = 0;
    noOfWalls = 0;
    nodeinformationLine1 = 0;
    interiorFaceInformationLine1 = 0;
    zonecount = 0;
    
    
    totalCells = 0;
    totalNodes = 0;
    totalBoundaryFaces = 0;
    totalFaces = 0;

}

fluent2DRead::~fluent2DRead()
{
  //  cout << " deallocating " << endl;
    delloacte();
}

void fluent2DRead::readbasicStructure(std::string name)
{
         char cstr[name.size() + 2];
        strcpy(cstr, name.c_str());
        
        
        //2. Opens the mesh file
        ifstream mesh_file;
        mesh_file.open( cstr);


// start reading
 string line; 
 int count=0;
 while (getline(mesh_file, line))
 {
    
    count++;
    fileinstring.push_back(line);
    int nodeNumberLine;
    if ( (line.find( "Node Section") != std::string::npos))
    {
        nodeNumberLine =count+1;
        nodeinformationLine1 = count + 3;
        cout << "Node number information will be from : " << nodeNumberLine << endl;

    }
    
   // ***********************************************************************************************************
   //  1 - NODES
   // A method to get no of nodes.
   // ***********************************************************************************************************

       int n1,n2,n3,n4,n5,n6;
       if(count == nodeNumberLine)
       {    
           string t = line;
           replace (t.begin(), t.end(), '(' , ' ');
           replace (t.begin(), t.end(), ')' , ' ');
           replace (t.begin(), t.end(), '"' , ' ');
           std::stringstream ss2(t);
           ss2  >> hex >> n1 >> n2 >> n3 >> n4 >> n5 >> n6 ;
        //   cout << t << endl;     
           cout << "no of nodes " << n4 << endl;
           noOfNodes = n4;
       }
       
//***********************************************************************************************************
//  2 - Interior Faces
//  A method to get no of face counts.
//***********************************************************************************************************       
       
    int interiorFaceLine; 
    int interiorFaceZoneNameLine; 
    if ( (line.find( "Interior faces") != std::string::npos))
    {
        interiorFaceLine = count+1;
        interiorFaceInformationLine1 = count + 1;
        interiorFaceZoneNameLine = count;

    }
    
       if(count == interiorFaceZoneNameLine)
       {    
       string s1="Faces";
       string s2="zone";
       string s3="of";
       string word;
       string zoneNowname;
       string t = line;
       istringstream iss(t);
        while(iss >> word) {
          if( (word!=s1) && (word!=s2) && (word!=s3))
          {
              zoneNowname = word;
          }
        }
        
        replace (zoneNowname.begin(), zoneNowname.end(), '"' , ' ');
        replace (zoneNowname.begin(), zoneNowname.end(), ')' , ' ');

        cout << "zone name " << zoneNowname << endl;
        zonenames.push_back(zoneNowname);
        zonecount +=1;

       }
       
       int f1,f2,f3,f4,f5,f6;
       if(count == interiorFaceLine)
       {    
           string t1 = line;
           replace (t1.begin(), t1.end(), '(' , ' ');
           replace (t1.begin(), t1.end(), ')' , ' ');
           replace (t1.begin(), t1.end(), '"' , ' ');
           std::stringstream ss2(t1);
           ss2  >> hex >> f1 >> f2 >> f3 >> f4 >> f5 >> f6 ;
           cout << "no of interior faces " << f4 << endl;
         //  cout << t1 << endl;
           noOfIntFaces = f4;
       }
//***********************************************************************************************************
//  3 - Boundaries
//  A method to get no of boundary face counts.
//***********************************************************************************************************  
   int wcount=0;
    if ( (line.find( "Faces of zone") != std::string::npos))
    {
        wcount = count + 1;
        wallInfoPos.push_back(wcount);
        noOfWalls +=1;
        
    }
    

    if ( (line.find( "Faces of zone") != std::string::npos))
    {
        string t = line;
                replace (t.begin(), t.end(), '(' , ' ');
                replace (t.begin(), t.end(), ')' , ' ');
                replace (t.begin(), t.end(), '"' , ' ');
       // cout << "string" << t << endl;
        istringstream iss(t);
       string s2="0";
       string s1="Faces";
       string s3="zone";
       string s4="of";

       string word;
        while(iss >> word) {
          if( (word!=s1) && (word!=s2) && (word!=s3) && (word!=s4) )
          {
                      wallnames.push_back(word);
          }
        }
    }


 }  
 
 
      mesh_file.close();
      
    cout << " No of walls " << noOfWalls << endl;
    for(unsigned int i=0; i< noOfWalls; i++)
    {
    //   cout << "Name " << wallnames[i] << endl;
     //  cout << " wall info " << wallInfoPos[i] << endl;
    }
    
    wallFaceCount.reserve(wallnames.size());

        
}



void fluent2DRead::checkStringFile()
{
    
    for(unsigned int k =0; k< wallInfoPos.size(); k++)
    {
         string t ;
         int pos = wallInfoPos[k];
        
        cout << fileinstring[pos] << endl;
    }
    
    
}

void fluent2DRead::getNodes()
{
    
    
    for(unsigned i=0; i<fileinstring.size(); i++)
    {
        double xpos = 0.0f;
        double ypos = 0.0f;
        if (i>=nodeinformationLine1 && i <= nodeinformationLine1+noOfNodes-1)
        {
            unsigned int k= i-nodeinformationLine1;
            string t = fileinstring[i];
            std::stringstream ss2(t);
            ss2.precision(10);
            ss2 >> fixed >> xpos >> ypos;
            nodeX.push_back(xpos);
            nodeY.push_back(ypos);
            NodeNo.push_back(k);
        }
        
    }
    
}

void fluent2DRead::getInteriorFaces()
{
    
    for(unsigned i=0; i<fileinstring.size(); i++)
    {

        int N1,N2,C1,C2;
        if (i>=interiorFaceInformationLine1 && i <= interiorFaceInformationLine1+noOfIntFaces-1)
        {
            string t = fileinstring[i];
            std::stringstream ss2(t);
             ss2  >> hex >> N1 >> N2 >> C1 >> C2;
            faceN1.push_back(N1);
            faceN2.push_back(N2);
            faceC1.push_back(C1);
            faceC2.push_back(C2);
    
        }
        
    }
    
    
}

void fluent2DRead::getBoundaryFaces()
{

        for(size_t  k=0; k<wallInfoPos.size()-1; k++)
        {
         std::string wallname = wallnames[k+1];
         unsigned int wcount=0;
         unsigned int wallfacecount_=0;
         
         for(size_t i=0; i<fileinstring.size(); i++)
         {
         wcount++;
         string t = (fileinstring[i]);
            
            if((t.find("Faces of zone") != std::string::npos) && ((t.find(wallname) != std::string::npos)) )
            {
            string t2 = fileinstring[i];
            wallfacecount_ = wcount -wallInfoPos[k]-3 ;
            }     

          }
        wallFaceCount.push_back(wallfacecount_);
        }

        // last boundary of walls
        unsigned int k=wallInfoPos.size()-1;
        unsigned int wcount=0;
        unsigned int wallfacecount_=0;


        for(unsigned i=0; i<fileinstring.size(); i++)
         {
         wcount++;
         string t = (fileinstring[i]);
            
          if(t.find("Zone Sections") != std::string::npos)
            {
            string t2 = fileinstring[i];
            wallfacecount_ = wcount - wallInfoPos[k]-3 ;
            } 

           }  
           wallFaceCount.push_back(wallfacecount_);



       for(size_t k=0; k<wallnames.size(); k++)
        {
            totalBoundaryFaces += wallFaceCount[k];
            
        }
        
        totalFaces = totalBoundaryFaces + noOfIntFaces;
        totalNodes = noOfNodes;
        cout << " total boundary faces " << totalBoundaryFaces << endl;
        cout << " total faces " << totalFaces << endl;
        
        // reserve some space
        faceAllN1.reserve(totalFaces);
        faceAllN2.reserve(totalFaces);
        faceAllC1.reserve(totalFaces);
        faceAllC2.reserve(totalFaces);
        faceNames.reserve(totalFaces);
        faceZoneType.reserve(totalFaces);
        faceNo.reserve(totalFaces);

       

        
      for(size_t k =0; k< wallInfoPos.size(); k++)
      {

         string t ;
         unsigned int pos = wallInfoPos[k];
         unsigned int pos2 = wallInfoPos[k] + wallFaceCount[k];
         unsigned int walltypeboundary;
         cout << " Enter boundary  zone type for " << wallnames[k] << " 0 internal, 1 inlet, 2 outlet, 3 wall, 4 movingwall " << endl;
         cin >> walltypeboundary;
         
         
        int N1,N2,C1,C2;
        for(size_t i=pos; i<pos2; i++)
        {
         string t = (fileinstring[i]);

            if( i >= pos && (t.find( ")" ) == std::string::npos) ) 
            {
            string t1 = fileinstring[i];
            std::stringstream ss2(t1);
            ss2  >> hex >> N1 >> N2 >> C1 >> C2;
            wallFaceN1.push_back(N1);
            wallFaceN2.push_back(N2);
            wallFaceC1.push_back(C1);
            wallFaceC2.push_back(C2);
            wallFaceZoneType.push_back(walltypeboundary);
            wallFaceZoneName.push_back(wallnames[k]);

             
            } 
         }  
      }  
       
}

void fluent2DRead::sortallvectors()
{
    
    unsigned int one= 1;
    cout << " No. of zones in mesh " << zonenames.size() << endl;
    for(size_t i=0; i<noOfIntFaces; i++)
    {
        faceAllN1.push_back(faceN1[i]-one);
        faceAllN2.push_back(faceN2[i]-one);
        faceAllC1.push_back(faceC1[i]-one);
        faceAllC2.push_back(faceC2[i]-one);
        faceNames.push_back(zonenames[0]);
        faceZoneType.push_back(0);
        faceNo.push_back(i);
    }
    
    for(size_t i=faceNames.size(); i<totalFaces; i++)
    {
        int pos = i - noOfIntFaces;
        faceAllN1.push_back(wallFaceN1[pos]-one);
        faceAllN2.push_back(wallFaceN2[pos]-one);
        faceAllC1.push_back(wallFaceC1[pos]-one);
        faceAllC2.push_back(wallFaceC2[pos]);
        faceNames.push_back(wallFaceZoneName[pos]);
        faceZoneType.push_back(wallFaceZoneType[pos]);
        faceNo.push_back(i);
        
    }
    
    
   // cout << "Face No " << ", " << "Face N1 " << ", " << "Face N2 " << ", " << "Face c1 " << ", " << "Face C2 " << endl;
    //for(int i=0; i<totalFaces; i++)
    {
      // cout << i  << ", " << faceAllN1[i]  << ", " << faceAllN2[i]   << ", " << faceAllC1[i]   << ", " << faceAllC2[i]  << endl;   
    }
    // find total cells
    
    int c1 = *std::max_element(faceAllC1.begin(),faceAllC1.end());
    int c2 = *std::max_element(faceAllC2.begin(),faceAllC2.end());       

    int cmax = std::max(c1,c2);
    
    
    cout << " total cells " << cmax+1 << endl;
    totalCells = cmax+1;
    
    
}