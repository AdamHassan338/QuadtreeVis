#include "node.h"

Node::Node(QRect bounds, int capacity, Node* parent)
{

    m_bounds = bounds;
    m_capacity = capacity;
    m_parent = parent;
    if(!parent){
        m_currentDepth = 0;
    }else{
        m_currentDepth = parent->m_currentDepth + 1;
    }

}


bool Node::insert(Circle* circle)
{
    if (!m_bounds.contains(circle->pos)) {
        //this->prune(point);
        return false;

    }
    //else
    if (m_circles.size() < m_capacity && m_children.isEmpty()) {
        m_circles.push_back(circle);
        return true;
    }
    //else
    if (m_children.isEmpty()) {
        subdivide();
    }

    for (Node* child : m_children) {
        if (child->insert(circle)) {
            return true;
        }
    }

    return false;
}

Node *Node::find(Circle* circle)
{
    if (!m_bounds.contains(circle->pos)) {
        return nullptr;
        }

    if (m_children.isEmpty()) {
        return this;
    }

    for (Node* child : m_children) {
        Node* resultNode = child->find(circle);
        if(resultNode){
            return resultNode;
        }
    }
    return nullptr;
}

Node *Node::findPos(QPoint pos)
{
    if (!m_bounds.contains(pos)) {
        return nullptr;
    }

    if (m_children.isEmpty()) {
        return this;
    }

    for (Node* child : m_children) {
        Node* resultNode = child->findPos(pos);
        if(resultNode){
            return resultNode;
        }
    }
    return nullptr;
}

Node* Node::search(QRect rect)
{
    if (!m_bounds.intersects(rect)) {
        return nullptr;
    }
}

Node *Node::getNeighbour(quadrant quad)
{
    switch(quad){
    case one:
        break;
    case two:
        break;
    case three:
        break;
    case four:
        break;
    }

    return nullptr;
}

Node *Node::getBigNeighbour(direction direction)
{
    Node* node;
    Node* northNeighbor;
    switch(direction){
    case N:
        //if root node
        if(!m_parent){
            return nullptr;
        }
        //if sw child
        if(m_parent->m_children.at(3)==this){
            //return nw sibbling
            return m_parent->m_children.at(0);
        }
        //if se child
        if(m_parent->m_children.at(2)==this){
            //return ne sibbling
            return m_parent->m_children.at(1);
        }
        node = m_parent->getBigNeighbour(direction);
        if(!node || node->m_children.isEmpty()){
            return node;
        }
        //if not retuned yet we are a north child
        if(m_parent->m_children.at(0)==this){
            return node->m_children.at(3);
        }else{
            node->m_children.at(2);
        }

        break;

    case E:
        if(!m_parent){
            return nullptr;
        }

        //if nw child
        if(m_parent->m_children.at(0)==this){
            //return ne sibbling
            return m_parent->m_children.at(1);
        }
        //if sw child
        if(m_parent->m_children.at(3)==this){
            //return se sibbling
            return m_parent->m_children.at(2);
        }
        node = m_parent->getBigNeighbour(direction);
        if(!node || node->m_children.isEmpty()){
            return node;
        }
        //if not retuned yet we are a east child
        if(m_parent->m_children.at(1)==this){
            return node->m_children.at(0);
        }else{
            node->m_children.at(2);
        }
        break;

    case S:

        //if root node
        if(!m_parent){
            return nullptr;
        }
        //if nw child
        if(m_parent->m_children.at(0)==this){
            //return sw sibbling
            return m_parent->m_children.at(3);
        }
        //if ne child
        if(m_parent->m_children.at(1)==this){
            //return se sibbling
            return m_parent->m_children.at(2);
        }
        node = m_parent->getBigNeighbour(direction);
        if(!node || node->m_children.isEmpty()){
            return node;
        }
        //if not retuned yet we are a south child ?????????????
        if(m_parent->m_children.at(3)==this){
            return node->m_children.at(0);
        }else{
            node->m_children.at(1);
        }

        break;

    case W:
        if(!m_parent){
            return nullptr;
        }

        //if ne child
        if(m_parent->m_children.at(1)==this){
            //return nw sibbling
            return m_parent->m_children.at(0);
        }
        //if se child
        if(m_parent->m_children.at(2)==this){
            //return sw sibbling
            return m_parent->m_children.at(3);
        }
        node = m_parent->getBigNeighbour(direction);
        if(!node || node->m_children.isEmpty()){
            return node;
        }
        //if not retuned yet we are a west child
        if(m_parent->m_children.at(0)==this){
            return node->m_children.at(1);
        }else{
            node->m_children.at(2);
        }
        break;

    case NW:
        northNeighbor = getBigNeighbour(N);
        if(northNeighbor) {
            return northNeighbor->getBigNeighbour(W);
        }
        break;
    case NE:
        northNeighbor = getBigNeighbour(N);
        if(northNeighbor) {
            return northNeighbor->getBigNeighbour(E);
        }
        break;

    case SW:
        northNeighbor = getBigNeighbour(S);
        if(northNeighbor) {
            return northNeighbor->getBigNeighbour(W);
        }
        break;
    case SE:
        northNeighbor = getBigNeighbour(S);
        if(northNeighbor) {
            return northNeighbor->getBigNeighbour(E);
        }
        break;


    default:
        break;
    }

    return nullptr;
}

