#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <array>

#include "unsortedMesh.h"


using namespace std;

// inlet = 1; outlet = 2; wall=3; fluid=0; movingwall=4;

int main(int argc, char **argv)
{
    fluent2DRead fReader_; 
    string filename = "Lid10by10.msh";
    #include "createMesh.h"
   

    cout << " end of program success " << endl;
	return 0;
}




