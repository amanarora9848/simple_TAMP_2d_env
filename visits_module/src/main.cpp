#include <iostream>
#include "VisitSolver.h"
#include <fstream>
#include <list>
#include <bits/stdc++.h> 

using namespace std;



int main(int argc, char **argv) {
    
    VisitSolver visitExample;
    string problem;
    
    if (argc > 2){
        problem = argv[1];
    }else{
        cout << "Usage:\n"
        << argv[0] << " problem parameters plan" << endl;
        return 1;
    }
    string parameters[] = {0};
    visitExample.loadSolver(parameters,1);
    map<string,double> initialState;

    initialState["(act-cost)"] = 0;
    initialState["(dummy)"] = 0;
    

    // Declaring Vector of String type 
    vector <string> region; 
  
    // Initialize vector with regions using push_back  
    // command 
    region.push_back("r0"); 
    region.push_back("r1"); 
    region.push_back("r2"); 
    region.push_back("r3"); 
    region.push_back("r4"); 
  
    // store the initial state fluents
    for (int i=0; i<region.size(); i++)   { 
    for(int j=0; j<region.size(); j++){
       if (i != j){       
        initialState["(triggered " + region[i] + " " + region[j]+")"] = 0;
       }
    }
    }   
    return 0;
}
