#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {
    
    queue<Node*> q;
    q.push(node);
    vector<Node*> leaf;

    while (!q.empty()) {
        int level_size = q.size();
        leaf.clear(); // clear previous level values

        for (int i = 0; i < level_size; i++) {
            Node* temp = q.front();
            q.pop();
            if (temp->NW != NULL) {
                q.push(temp->NW);
            }
            if (temp->NE != NULL) {
                q.push(temp->NE);
            }
            if (temp->SW != NULL) {
                q.push(temp->SW);
            }
            if (temp->SE != NULL) {
                q.push(temp->SE);
            }
            leaf.push_back(temp);
        }
    }
    
    RGBAPixel* average = &node->avg;
    
    int rAve = average->r;
    int gAve = average->g;
    int bAve = average->b;
    for(Node* pix : leaf){
        RGBAPixel* curr = &pix->avg;
        
        int r = curr->r;
        int g = curr->g;
        int b = curr->b;

        double distance = pow((rAve - r),2) + pow((gAve - g),2) + pow((bAve - b),2);
        
        if(distance > tol){   
            return false;
        }
    }
    return true;
}
