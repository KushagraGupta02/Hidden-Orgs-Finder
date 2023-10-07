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

int main() {
    int numVertices, numEdges, K1, K2;

    // Open the input file
    std::ifstream inputFile("../test.graph");
    std::ofstream outputFile("output.txt");
    
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
    cout<<K1<<numVertices;
    vector<vector<vector<vector<pair<int,bool> > > > > Table(numVertices+1, std::vector<std::vector<std::vector<pair<int,bool> > > >(K1+1));
    for (int i =0; i<numVertices+1; i++){
        std::vector<std::vector<pair<int,bool> > > emptyvector;  
        Table[i][0]=emptyvector;
    }
    for (int k =1; k<K1+1;k++){
        Table[k-1][k]={{}};
    }
    for (int k =1; k<K1+1;k++){
        for (int n =k; n<numVertices+1; n++){
            // cout<<"n"<<n<<" "<<"k"<<k<<endl;
            std::vector<std::vector<pair<int,bool> > > newentry; 
            // std::vector<std::vector<pair<int,bool> > > tempentry; 
            std::vector<std::vector<pair<int,bool> > > firstclause; 
            std::vector<std::vector<pair<int,bool> > > secondclause;
            std::vector<std::vector<pair<int,bool> > > thirdclause;
            for (int t =0; t<Table[n-1][k].size(); t++){
                vector<pair<int,bool> > element =Table[n-1][k][t];
                element.push_back({n,false});
                firstclause.push_back(element);
                // for (const auto& pair : element) {
                // std::cout << "(" << pair.first << ", " << pair.second << ") ";
                // }
            }
            for (int t =0; t<Table[n-1][k-1].size(); t++){
                vector<pair<int,bool> > element =Table[n-1][k-1][t];
                element.push_back({n,true});
                secondclause.push_back(element);
            }
            for (int t =0; t<Table[n-1][k].size(); t++){
                for (int u =0; u<Table[n-1][k-1].size(); u++){
                    vector<pair<int,bool> > element1 =Table[n-1][k][t];
                    vector<pair<int,bool> > element2 =Table[n-1][k-1][u];
                    vector<pair<int,bool> >  element3;
                    std::set_union(element1.begin(), element1.end(), element2.begin(), element2.end(), std::back_inserter(element3));
                    vector<pair<int,bool> >  element4;
                    int prevX = -1; // Initialize with a value that won't be in the vector
                    bool prevValue = false;
                    bool removee =false;
                    int i=0;
                    while (i < element3.size()) {
                        int x = element3[i].first;
                        bool value = element3[i].second;
                        // Check if the current integer and boolean values match the previous ones
                        if (x == prevX && value != prevValue) {
                            // Remove the current element from the vector
                            removee=true;
                            break;
                            // element4.pop_back();
                            // prevX = -1;
                            // prevValue = false;
                        } 
                        else {
                            // Update the previous values
                            element4.push_back(element3[i]);
                            prevX = x;
                            prevValue = value;
                            
                        }
                        if (removee){
                            break;
                        }
                        i++;
                    }
                    if (!removee)
                    thirdclause.push_back(element4);
                }
            }
            // std::set_union(firstclause.begin(), firstclause.end(), secondclause.begin(), secondclause.end(), std::back_inserter(tempentry));
            // std::set_union(tempentry.begin(), tempentry.end(), thirdclause.begin(), thirdclause.end(), std::back_inserter(newentry));
            for (int t =0; t<firstclause.size(); t++){
                newentry.push_back(firstclause[t]);
            }
            for (int t =0; t<secondclause.size(); t++){
                newentry.push_back(secondclause[t]);
            }
            for (int t =0; t<thirdclause.size(); t++){
                newentry.push_back(thirdclause[t]);
            }
            Table[n][k]=newentry;
        }
    }
    vector<vector<pair<int,int> > > nonedgeconstraint;
    for (int i=0;i<numVertices;i++){
        sort(graph.adjacencyList[i].begin(),graph.adjacencyList[i].end());
        vector<int> notedge;
        int count=i+2;
        int j=0;
        // cout<<"vertex"<<i+1<<"\n";
        while (j<graph.adjacencyList[i].size()){
            // cout<<graph.adjacencyList[i][j]<<" ";
            if (graph.adjacencyList[i][j]>i+1){
                 while (graph.adjacencyList[i][j]>count && count<numVertices+1){
                    notedge.push_back(count);
                    // cout<<count<<" ";
                    count++;
                 }
                 if (graph.adjacencyList[i][j]==count){
                    j++;
                    count++;
                 }
                 if (j==graph.adjacencyList[i].size()){
                    while (count<numVertices+1){
                    notedge.push_back(count);
                    // cout<<count<<" ";
                    count++;
                 }
                 }
            }
            else{
                j++;
            }
        }
        for (auto j: notedge)
        nonedgeconstraint.push_back ( {{i,true},{j,true}} );
        // cout<<"newvertex"<<"\n";
    }
    
    // Table[1][1]={{{1,false}}}
    // Table[2][1]={{1,2}}
    // Table[2][2]={{1},{2}}

    // Close the input file
    inputFile.close();
    graph.printGraph();
    // Print the graph (for testing)
    outputFile << "Hello, Word!" << std::endl;
    outputFile << "This is a sample text file." << std::endl;

    // Close the file
    outputFile.close();

    

    return 0;
}
