
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    stats t  = stats(imIn);
    pair<int,int> ul = make_pair(0,0);
    edge = pow(2, (int)log2 (imIn.width()));
    int dim = log2 (edge);
    root = new Node(ul, dim , t.getAvg(ul, dim), t.getVar(ul, dim));
    int dimS = edge/2;
    if(dimS > 0){
        root->NW = buildTree(t, ul, dimS);
        root->NE = buildTree(t, make_pair(ul.first + dimS, ul.second), dimS);
        root->SW = buildTree(t, make_pair(ul.first, ul.second + dimS ), dimS);
        root->SE = buildTree(t, make_pair(ul.first + dimS , ul.second + dimS ), dimS);
    }

}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    Node* temp = new Node(ul, log2 (dim) , s.getAvg(ul, log2 (dim)), s.getVar(ul, log2 (dim)));
    int dimS = dim/2;
    if(dimS == 0){
        return temp;
    }
    else{
        temp->NW = buildTree(s, ul, dimS);
        temp->NE = buildTree(s, make_pair(ul.first + dimS , ul.second), dimS);
        temp->SW = buildTree(s, make_pair(ul.first, ul.second + dimS), dimS);
        temp->SE = buildTree(s, make_pair(ul.first + dimS , ul.second + dimS ), dimS);
        return temp;
    }
}

PNG quadtree::render() const {
    PNG out = PNG(edge, edge);
    if(root == NULL){return out;}
    stack<Node*> s;
    
    s.push(root);
    while (!s.empty()) {
        Node* curr = s.top();
        s.pop();

        bool isLeaf = true;
        if (curr->NW != NULL){
            s.push(curr->NW);
            isLeaf = false;
        }
        if (curr->NE != NULL){
            s.push(curr->NE);
            isLeaf = false;
        } 
        if (curr->SW != NULL){
            s.push(curr->SW);
            isLeaf = false;
        }
        if (curr->SE != NULL) {
            s.push(curr->SE);
            isLeaf = false;
        }

        if (isLeaf) {
            for(int x = 0; x < pow(2, curr->dim); x++){
                for (int y = 0; y < pow(2, curr->dim); y++){
                    *out.getPixel(curr->upLeft.first + x, curr->upLeft.second + y) = curr->avg;
                }
            }
        }
    }
    return out;
}

// int quadtree::idealPrune(const int leaves) const {
//     int max_tol = (int) (pow(255, 2) + pow(255, 2) + pow(255, 2));
//     int min_tol = 0;

//     int low = min_tol;
//     int hi = max_tol;
//     std::cout << "Prunesize(3865) " << pruneSize(3865) << endl;
//     int result = 0;
//     while(low <= hi){
//         int mid = (hi + low) / 2;
//         std::cout << mid << " " << pruneSize(mid) << endl;
//         if(pruneSize(mid) <= leaves && pruneSize(mid) >= leaves-1){
//             result = mid;
//             break;
//         }
//         else if(pruneSize(mid) > leaves ){
//             low = mid + 1;
//         }
//         else{
//             hi = mid - 1;
//         }
//     }

//     bool is_done = false;
//     while(!is_done){
//         if(pruneSize(result -1) <= leaves){
//             result--;
//         }
//         else{
//             is_done = true;
//         }
//     }
//     std::cout << result << endl;

//     return result;
// }

int quadtree::idealPrune(const int leaves) const {
    // const int max_tol = 3 * 255 * 255;
    // const int min_tol = 0;

    // int low = min_tol;
    // int hi = max_tol;
    // int result = 0;

    // while (low <= hi) {
    //     int mid = low + (hi - low) / 2; 
    //     int size = pruneSize(mid);

    //     if (size <= leaves && size >= leaves - 10) {
    //         result = mid;
    //         break;
    //     } else if (size > leaves) {
    //         low = mid + 1;
    //     } else {
    //         hi = mid - 1;
    //     }
    // }

    // // No need for a second loop, just decrement result until the condition is no longer satisfied
    // while (result > 0 && pruneSize(result - 1) <= leaves) {
    //     result--;
    // }

    // return result;
    const int max_tol = 3 * 255 * 255;
    const int min_tol = 0;

    int low = min_tol;
    int hi = max_tol;
    int result = max_tol;

    while (low <= hi) {
        int mid = low + (hi - low) / 2;
        int size = pruneSize(mid);

        if (size <= leaves) {
            result = mid;
            hi = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}

int quadtree::pruneSize(const int tol) const {
    vector<Node*>  coreleaf;
    vector<Node*>  *leaf = & coreleaf;
    pruneSize(root, tol, leaf);
    return leaf->size();
}

void quadtree::pruneSize(Node * node , const int tol, vector<Node*> * leaf) const {
    if(node == NULL){
        return;
    }
    else if (prunable(node, tol)){
        leaf->push_back(node);
    }
    else{
        pruneSize(node->NW, tol, leaf);
        pruneSize(node->NE, tol, leaf);
        pruneSize(node->SW, tol, leaf);
        pruneSize(node->SE, tol, leaf);
    }
}

void quadtree::prune(const int tol) {
    prune(root , tol);
}

void quadtree::prune(Node* node, const int tol){
    if(node == NULL){
        return;
    }
    else if (prunable(node, tol)){
        clear_helper(node->NW);
        clear_helper(node->NE);
        clear_helper(node->SW);
        clear_helper(node->SE);
        node->NW = NULL;
        node->NE = NULL;
        node->SW = NULL;
        node->SE = NULL;
    }
    else{
        prune(node->NW, tol);
        prune(node->NE, tol);
        prune(node->SW, tol);
        prune(node->SE, tol);
    }
}

void quadtree::clear() {
    clear_helper(this->root);
    delete root;
}   

void quadtree::clear_helper(Node *curr){
    if(curr == NULL) return ;
    else{
        clear_helper(curr->NW);
        delete curr->NW;
        clear_helper(curr->NE);
        delete curr->NE;
        clear_helper(curr->SE);
        delete curr->SE;
        clear_helper(curr->SW);
        delete curr->SW;
    }
}

void quadtree::copy(const quadtree& orig) {
    Node * oroot = orig.root;
    edge = orig.edge;
    root = new Node(oroot->upLeft, oroot->dim, oroot->avg, oroot->var);
    copySupport(root, orig.root);

}

void quadtree::copySupport(Node *  before, Node * orig){
    if(orig != NULL){
        Node* rootNW = orig->NW;
        Node* rootNE = orig->NE;
        Node* rootSW = orig->SW;
        Node* rootSE = orig->SE;

        if(rootNW != NULL){
            Node * NW = new Node(rootNW->upLeft, rootNW->dim, rootNW->avg, rootNW->var);
            before->NW = NW;
            copySupport(NW , rootNW);
        }
        if(rootNE != NULL){
            Node * NE = new Node(rootNE->upLeft, rootNE->dim, rootNE->avg, rootNE->var);
            before->NE = NE;
            copySupport(NE , rootNE);
        }
        if(rootSW != NULL){
            Node * SW = new Node(rootSW->upLeft, rootSW->dim, rootSW->avg, rootSW->var);
            before->SW = SW;
            copySupport(SW , rootSW);
        }
        if(rootSE != NULL){
            Node * SE = new Node(rootSE->upLeft, rootSE->dim, rootSE->avg, rootSE->var);
            before->SE = SE;
            copySupport(SE , rootSE);
        }
    }
    return;
}

