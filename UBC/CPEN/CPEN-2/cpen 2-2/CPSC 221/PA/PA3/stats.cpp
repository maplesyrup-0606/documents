
#include "stats.h"

stats::stats(PNG& im) {
    int width = im.width();
    int height = im.height();
    sumRed.resize(width, vector<long>(height));
    sumGreen.resize(width, vector<long>(height));
    sumBlue.resize(width, vector<long>(height));
    sumsqRed.resize(width, vector<long>(height));
    sumsqGreen.resize(width, vector<long>(height));
    sumsqBlue.resize(width, vector<long>(height));
    

    for(int x =0 ; x < width; x++){
        for(int y = 0; y < height; y++){
            RGBAPixel* current = im.getPixel(x, y);
            if(x == 0 && y ==0 ){
                sumRed[x][y] = current->r;
                sumGreen[x][y] = current->g;
                sumBlue[x][y] = current->b;
                
                sumsqRed[x][y] = ((current->r)*(current->r));
                sumsqGreen[x][y] = ((current->g)*(current->g));
                sumsqBlue[x][y] =((current->b)*(current->b));
            }
            else if(x == 0 && y != 0){
                sumRed[x][y] = current->r + sumRed[x][y-1];
                sumGreen[x][y] = current->g + sumGreen[x][y-1];
                sumBlue[x][y] = current->b + sumBlue[x][y-1];

                sumsqRed[x][y] = ((current->r)*(current->r)) + sumsqRed[x][y-1];
                sumsqGreen[x][y] = ((current->g)*(current->g)) + sumsqGreen[x][y-1];
                sumsqBlue[x][y] = ((current->b)*(current->b)) + sumsqBlue[x][y-1];
            }
            else if(x != 0 && y == 0){
                sumRed[x][y] = current->r + sumRed[x-1][y];
                sumGreen[x][y] = current->g + sumGreen[x-1][y];
                sumBlue[x][y] = current->b + sumBlue[x-1][y];

                sumsqRed[x][y] = ((current->r)*(current->r)) + sumsqRed[x-1][y];
                sumsqGreen[x][y] = ((current->g)*(current->g)) + sumsqGreen[x-1][y];
                sumsqBlue[x][y] = ((current->b)*(current->b)) + sumsqBlue[x-1][y];
            }
            else{
                sumRed[x][y] = current->r + sumRed[x-1][y] + sumRed[x][y-1] - sumRed[x-1][y-1];
                sumGreen[x][y] = current->g + sumGreen[x-1][y] + sumGreen[x][y-1] - sumGreen[x-1][y-1];
                sumBlue[x][y] = current->b + sumBlue[x-1][y] + sumBlue[x][y-1] - sumBlue[x-1][y-1];

                sumsqRed[x][y] = ((current->r)*(current->r)) + sumsqRed[x-1][y] + sumsqRed[x][y-1] - sumsqRed[x-1][y-1];
                sumsqGreen[x][y] = ((current->g)*(current->g)) + sumsqGreen[x-1][y] + sumsqGreen[x][y-1] - sumsqGreen[x-1][y-1];
                sumsqBlue[x][y] = ((current->b)*(current->b)) + sumsqBlue[x-1][y] + sumsqBlue[x][y-1] - sumsqBlue[x-1][y-1];
                // we subtract the left diagonal pixel, becuase the one above and the one to the left
                // already has the left diagonal value 
            }
        }
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    
    long sumR, sumG, sumB;
    int side = (int)pow(2.0, (double)dim) -1;

    if(ul.first == 0 && ul.second == 0){    
        sumR = sumRed[side][side];
        sumG = sumGreen[side][side];
        sumB = sumBlue[side][side];
    }
    else if(ul.first == 0 && ul.second != 0){
        sumR = sumRed[ul.first + side][ul.second + side] - sumRed[side][ul.second-1];
        sumG = sumGreen[ul.first + side][ul.second + side] - sumGreen[side][ul.second-1];
        sumB = sumBlue[ul.first + side][ul.second + side] - sumBlue[side][ul.second-1];
    }
    else if(ul.first != 0 && ul.second == 0){
        sumR = sumRed[ul.first + side][ul.second + side] - sumRed[ul.first - 1][side];
        sumG = sumGreen[ul.first + side][ul.second + side] - sumGreen[ul.first - 1][side];
        sumB = sumBlue[ul.first + side][ul.second + side] - sumBlue[ul.first - 1][side];
    }
    else{
        sumR = sumRed[ul.first+side][ul.second+side] - sumRed[ul.first + side][ul.second-1] - sumRed[ul.first-1][ul.second+side] + sumRed[ul.first-1][ul.second-1];
        sumG = sumGreen[ul.first+side][ul.second+side] - sumGreen[ul.first + side][ul.second-1] - sumGreen[ul.first-1][ul.second+side] + sumGreen[ul.first-1][ul.second-1];
        sumB = sumBlue[ul.first+side][ul.second+side] - sumBlue[ul.first + side][ul.second-1] - sumBlue[ul.first-1][ul.second+side] + sumBlue[ul.first-1][ul.second-1];
    }
    
    switch(channel){
        case 'r':
            return sumR;
            break;
        case 'g' :
            return sumG;
            break;
        default:
            return sumB;
            break;
    }
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    long sumR, sumG, sumB;

    int side = (int)pow(2.0, (double)dim) -1 ;
    if(ul.first == 0 && ul.second == 0){
        sumR = sumsqRed[side][side];
        sumG = sumsqGreen[side][side];
        sumB = sumsqBlue[side][side];
    }
    else if(ul.first == 0 && ul.second != 0){
        sumR = sumsqRed[ul.first + side][ul.second + side] - sumsqRed[side][ul.second-1];
        sumG = sumsqGreen[ul.first + side][ul.second + side] - sumsqGreen[side][ul.second-1];
        sumB = sumsqBlue[ul.first + side][ul.second + side] - sumsqBlue[side][ul.second-1];
    }
    else if(ul.first != 0 && ul.second == 0){
        sumR = sumsqRed[ul.first + side][ul.second + side] - sumsqRed[ul.first - 1][side];
        sumG = sumsqGreen[ul.first + side][ul.second + side] - sumsqGreen[ul.first - 1][side];
        sumB = sumsqBlue[ul.first + side][ul.second + side] - sumsqBlue[ul.first - 1][side];
    }
    else{
        sumR = sumsqRed[ul.first+side][ul.second+side] - sumsqRed[ul.first + side][ul.second-1] - sumsqRed[ul.first-1][ul.second+side] + sumsqRed[ul.first-1][ul.second-1];
        sumG = sumsqGreen[ul.first+side][ul.second+side] - sumsqGreen[ul.first + side][ul.second-1] - sumsqGreen[ul.first-1][ul.second+side] + sumsqGreen[ul.first-1][ul.second-1];
        sumB = sumsqBlue[ul.first+side][ul.second+side] - sumsqBlue[ul.first + side][ul.second-1] - sumsqBlue[ul.first-1][ul.second+side] + sumsqBlue[ul.first-1][ul.second-1];
    }
    
    switch(channel){
        case 'r':
            return sumR;
            break;
        case 'g' :
            return sumG;
            break;
        default:
            return sumB;
            break;
    }
}

long stats::rectArea(int dim) {
    return (long)pow(pow(2.0, (double)dim), 2);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    return (getSumSq('r',ul,dim) + getSumSq('g',ul,dim) + getSumSq('b',ul,dim)) - (((int)pow(getSum('r',ul,dim), 2)+(int)pow(getSum('g',ul,dim), 2)+(int)pow(getSum('b',ul,dim), 2))/rectArea(dim));
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    long a = rectArea(dim);
    long red = getSum('r', ul, dim) / a;
    long green = getSum('g', ul, dim) / a;
    long blue = getSum('b', ul, dim) / a;
    return RGBAPixel(red, green, blue);
}
