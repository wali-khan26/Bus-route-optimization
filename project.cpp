#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits> // For INT_MAX
#include <vector>
#include <algorithm> // For find
using namespace std;

// Node structure for adjacency list
struct Node {
    int vertex;
    int weight;
    Node* next;
};

// Graph class
class Graph {
public:
    int V; // Number of vertices
    Node** adjList;

    Graph(int vertices) {
        V = vertices;
        adjList = new Node*[V];
        for (int i = 0; i < V; i++) {
            adjList[i] = NULL;
        }
    }

    // Add edge to the graph
    void addEdge(int from, int to, int weight) {
        Node* newNode = new Node;
        newNode->vertex = to;
        newNode->weight = weight;
        newNode->next = adjList[from];
        adjList[from] = newNode;

        // Undirected graph: add edge both ways
        newNode = new Node;
        newNode->vertex = from;
        newNode->weight = weight;
        newNode->next = adjList[to];
        adjList[to] = newNode;
    }

    // Dijkstra's Algorithm
    int dijkstra(int src, int dest) {
        int dist[V];        // Stores shortest distances
        bool visited[V];    // Tracks visited nodes

        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            visited[i] = false;
        }
        dist[src] = 0; // Distance to the source is 0

        for (int count = 0; count < V - 1; count++) {
            int u = -1;
            for (int i = 0; i < V; i++) {
                if (!visited[i] && (u == -1 || dist[i] < dist[u])) {
                    u = i;
                }
            }

            if (u == -1) break; // All reachable nodes have been visited

            visited[u] = true;

            Node* temp = adjList[u];
            while (temp != NULL) {
                int v = temp->vertex;
                int weight = temp->weight;

                if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
                temp = temp->next;
            }
        }

        return dist[dest];
    }

    // Destructor to clean up memory
    ~Graph() {
        for (int i = 0; i < V; i++) {
            Node* temp = adjList[i];
            while (temp != NULL) {
                Node* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
        delete[] adjList;
    }
};

// Function to convert string to integer (C++98 compatible)
int stringToInt(const string& str) {
    stringstream ss(str);
    int value;
    ss >> value;
    return value;
}

// Function to count the number of unique stops from the CSV file
int countUniqueStops() {
    ifstream file("C:\\Users\\Dell\\Desktop\\DSAproject\\bus_stops.csv");
    string line;
    vector<int> stops;

    if (!file.is_open()) {
        cout << "Error opening file for counting unique stops!\n";
        return 0;
    }

    getline(file, line); // Skip the header
    while (getline(file, line)) {
        stringstream ss(line);
        string from, to, distance;
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, distance, ',');

        int fromStop = stringToInt(from);
        int toStop = stringToInt(to);

        if (find(stops.begin(), stops.end(), fromStop) == stops.end())
            stops.push_back(fromStop);
        if (find(stops.begin(), stops.end(), toStop) == stops.end())
            stops.push_back(toStop);
    }

    file.close();
    return stops.size();
}

// Function to load graph data from the CSV file
void loadGraphFromFile(Graph& graph) {
    ifstream file("C:\\Users\\Dell\\Desktop\\DSAproject\\bus_stops.csv");
    string line;

    if (file.is_open()) {
        getline(file, line); // Skip the header
        while (getline(file, line)) {
            stringstream ss(line);
            string from, to, distance;
            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, distance, ',');

            int fromStop = stringToInt(from);
            int toStop = stringToInt(to);
            int dist = stringToInt(distance);

            graph.addEdge(fromStop, toStop, dist);
        }
        file.close();
    } else {
        cout << "Error opening file!\n";
    }
}

// Function to print all data from the CSV file
void printAllBusData() {
    ifstream file("C:\\Users\\Dell\\Desktop\\DSAproject\\bus_stops.csv");
    string line;

    if (!file.is_open()) {
        cout << "Error opening file for reading!\n";
        return;
    }

    cout << "Bus Stop Data:\n";
    cout << "From Stop, To Stop, Distance\n";

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

int main() {
    int numStops = countUniqueStops();
    if (numStops == 0) {
        cout << "No bus stops found. Exiting program.\n";
        return 1;
    }

    Graph graph(numStops);
    loadGraphFromFile(graph);

    cout << "Bus stops loaded successfully.\n";

    // Print all bus stop data
    printAllBusData();

    int src, dest;
    cout << "\nEnter the source bus stop: ";
    cin >> src;
    cout << "Enter the destination bus stop: ";
    cin >> dest;

    if (src >= numStops || dest >= numStops || src < 0 || dest < 0) {
        cout << "Invalid bus stop numbers.\n";
        return 1;
    }

    int distance = graph.dijkstra(src, dest);
    if (distance == INT_MAX) {
        cout << "No path exists between Bus Stop " << src << " and Bus Stop " << dest << ".\n";
    } else {
        cout << "Shortest distance from Bus Stop " << src << " to Bus Stop " << dest << ": " << distance << " km\n";
    }

    return 0;
}

