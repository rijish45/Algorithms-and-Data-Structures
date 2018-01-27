#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <list>
#include "Graph.h"

//Spencer Liu and Rijish Ganguly

using namespace std;

Graph::Graph()

{
	vertices.clear();
	edges.clear();
	weights.clear();
}

int Graph::addVertex()

{
	//Create a vertex with ID 0 if no vertices exist
	if(vertices.size() == 0){
		vertices.insert(0);								//Constant because size = 0
		return 0;
	}
	//Add an additional vertices in the order of natural numbers
	else{
		set<int>::iterator it = vertices.end();
		it--;
		int vertex = *it;
		vertex++;
		vertices.insert(vertices.end(), vertex);		//Constant runtime because of position hint
		return vertex;
	}
}


void Graph::addEdge(directedEdge newEdge, int weight){

	//Making sure the source and destination vertices exist
	set<int>::iterator source;
	set<int>::iterator dest;
	source = vertices.find(newEdge.first);			//Runtime log(V)
	dest = vertices.find(newEdge.second);			//Runtime log(V)

	if(source == vertices.end() || dest == vertices.end()){
		cout << "Error: addEdge" << endl;		
		return;
	}
	
	//Inserting edge into set of edges and its weight pairing into the set of weights
	edges.insert(newEdge);										//Runtime log(# of E already inserted)
	weights.insert(pair<directedEdge, int>(newEdge, weight));	//Runtime log(# of E already inserted)
																//Aggregate runtime E*log(E)
	return;
}


int Graph::getNumVertices()
{
	return vertices.size();
}


int Graph::getWeight(directedEdge edge){

	map<directedEdge, int>::iterator it2;
	it2 = weights.find(edge);					//Runtime log(E)

	//Make sure the edge exists
	if(it2 == weights.end()){
		cout << "ERROR getWeight" << endl;
		return -1;
	}
	//Return weight if found
	else{
		return it2->second;
	}
}


bool Graph::isEdge(directedEdge newEdge){
	
	set<directedEdge>::iterator edge;
	edge = edges.find(newEdge);

	if(edge == edges.end()){
		return false;
	}
	else{
		return true;
	}
}


void Graph::print()

{
	set<int>::iterator vertexIt;
	set<directedEdge>::iterator edgeIt;
	map<directedEdge, int>::iterator weightIt;

	for(vertexIt = vertices.begin(); vertexIt != vertices.end(); vertexIt++){
		cout << *vertexIt << ":";
		for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++){
			if(edgeIt->first == *vertexIt){
				weightIt = weights.find(*edgeIt);
				cout << " " << edgeIt->second << " (" << weightIt->second << ")";
			}
		}
		cout << endl;
	}
	return;
}


void Graph::generateGraph(string fileName){
	ifstream file(fileName);
	int numVertices;
	int numEdges;
	string text;

	file >> numVertices;
	file >> numEdges;
	getline(file, text);

	for(int i = 0; i < numVertices; i++){
		addVertex();
	}

	int source;
	int dest;
	int weight;

	directedEdge newEdge;
	
	while(!file.eof()){
		file >> source;
		file >> dest;
		file >> weight;

		newEdge = directedEdge(source, dest);
		addEdge(newEdge, weight);
	}
	
	return;	
}

/*

We start out by creating 2 arrays that store the distance from the source to all the vertices, and the total number of shortest paths.

We used -1 to initialize the distance array since integers do not support infinity, and the graph is guaranteed to be non-negative.
We also initialized the paths array to 0.

We used a list instead of a queue to store the unchecked vertices so that we can remove from any part of the list.
At the start, we add every vertex to the list except the source vertex, which will be handled as a special case.

For the body of the algorithm, iterate through the list of vertices, and pick the one with the shortest distance from the source.
For the selected vertex, iterate through all of the edges to look for ones where the starting vertex is the selected vertex, and
update the distances of the destination vertex if the distance to the starting vertex added to the weight of the edge is less than the distance to the destination vertex.
If the two values are equal, increment the paths counter in the paths array for the destination vertex.
There are special cases since we used -1 for infinity, but that is explained more in the comments in the code.

The algorithm is done when the list of vertices is empty.

*/

void Graph::modifiedDijkstra(int source)

{

	//Solution arrays
	int distance[getNumVertices()];
	int paths[getNumVertices()];


	list<int>q;		//Queue implemented as a list, since I have to search for the vertex with minimum distance anyways
	list<int>::iterator qIt;
	set<int>::iterator vertIt;
	set<directedEdge>::iterator edgeIt;

	
	//Initializing solution arrays and filling list with vertices
	for(int i = 0; i < getNumVertices(); i++){
		distance[i] = -1;							//Using -1 as infinity since we can assume no negative weights
		paths[i] = 0;
		if(i != source){
			q.push_back(i);							//List contains all vertices except source
		}
	}
	distance[source] = 0;


	//Special case for source node
	//Automatically update minimum distance since every other vertex is "infinity" 
	for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++)
	{
		if(edgeIt->first == source){
			distance[edgeIt->second] = getWeight(*edgeIt);
			paths[edgeIt->second]++;
		}
	}


	int minDist;						//Stores the distance of the vertex closest to the source 
	list<int>::iterator minIndex;		//Stores the position of the closest vertex in the list and also the ID of the vertex itself

	while(!q.empty()){
		//Initialize values to the first vertex in the list
		qIt = q.begin();
		minDist = distance[*qIt];
		minIndex = qIt;
		
		//Search for the vertex closest to the source		
		for(qIt = q.begin(); qIt != q.end(); qIt++){
			//Ignore if distance is infinity
			//Since values are initialized to the first vertex in the list, there will always be a selected vertex, even if all distances are infinity
			if(distance[*qIt] == -1){
				continue;
			}
			//Automatically update the closest vertex if the current closest vertex is infinitely far
			//Since we ignore vertices infinitely far in the if-condition above, the new closest vertex cannot possibly be infinity 
			if(minDist == -1){
				minDist = distance[*qIt];
				minIndex = qIt;
			}
			//Update closest vertex if a new closest vertex is found
			else if(distance[*qIt] < minDist){
				minDist = distance[*qIt];
				minIndex = qIt;
			}
		}


		//Search all edges for edges that contain the selected vertex as the starting vertex
		for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++){
			if(edgeIt->first == *minIndex){
				//Automatically update distance and paths counter if destination vertex is infinitely far
				if(distance[edgeIt->second] == -1){
					distance[edgeIt->second] = distance[edgeIt->first] + getWeight(*edgeIt);
					paths[edgeIt->second]++;
				}
				//Update distance if the path through the selected vertex is shorter
				//Reset the paths counter because a new shortest path is found
				else if(distance[edgeIt->first] + getWeight(*edgeIt) < distance[edgeIt->second]){
					distance[edgeIt->second] = distance[edgeIt->first] + getWeight(*edgeIt);
					paths[edgeIt->second] = 1;
				}
				//Increment paths counter if the path through the selected vertex is equally as the current shortest path
				else if(distance[edgeIt->first] + getWeight(*edgeIt) == distance[edgeIt->second]){
					paths[edgeIt->second]++;
				}
			}

		}
		
		//Remove vertex from list
		q.erase(minIndex);
	}

	cout << endl << "Shortest paths from node " << source << ":" << endl;
	for(int i = 1; i < getNumVertices(); i++){
		cout << "Distance to vertex " << i << " is " << distance[i] << " and there are " << paths[i] << " shortest paths" << endl;
	}

	return;
}

