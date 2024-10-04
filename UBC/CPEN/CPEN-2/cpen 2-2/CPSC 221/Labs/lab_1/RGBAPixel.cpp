#include "RGBAPixel.h"
namespace cs221util {
    RGBAPixel::RGBAPixel(){
        r = 255;
        g = 255;
        b = 255;
        a = 1.0;
    } 

    RGBAPixel::RGBAPixel(unsigned char r_, unsigned char g_, unsigned char b_){
        r = r_;
        g = g_;
        b = b_;
        a = 1.0;
    }

    RGBAPixel::RGBAPixel(unsigned char r_, unsigned char g_, unsigned char b_, double a_){
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
}