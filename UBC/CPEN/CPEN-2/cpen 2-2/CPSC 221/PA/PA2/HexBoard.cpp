#include "HexBoard.h"

#include <iostream>

#include "queue.h"
#include "stack.h"

HexBoard::HexBoard(int radius) {
    /**
     * @todo Your code here!
     */

    this->radius = radius;
    this->start_coord = make_pair(0,0);
    this->longest_path_dist = 0;
    create_board();
}

void HexBoard::create_board() {
    /**
     * @todo Your code here!
     */

    for(int q = -(radius-1); q < radius; q++){
        int r1 = std::max(-(radius-1), -q-(radius-1));
        int r2 = std::min(radius-1, -q + (radius-1));

        for(int r = r1; r <= r2; r++){
            pair<int,int> coord;
            coord.first = q;
            coord.second = r;
            this->cells[coord] = new HexCell(q,r);
        }
    }
    configure_cells();
}

void HexBoard::configure_cells() {
    /**
     * @todo Your code here!
     */

    for(auto& cell : cells){
        vector<pair<int, int>> neigh = get_neigh_coords(make_pair(cell.second->q, cell.second->r));

        for(int i = 0; i < 6; i++){
            auto find = this->cells.find(neigh[i]);
            if(find == cells.end()){
                cell.second->neighbours[i] = nullptr;
            }
            else{
                cell.second->neighbours[i] = find->second;
                if(i < 3){
                    find->second->neighbours[i + 3] = cell.second;
                }
                else{
                    find->second->neighbours[i % 3] = cell.second;
                }
            }
        }
    }
}

void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    /**
     * @todo Your code here!
     */

    Stack<HexCell*> cell_stack;
    this->branch_prob = branch_prob;
    this->start_coord = start_coord;
    auto it =  this->cells.find(start_coord);
    cell_stack.push(it->second);
    it->second->path_dist_from_start = 0;
    
    map<pair<int,int>, bool> visited;
    visited[start_coord] = true;

    int longpath = 0;

    while(!cell_stack.isEmpty()){
        HexCell *curr_cell = cell_stack.peek();
        HexCell *curr_neighbour = chooseNeighbour(curr_cell, visited, branch_prob);



        if(curr_neighbour != nullptr){
            curr_cell->link(curr_neighbour);
            visited[make_pair(curr_neighbour->q,curr_neighbour->r)] = true;
            curr_neighbour->path_dist_from_start = curr_cell->path_dist_from_start + 1;

            if(curr_neighbour->path_dist_from_start > longpath && curr_neighbour->is_edge()){
                longpath  = curr_neighbour->path_dist_from_start;
                end_coord = {curr_neighbour->q, curr_neighbour->r};
            }

            if(curr_neighbour->path_dist_from_start > longest_path_dist){
                longest_path_dist = curr_neighbour->path_dist_from_start;
            }
            cell_stack.push(curr_neighbour);
        }
        else{
            cell_stack.pop();
        }
        
    }
}

//solution  == explored
void HexBoard::solve_maze() {
    /**
     * @todo Your code here!
     */
    HexCell *start_cell = cells[start_coord];
    vector<pair<int, int>> sol_sol;
    solve_maze_help(start_cell, sol_sol);

    
}

void HexBoard::solve_maze_help(HexCell *curr, vector<pair<int, int>> path){
    

    if(curr != nullptr){
        pair<int, int> curr_coord = {curr->q, curr->r};
    
        path.push_back(curr_coord);
        visited.push_back(curr_coord);
        HexCell *current_cell = this->cells[curr_coord];

        if(curr_coord == end_coord){
            this->solution = path;
        }


        for(auto& cell : current_cell->links){ //cells in links 
            if(cell != nullptr){
                pair<int, int> cell_coordinate = {cell->q,cell->r};
                if(std::find(visited.begin(), visited.end(), cell_coordinate) == visited.end()){
                    solve_maze_help(cell, path);
                }

            }
        }
    }
    
}