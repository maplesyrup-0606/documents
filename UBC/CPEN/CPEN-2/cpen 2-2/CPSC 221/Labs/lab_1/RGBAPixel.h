#ifndef CS221UTIL_RGBAPixel_H
#define CS221UTIL_RGBAPixel_H

#include <string>
#include <vector>
#include "RGBAPixel.h"

using namespace std;

namespace cs221util {
    class RGBAPixel {
        public:
            RGBAPixel();

            RGBAPixel(unsigned char r, unsigned char g, unsigned char b);
            
            RGBAPixel(unsigned char r, unsigned char g, unsigned char b,
            double a);
        // private:
            unsigned char r;
            unsigned char g;
            unsigned char b;
            double a;
        private: 
    };
}

#endif