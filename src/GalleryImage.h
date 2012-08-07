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
#include "Vector.h"
#include "cinder/Timeline.h"


using namespace ci;
using namespace ci::app;
using namespace std;

enum { TOP_LEFT,TOP_CENTER,TOP_RIGHT,MIDDLE_LEFT,CENTER,MIDDLE_RIGHT,BOTTOM_LEFT,BOTTOM_CENTER,BOTTOM_RIGHT };


class GalleryImage : public gl::Texture {
public:
    Anim<float> x;
    Anim<float> y;
    Anim<float> a;
    Anim<float> scale;
    Anim<float> rot;
    
    Anim<float> z;
    float hOffset, vOffset;
    char align;
    gl::Texture tex;
    gl::Texture back;
    gl::Texture foreground;
    string path;
    GalleryImage(string _path=NULL, char _align = CENTER);
    ~GalleryImage();
    GalleryImage(Texture _tex,  char _align = CENTER);
    GalleryImage(Texture front, Texture _back, char _align = CENTER);
    
    void rotate(int deg);
    void update();
    void draw();
    // void load(string _path);
    
private:
    float scaleShadow();
    
    
};
GalleryImage::~GalleryImage(){
    
}
GalleryImage::GalleryImage(string _path, char _align){
    scale = 1.0f;
    x = y = z;
    align = _align;
    path = _path;
  //   tex = gl::Texture(loadImage(loadUrl("http://25.media.tumblr.com/tumblr_m7c1drS9zB1qd0obwo1_500.jpg")));
   tex = gl::Texture(loadImage(loadResource(path)));
    
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
    foreground = loadImage(loadResource("postcard_foreground.png"));
}

GalleryImage::GalleryImage(Texture _tex,  char _align)
{
    scale = 1.0f;
    x = y = z;
    align = _align;
    tex = _tex;
    
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
    foreground = loadImage(loadResource("postcard_foreground.png"));
}

GalleryImage::GalleryImage(Texture front, Texture _back, char _align)
{
    scale = 1.0f;
    x = y = z;
    align = _align;
    tex = front;
    back = _back;
    
    
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
    foreground = loadImage(loadResource("postcard_foreground.png"));
}

void GalleryImage::update(){
    // nothing to see here
}
void GalleryImage::draw(){
    float s = scaleShadow();
   // console() << s << endl;
    gl::color(ColorA(s, s, s, a));
    gl::pushMatrices();
    gl::translate(x,y,-z);
    gl::enableAlphaBlending();
    gl::rotate(Vec3f(0, rot, 0));
    Rectf r = Rectf(0, 0, 500, 350);                                
    //gl::translate(-tex.getWidth()/2.0, -tex.getHeight()/2.0);         // translate by r width Or tex width
    gl::translate(-r.getWidth()/2.0, -tex.getHeight()/2.0);

    gl::draw( tex, r );
    if (back)  {
        gl::pushMatrices();
        gl::translate(0,0,-1);
        gl::draw( back );
        gl::popMatrices();
    }
    
    gl::pushMatrices();
    gl::translate(0,0,1);
    gl::draw(foreground, r);
    gl::popMatrices();
    
    gl::popMatrices();

}

void GalleryImage::rotate(int deg)
{
//    rot += deg;

    timeline().appendTo(&rot, rot + deg, 1.0f);
}

float GalleryImage::scaleShadow()
{
    return 1.0f - (z / 1000.0f * 1.0f);
}

#endif
