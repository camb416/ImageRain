//
//  GalleryImage.h
//  ImageRain
//
//  Created by Cameron Browning on 7/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ImageRain_GalleryImage_h
#define ImageRain_GalleryImage_h

#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <string.h>
#include "cinder/Timeline.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum { TOP_LEFT,TOP_CENTER,TOP_RIGHT,MIDDLE_LEFT,CENTER,MIDDLE_RIGHT,BOTTOM_LEFT,BOTTOM_CENTER,BOTTOM_RIGHT };


class GalleryImage : public gl::Texture {
public:
    Anim<float> x;
    Anim<float> y;
    
    Anim<float> scale;
    
    float z;
    float hOffset, vOffset;
    char align;
    gl::Texture tex;
    string path;
    GalleryImage(string _path=NULL, char _align=4);
    ~GalleryImage();
    void update();
    void draw();
    // void load(string _path);
    
    
};
GalleryImage::~GalleryImage(){
    
}
GalleryImage::GalleryImage(string _path, char _align){
    scale = 1.0f;
    x = y = z;
    align = _align;
    path = _path;
    tex = gl::Texture(loadImage(loadUrl("http://25.media.tumblr.com/tumblr_m7c1drS9zB1qd0obwo1_500.jpg")));
   // tex = gl::Texture(loadImage(loadResource(path)));
    
    if(align<3){
        vOffset = 0;
    } else if(align<6){
        vOffset = -0.5f*tex.getHeight();
    } else {
        vOffset = -1*tex.getHeight();
    }
    if(align%3==0){
        hOffset = 0;
    } else if(align%3==1){
        hOffset = -0.5f*tex.getWidth();
    } else {
        hOffset = -1*tex.getWidth();
    }
    
}
void GalleryImage::update(){
    // nothing to see here
}
void GalleryImage::draw(){

    gl::color(ColorA(255,255,255,0.5f));
    gl::pushMatrices();
    gl::translate(x,y);

    // gl::color(ColorA((float)i/(float)numImages/2.0f+0.5f,0.75f,0.25f));
    Rectf r = Rectf(-0.5f*tex.getWidth()*scale,-0.5f*tex.getHeight()*scale,scale*tex.getWidth(),scale*tex.getHeight());
    gl::draw( tex, r);
    gl::popMatrices();

}

#endif
