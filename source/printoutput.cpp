#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
struct Edge {
    int u, v; // Nodes connected by the edge

    Edge(int u, int v) : u(u), v(v) {}
};

int main(int argc, char** argv )
{

     // Parse the input.
    if ( argc < 2 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    int numVertices, numEdges, K1, K2;

    // Open the input file

    std::ifstream inputFile(inputfilename+".graph");
    std::ifstream outputFile(inputfilename+".satoutput");
    std::ofstream outputMapping(inputfilename+".mapping");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open sat output file." << std::endl;
        return 1; // Exit the program with an error code
    }

    if (!outputMapping.is_open()) {
        std::cerr << "Failed to open mapping output file." << std::endl;
        return 1; // Exit the program with an error code
    }
    // Read the first line of input
    inputFile >> numVertices >> numEdges >> K1 >> K2;
    inputFile.close();

    std::string firstWord;
    outputFile >> firstWord;
    if (firstWord=="SAT"){
    vector<int> mapping1;
    vector<int> mapping2;
    for (int i = 0; i < numVertices*K1; ++i) {
        int u;
        outputFile >> u;
        if (u>0){
            u=u%numVertices;
            if (u==0) u=numVertices;
            mapping1.push_back(u);
        }
    }
    for (int i = 0; i < numVertices*K2; ++i) {
        int u;
        outputFile >> u;
        if (u>0){
            u=(u-numVertices*K1)%numVertices;
            if (u==0) u=numVertices;
            mapping2.push_back(u);
        }
    }
    outputFile.close();
    outputMapping << "#1"<<"\n";
    for (int i=0; i<mapping1.size(); i++){
        outputMapping<<mapping1[i]<<" ";
    }
    outputMapping<<"\n";
    outputMapping << "#2"<<"\n";
    for (int i=0; i<mapping2.size(); i++){
        outputMapping<<mapping2[i]<<" ";
    }
    outputMapping<<"\n";
    // outputFile << "This is a sample text file." << std::endl;

    // Close the file
    outputMapping.close();
    }
    else{
        outputMapping<<0;
    }
    // instantiate 1 to k1*n variables to represent the xiv in row major order which says that ith vertex of klique k1 is v
    // next variables nk1+1 to nk1+nk2 represent the xiv in row major order which says that ith vertex of klique k2 is v

    // now constraints that all vertices are different in both the cliques xiv is not same as yjv for any i,j
    // Print the graph (for testing)
    
    // to convert from minisat to back, we see the number x if it is positive (from 1 to nk1), then divide it by n to get i-1, and remainder as v
    // return the set of v's as the answer

    return 0;
}

