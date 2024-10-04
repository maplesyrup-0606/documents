#include "block.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

int Block::height() const {
  if(data.size() == 0) return 0;
  return this->data[0].size();
}

int Block::width() const {
  /* your code here */
  return this->data.size();
}

void Block::render(PNG &im, int x) const {
  im.resize(im.width(), height());
  for(unsigned int y = 0; y < height(); y++){
    for(unsigned int i = x; i < x + width(); i++){
      *im.getPixel(i,y) = this->data[i-x][y];
    }
  }
}
void Block::build(PNG &img, int x, int width) {
  for( unsigned int i = x ;i < x + width; i++){
    vector<HSLAPixel> vertic;
    for( unsigned int y = 0; y < img.height(); y++){
      HSLAPixel *pixel = img.getPixel(i,y);
      vertic.push_back(*pixel);
    }
    this->data.push_back(vertic);
  }
}