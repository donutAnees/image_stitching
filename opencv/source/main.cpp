#include <iostream>
#include "../header/stitcher.hpp"

using namespace std;

int main(int argc, char *argv[]){
    if (argc < 3){
        cerr << "Usage: " << argv[0] << " <argument1> <argument2> [<argument3> <argument4>...]" <<endl;
        return -1;
    }
   Stitcher stitcher = Stitcher();

    for (int i = 1; i < argc; i++){

    }

    return 0;
}