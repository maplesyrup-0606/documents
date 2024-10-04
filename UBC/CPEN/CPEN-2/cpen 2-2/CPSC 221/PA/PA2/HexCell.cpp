#include "HexCell.h"

#include <cmath>

HexCell::HexCell(int Q, int R) {
    /**
     * @todo Your code here!
     */
    q = Q;
    r = R;

    for(int i = 0; i < 6; i++){
        links[i] = nullptr;
        neighbours[i] = nullptr;
    }

    path_dist_from_start = 0;
}

void HexCell::link(HexCell* cell) {
    /**
     * @todo Your code here!
     */

    if(cell == nullptr) return;
    
    int cell_q = cell->q;
    int cell_r = cell->r;

    if(cell_q == q && cell_r == r-1){ //N
        neighbours[N] = cell;
        cell->neighbours[3+N] = this;

        links[N] = cell;
        cell->links[3+N] = this;
    }
    else if(cell_q == q + 1 && cell_r == r - 1){ //NE
        neighbours[NE] = cell;
        cell->neighbours[3+NE] = this;

        links[NE] = cell;
        cell->links[3+NE] = this;
    }
    else if(cell_q == q + 1 && cell_r == r){ //SE
        neighbours[SE] = cell;
        cell->neighbours[3+SE] = this;

        links[SE] = cell;
        cell->links[3+SE] = this;
    }
    else if(cell_q == q && cell_r == r + 1){ //S
        neighbours[S] = cell;
        cell->neighbours[S%3] = this;

        links[S] = cell;
        cell->links[S%3] = this;
    }
    else if(cell_q == q - 1 && cell_r == r + 1){ //SW
        neighbours[SW] = cell;
        cell->neighbours[SW%3] = this;

        links[SW] = cell;
        cell->links[SW%3] = this;
    }
    else if(cell_q == q - 1 && cell_r == r){ //NW
        neighbours[NW] = cell;
        cell->neighbours[NW%3] = this;

        links[NW] = cell;
        cell->links[NW%3] = this; 
    }
    else return;
}

int HexCell::get_neighbour_side(const HexCell* cell) const {
    /**
     * @todo Your code here! You will need to replace the following line.
     */

    
    int cell_q = cell->q;
    int cell_r = cell->r;

    if(cell_q == q && cell_r == r-1){ //N
        return N;
    }
    else if(cell_q == q + 1 && cell_r == r - 1){ //NE
        return NE;
    }
    else if(cell_q == q + 1 && cell_r == r){ //SE
        return SE;
    }
    else if(cell_q == q && cell_r == r + 1){ //S
        return S;
    }
    else if(cell_q == q - 1 && cell_r == r + 1){ //SW
        return SW;
    }
    else if(cell_q == q - 1 && cell_r == r){ //NW
        return NW;
    }
    else return 300;
}

bool HexCell::is_linked(const HexCell* cell) const {
    /**
     * @todo Your code here! You will need to replace the following line.
     */
    if(cell == nullptr) return false;
    for(auto link_s : links){
        if(link_s == cell){
            return true;
        }
    }
    return false;
}

int HexCell::distance(const HexCell* cell) const {
    /**
     * @todo Your code here! You will need to replace the following line.
     */
    if(cell == nullptr) return -1;
    int cell_q = cell->q;
    int cell_r = cell->r;

    int distance = (abs(q - cell_q) + abs(r - cell_r) + abs(q + r - cell_q - cell_r))/2;
    
    return distance;
}

bool HexCell::is_edge() const {
    /**
     * @todo Your code here! You will need to replace the following line.
     */

    for(auto n : neighbours){
        if(n == nullptr) return true;
    }
    return false;
}