QVector<Node *> Node::getSmallNeighbour(Node* neighbor, direction direction)
{
    QVector<Node *> candidates;
    if(neighbor){
        candidates.push_back(neighbor);
    }
    QVector<Node *> neigbors;


    switch(direction){
    case N:
        while(!candidates.isEmpty()){
            //if not leaf node add children to candiates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            }else{
                candidates.push_back(candidates.at(0)->m_children.at(3));
                candidates.push_back(candidates.at(0)->m_children.at(2));
            }
            candidates.removeAt(0);
        }
        break;
    case E:
        while(!candidates.isEmpty()){
            //if not leaf node add children to candiates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            }else{
                candidates.push_back(candidates.at(0)->m_children.at(0));
                candidates.push_back(candidates.at(0)->m_children.at(3));
            }
            candidates.removeAt(0);
        }
        break;
    case S:
        while(!candidates.isEmpty()){
            //if not leaf node add children to candiates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            }else{
                candidates.push_back(candidates.at(0)->m_children.at(0));
                candidates.push_back(candidates.at(0)->m_children.at(1));
            }
            candidates.removeAt(0);
        }
        break;
    case W:
        while(!candidates.isEmpty()){
            //if not leaf node add children to candiates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            }else{
                candidates.push_back(candidates.at(0)->m_children.at(1));
                candidates.push_back(candidates.at(0)->m_children.at(2));
            }
            candidates.removeAt(0);
        }
        break;
    case NW:
        while(!candidates.isEmpty()){
            // if not a leaf node, add children to candidates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            } else {
                candidates.push_back(candidates.at(0)->m_children.at(0));  // NW child
            }
            candidates.removeAt(0);
        }
        break;
    case NE:
        while(!candidates.isEmpty()){
            // if not a leaf node, add children to candidates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            } else {
                candidates.push_back(candidates.at(0)->m_children.at(1));  // NE child
            }
            candidates.removeAt(0);
        }
        break;
    case SW:
        while(!candidates.isEmpty()){
            // if not a leaf node, add children to candidates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            } else {
                candidates.push_back(candidates.at(0)->m_children.at(3));  // SW child
            }
            candidates.removeAt(0);
        }
        break;
    case SE:
        while(!candidates.isEmpty()){
            // if not a leaf node, add children to candidates
            if(candidates.at(0)->m_children.empty()){
                neigbors.push_back(candidates.at(0));
            } else {
                candidates.push_back(candidates.at(0)->m_children.at(2));  // SE child
            }
            candidates.removeAt(0);
        }
        break;

    }


    return neigbors;
}

QVector<Node *> Node::getNeighboringNodes()
{
    QVector<Node*> neighbors;

    Node* neighbor = getBigNeighbour(N);
    neighbors.append(getSmallNeighbour(neighbor, N));

    neighbor = getBigNeighbour(E);
    neighbors.append(getSmallNeighbour(neighbor, E));

    neighbor = getBigNeighbour(NW);
    neighbors.append(getSmallNeighbour(neighbor, NW));

    neighbor = getBigNeighbour(NE);
    neighbors.append(getSmallNeighbour(neighbor, NE));

    neighbor = getBigNeighbour(SW);
    neighbors.append(getSmallNeighbour(neighbor, SW));

    neighbor = getBigNeighbour(SE);
    neighbors.append(getSmallNeighbour(neighbor, SE));

    return neighbors;
}

void Node::subdivide()
{
    if (m_currentDepth >= MAX_DEPTH) {
        return;  // Don't subdivide if the maximum depth is reached
    }

    int x = m_bounds.x();
    int y = m_bounds.y();
    int halfWidth = m_bounds.width() / 2;
    int halfHeight = m_bounds.height() / 2;
    int childDepth = m_currentDepth +1;

    //quadrant 1 #0
    m_children.push_back(new Node(QRect(x + halfWidth, y, halfWidth, halfHeight), m_capacity,this));
    //quadrant 2 #1
    m_children.push_back(new Node(QRect(x, y, halfWidth, halfHeight), m_capacity,this));
    //quadrant 3 #2
    m_children.push_back(new Node(QRect(x, y + halfHeight, halfWidth, halfHeight), m_capacity,this));
    //quadrant 4 #3
    m_children.push_back(new Node(QRect(x + halfWidth, y + halfHeight, halfWidth, halfHeight), m_capacity,this));



    for (Circle* circle : m_circles) {
        for (Node* child : m_children) {
            if (child->insert(circle)) {
                break;
            }
        }
    }

    m_circles.clear();
}

void Node::prune(const QPoint &cursor)
{
    if (!m_bounds.contains(cursor)) {
        for (Node* child : m_children) {  // Delete the child nodes
            delete child;
        }
        m_children.clear();
        return;
    }

   // for (Node* child : children) {
        //child->prune(cursor);
        //delete child;
    //}
    //children.clear();
    //return;
}

void Node::resize(QRect newBounds)
{
    m_bounds = newBounds;   //new bounds

    //recalucate childrens bounds
    int x = m_bounds.x();
    int y = m_bounds.y();
    int halfWidth = m_bounds.width() / 2;
    int halfHeight = m_bounds.height() / 2;

    if (m_children.size() == 4) {
        m_children[0]->resize(QRect(x, y, halfWidth, halfHeight));
        m_children[1]->resize(QRect(x + halfWidth, y, halfWidth, halfHeight));
        m_children[2]->resize(QRect(x, y + halfHeight, halfWidth, halfHeight));
        m_children[3]->resize(QRect(x + halfWidth, y + halfHeight, halfWidth, halfHeight));
    }
}

Node::~Node()
{
    for (Node* child : m_children) {
        delete child;
    }
    m_children.clear();
}

