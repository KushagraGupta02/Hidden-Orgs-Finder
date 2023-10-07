#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
struct Edge {
    int u, v; // Nodes connected by the edge

    Edge(int u, int v) : u(u), v(v) {}
};

class UndirectedGraph {
public:
    int numVertices;
    std::vector<std::vector<int> > adjacencyList;
    UndirectedGraph(int numVertices, const std::vector<Edge>& edges) 
        : numVertices(numVertices), adjacencyList(numVertices) {
        for (const Edge& edge : edges) {
            addEdge(edge.u, edge.v);
        }
    }

    void addEdge(int u, int v) {
        adjacencyList[u - 1].push_back(v);
        adjacencyList[v - 1].push_back(u);
    }

    void printGraph() {
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "Node " << (i + 1) << " is connected to:";
            for (int neighbor : adjacencyList[i]) {
                std::cout << " " << neighbor;
            }
            std::cout << std::endl;
        }
    }

private:
    // int numVertices;
    // std::vector<std::vector<int> > adjacencyList;
};

void print(std::vector<std::vector<int>> Newer){
    for (int i=0; i<Newer.size(); i++){
        for (int j=0; j<Newer[i].size(); j++){
        cout<<Newer[i][j]<<" ";
    }
    cout<<"\n";
    }
}

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
    std::ofstream outputFile(inputfilename+".satinput");
    
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1; // Exit the program with an error code
    }
    // Read the first line of input
    inputFile >> numVertices >> numEdges >> K1 >> K2;

    std::vector<Edge> edges;

    // Read the edges
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        inputFile >> u >> v;
        edges.push_back(Edge(u, v)); // Use push_back instead of emplace_back
    }
    // Create the undirected
    UndirectedGraph graph(numVertices, edges);

    // instantiate 1 to k1*n variables to represent the xiv in row major order which says that ith vertex of klique k1 is v
    // next variables nk1+1 to nk1+nk2 represent the xiv in row major order which says that ith vertex of klique k2 is v
    vector<vector<int> > k1hammingconstraint;
    for (int i=0; i< K1; i++){
        // Loop through each column
        vector<int> element1;
        for (int v = 0; v < numVertices; ++v) {
            element1.push_back(i * numVertices + v + 1);
        }
        k1hammingconstraint.push_back(element1);
    }
    // this ensures that xiv is not same as xjv for any i,j
    for (int v = 0; v < numVertices; ++v) {
        // Loop through each column
        
        for (int i=0; i< K1; i++){
            for (int j=i+1; j<K1; j++){
            k1hammingconstraint.push_back({i * numVertices * (-1) - v - 1,j* numVertices * (-1) - v - 1});
            }
        }
    }
    // this ensures that xiv is not same as xiw for any v,w
    for (int i=0; i< K1; i++){
        // Loop through each column
        vector<int> element1;
        for (int v = 0; v < numVertices; ++v) {
            for (int w=v+1; w < numVertices; ++w){
            k1hammingconstraint.push_back({i * numVertices * (-1) - v - 1,i* numVertices * (-1) - w - 1});
            }
        }
    } 

    // now we add the constraint for completeness of the clique
    for (int i=0; i< K1; i++){
            for (int j=i+1; j<K1; j++){
                
            for (int u = 0; u < numVertices; ++u) {
                vector<int> element1;
                element1.push_back(i * numVertices * (-1) - u - 1);
                for (auto v : graph.adjacencyList[u]){
                    element1.push_back(j * numVertices + v);
                }
                k1hammingconstraint.push_back(element1);
            }
            }
    }
    //k2 constraints
    for (int i=0; i< K2; i++){
        // Loop through each column
        vector<int> element1;
        for (int v = 0; v < numVertices; ++v) {
            element1.push_back(numVertices*K1 +i * numVertices + v + 1);
        }
        k1hammingconstraint.push_back(element1);
    }
    // this ensures that xiv is not same as xjv for any i,j
    for (int v = 0; v < numVertices; ++v) {
        // Loop through each column
        
        for (int i=0; i< K2; i++){
            for (int j=i+1; j<K2; j++){
            k1hammingconstraint.push_back({numVertices*K1* (-1)+i * numVertices * (-1) - v - 1,numVertices*K1* (-1)+j* numVertices * (-1) - v - 1});
            }
        }
    }
    // this ensures that xiv is not same as xiw for any v,w
    for (int i=0; i< K2; i++){
        // Loop through each column
        vector<int> element1;
        for (int v = 0; v < numVertices; ++v) {
            for (int w=v+1; w < numVertices; ++w){
            k1hammingconstraint.push_back({numVertices*K1* (-1)+i * numVertices * (-1) - v - 1,numVertices*K1* (-1)+i* numVertices * (-1) - w - 1});
            }
        }
    } 

    // now we add the constraint for completeness of the K2 clique
    for (int i=0; i< K2; i++){
            for (int j=i+1; j<K2; j++){
                
            for (int u = 0; u < numVertices; ++u) {
                vector<int> element1;
                element1.push_back(numVertices*K1* (-1)+i * numVertices * (-1) - u - 1);
                for (auto v : graph.adjacencyList[u]){
                    element1.push_back(numVertices*K1+ j * numVertices + v);
                }
                k1hammingconstraint.push_back(element1);
            }
            }
    }

    // now constraints that all vertices are different in both the cliques xiv is not same as yjv for any i,j
    for (int v = 0; v < numVertices; ++v) {
        // Loop through each column
        
        for (int i=0; i< K1; i++){
            for (int j=0; j<K2; j++){
            k1hammingconstraint.push_back({i * numVertices * (-1) - v - 1,numVertices*K1* (-1)+j* numVertices * (-1) - v - 1});
            }
        }
    }

    // now we add the constraint that there can't be an edge from one vertex of x to y
    // for (int i=0; i< K1; i++){
    //         for (int j=0; j<K2; j++){
                
    //         for (int u = 0; u < numVertices; ++u) {
    //             vector<int> element1;
    //             for (auto v : graph.adjacencyList[u]){
    //                 k1hammingconstraint.push_back({i * numVertices * (-1) - u - 1,numVertices*K1* (-1)+ j * numVertices* (-1) - v});
    //             }
    //         }
    //         }
    // }

    // vector<vector<pair<int,int> > > nonedgeconstraint;
    // for (int i=0;i<numVertices;i++){
    //     sort(graph.adjacencyList[i].begin(),graph.adjacencyList[i].end());
    //     vector<int> notedge;
    //     int count=i+2;
    //     int j=0;
    //     // cout<<"vertex"<<i+1<<"\n";
    //     while (j<graph.adjacencyList[i].size()){
    //         // cout<<graph.adjacencyList[i][j]<<" ";
    //         if (graph.adjacencyList[i][j]>i+1){
    //              while (graph.adjacencyList[i][j]>count && count<numVertices+1){
    //                 notedge.push_back(count);
    //                 // cout<<count<<" ";
    //                 count++;
    //              }
    //              if (graph.adjacencyList[i][j]==count){
    //                 j++;
    //                 count++;
    //              }
    //              if (j==graph.adjacencyList[i].size()){
    //                 while (count<numVertices+1){
    //                 notedge.push_back(count);
    //                 // cout<<count<<" ";
    //                 count++;
    //              }
    //              }
    //         }
    //         else{
    //             j++;
    //         }
    //     }
    //     for (auto j: notedge)
    //     nonedgeconstraint.push_back ( {{i,true},{j,true}} );
        // cout<<"newvertex"<<"\n";
    // }
    
    // Table[1][1]={{{1,false}}}
    // Table[2][1]={{1,2}}
    // Table[2][2]={{1},{2}}

    // Close the input file
    inputFile.close();
    graph.printGraph();
    // Print the graph (for testing)
    outputFile << "p cnf " << K1*numVertices << " " << k1hammingconstraint.size()<<"\n";
    for (int i=0; i<k1hammingconstraint.size(); i++){
        for (int j=0; j<k1hammingconstraint[i].size(); j++){
        outputFile<<k1hammingconstraint[i][j]<<" ";
    }
    outputFile<<0<<"\n";
    }
    // outputFile << "This is a sample text file." << std::endl;

    // Close the file
    outputFile.close();
    // to convert from minisat to back, we see the number x if it is positive (from 1 to nk1), then divide it by n to get i-1, and remainder as v
    // return the set of v's as the answer

    return 0;
}
