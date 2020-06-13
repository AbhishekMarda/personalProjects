//
//  node.hpp
//  DijkstrasAlgorithm
//
//  Created by Abhishek Marda on 6/8/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#ifndef node_h
#define node_h
#include <vector>
#include <unordered_map>
#include <iterator>
class Node
{
public:
    Node();
    Node(int ID);
    
    void updateWeight(int weight, Node* updator);
    void setID(int ID);
    void noteUpdate();
    
    int getWeight() const;
    int getNodeNum() const;
    Node* getPrevious() const;
//    Node* getAdjNode(int ID) const;
    int getID() const;
    bool wasUpdated() const;
    
    
    void removeNode(int ID);
    void insertOrUpdateAdjNode(Node* newnode, int length);
    void setAdjNodeWeights();
private:
    std::unordered_map<Node*, int> m_adjacentNodes;
    int m_nodeID;
    int m_weight;
    int m_numAdjacentNodes;
    bool m_wasUpdated;
    Node* m_previous;
    
};


#endif /* node_h */
