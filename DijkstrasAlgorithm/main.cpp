
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "node.h"
using namespace std;

void connect(int first, int second, int length, vector<Node*> node_collection);
bool weightCompare(Node* first, Node* second);

int main() {

    int numnodes = 0;
    cout << "Enter the number of nodes you will input: ";
    cin >> numnodes;
    if (numnodes <= 0)
    {
        cout << "Invalid number of nodees or you have entered an empty graph. Finding the shortest path would be meaningless."<<endl;
        return 1;
    }
    vector<Node*> node_collection(numnodes);
    for (int i = 0; i<numnodes; i++)
        node_collection[i]= new Node(i);
    
    
    
    
    for(int i=0; i<numnodes; i++)
    {
        cout<<"Enter number of nodes connected to node "<<i+1<<". You may discount a node if you input the connection previously : ";
        int connectedNodes = 0;
        cin >> connectedNodes;
        if (connectedNodes>0)
            cout<<"Enter the node number of the connected node spaced with it's length from the current node and hit enter to input the next connected node."<<endl;
        
        //load the network of nodes
        for(int j=0; j<connectedNodes; j++)
        {
            int nodeID, len;
            cin >> nodeID;
            cin >> len;
            if (nodeID > numnodes)
            {
                cout << "You have entered a node number that is more than the number of specified nodes.\n";
                for (int i = 0; i< numnodes; i++)
                    delete node_collection[i];
                return 1;
            }
            nodeID--;            //ID number in our vector is one less than what the user would interpret
            connect(i, nodeID, len, node_collection); //this ensures that user can enter the same connection once or twice
        }
    }
    
    
    int startNodeID, endNodeID;
    cout << "Nodes loaded. Enter the starting node number and ending node number separated by a space"<<endl;
    cin >> startNodeID;
    cin >> endNodeID;
    if (startNodeID > numnodes || endNodeID > numnodes || startNodeID < 1 || endNodeID < 1)
    {
        cout << "One of the node numbers that you have provided do not exist in the graph. Please try again."<<endl;
        for (int i = 0; i< numnodes; i++)
            delete node_collection[i];
        return 1;
    }
    
    
    //user interpretation for node ID will be one more than our implementation
    --startNodeID;
    --endNodeID;
    Node* startptr = node_collection[startNodeID];
    Node* endptr = node_collection[endNodeID];
    Node* currptr = startptr;
    vector<Node*> perm_node_collection = node_collection;
    startptr->updateWeight(0, nullptr);                  //starting point is given 0 weight for Dijkstra's algorithm
    sort(node_collection.begin(), node_collection.end(), weightCompare);
    
    int len = (int) node_collection.size();
    while (currptr!=endptr && len>0)
    {
        currptr->setAdjNodeWeights();           //updates the weights of the adjacent nodes and removes itself from the network
        node_collection[0] = node_collection[node_collection.size()-1];
        node_collection.pop_back();
        --len;
        if (len>0)
        {
            sort(node_collection.begin(), node_collection.end(), weightCompare);
            currptr = node_collection[0];       //since the collection is sorted by ascending weights, the first index will be what we will consider for every iteration
        }
    }
    
    int minlen = currptr->getWeight();
    
    if (!endptr->wasUpdated())
    {
        cout << "Looks like there is no valid path between the starting and ending node. Please try again."<<endl;
        for (int i=0; i<numnodes; i++)
            delete perm_node_collection[i];
        return 1;
    }
    
    cout <<endl<< "The shortest path length is " <<minlen<< endl;
    cout << "The path taken would be: ";
    currptr = endptr;
    stack<int> path;
    while (currptr->getPrevious()!=nullptr)
    {
        path.push(currptr->getID());
        currptr = currptr->getPrevious();
    }
    path.push(currptr->getID());
    
    while(!path.empty())
    {
        cout<<path.top()+1<<" ";
        path.pop();
    }
    cout<<endl;
    for (int i=0; i<numnodes; i++)
        delete perm_node_collection[i];
    

}

void connect(int first, int second, int length, vector<Node*> node_collection)
{
    if (first < 0 || second < 0 || first >= node_collection.size() || second >= node_collection.size())
        return;
    node_collection[first]->insertOrUpdateAdjNode(node_collection[second], length);
    node_collection[second]->insertOrUpdateAdjNode(node_collection[first], length);
}

bool weightCompare(Node* first, Node* second)
{
    return first->getWeight() < second->getWeight();
}
