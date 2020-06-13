

#include "node.h"

using namespace std;

Node::Node()
{
    m_nodeID = 0;
    m_weight = INT_MAX;
    m_numAdjacentNodes = 0;
    m_wasUpdated=false;
    m_previous = nullptr;
}

Node::Node(int ID)
{
    m_nodeID=ID;
    m_weight = INT_MAX;
    m_numAdjacentNodes = (int)m_adjacentNodes.size();
    m_wasUpdated=false;
    m_previous = nullptr;
}

void Node::updateWeight(int weight, Node* updator)
{
    if (weight < m_weight && weight>=0)
    {
        m_weight = weight;
        m_previous = updator;
    }
}

int Node::getWeight() const
{
    return m_weight;
}


int Node::getNodeNum() const
{
    return m_numAdjacentNodes;
}

inline
int Node::getID() const
{
    return m_nodeID;
}


bool Node::wasUpdated() const
{
    return m_wasUpdated;
}

Node* Node::getPrevious() const
{
    return m_previous;
}

void Node::setID(int ID)
{
    m_nodeID = ID;
}

void Node::noteUpdate()
{
    m_wasUpdated = true;
}

void Node::insertOrUpdateAdjNode(Node* newnode, int length)
{
    if (newnode == nullptr || length<0)
        return;
    unordered_map<Node*, int>::iterator it;
    it = m_adjacentNodes.find(newnode);
    if (it == m_adjacentNodes.end())
    {
        m_adjacentNodes[newnode]=length;
        m_numAdjacentNodes++;
    }
    else
        if (it->second > length)
            it->second = length;
}

void Node::removeNode(int ID)
{
    unordered_map<Node*, int>::iterator it = m_adjacentNodes.begin();
    for (; it!=m_adjacentNodes.end(); it++)
    {
        if (it->first->getID() == ID)
        {
            m_adjacentNodes.erase(it);
            m_numAdjacentNodes--;
            return;
        }
    }
    
}

void Node::setAdjNodeWeights()
{
    unordered_map<Node*, int>::iterator it = m_adjacentNodes.begin();
    for (;it!=m_adjacentNodes.end(); it++)
    {
        Node *ptr = it->first;
        ptr->updateWeight(m_weight+it->second, this);
        ptr->removeNode(m_nodeID);
        ptr->noteUpdate();
    }
}
