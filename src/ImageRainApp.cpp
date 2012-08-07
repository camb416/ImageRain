#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "GalleryImage.h"
#include <sstream>
#include <math.h>
#include "cinder/Timeline.h"
#include "cinder/Easing.h"
#include "cinder/Xml.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class ImageRainApp : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
    void mouseUp(MouseEvent event);
	void update();
	void draw();
    void prepareSettings(Settings *settings);
    void parseXML();
    vector<GalleryImage> images;
    int frameCount;
    int state;
    int numStates;
    void tweenApp();
    void keyDown(KeyEvent event);
    int numImages;
    
    void loadResources();
    void loadRSS();
    void loadTumblr();
    
    void grid();
    void stack();
    void scatter();
    void zScatter();
    void yRotate();
    
    XmlTree doc;
    
};

void ImageRainApp::setup()
{
    images.clear();
    numStates = 2;
    state = -1;
    srand(time (NULL) );
    
    //loadTumblr();
    loadRSS();
    state = 1;   
    zScatter();
}

void ImageRainApp::loadResources()
{
    numImages = 100;
    for(int i=0;i<numImages;i++){
        std::ostringstream oss;
        oss << "img_sml" << (i+1) << ".jpg";
        try {
            GalleryImage gImage = GalleryImage(oss.str(),CENTER);
            images.push_back(gImage);
        }
        catch( ... ) {
            console() << "unable to load the texture file at " << oss.str() << std::endl;
        }
    }
}

void ImageRainApp::loadRSS()
{
    // Instagram Settings
    XmlTree doc = XmlTree( loadUrl( "http://widget.stagram.com/rss/n/justinbieber" ) );
    XmlTree rss = doc.getChild("rss/channel");
    
    for ( XmlTree::Iter item = rss.begin(); item != rss.end(); ++item)
    {
        if( item->getTag() == "item" )
        {
            string url = item->getChild("image/url").getValue();
            gl::Texture front = gl::Texture(loadImage(loadUrl(url)));
            gl::Texture back = gl::Texture(loadImage(loadUrl( "http://distilleryimage9.s3.amazonaws.com/e257cc92a1e611e181bd12313817987b_6.jpg")));
            
            GalleryImage gImage = GalleryImage(front);
            images.push_back(gImage);
        }
    }   
    numImages = images.size();
}

void ImageRainApp::loadTumblr()
{
    // XmlTree doc = XmlTree( loadUrl( "http://amnhnyc.tumblr.com/rss" ) );
    
    XmlTree doc = XmlTree( loadUrl( "http://amnhnyc.tumblr.com/rss" ) );
    XmlTree rss = doc.getChild("rss/channel");
    string url;
    int i = 0;
    for ( XmlTree::Iter item = rss.begin(); item != rss.end(); ++item)
    {
        console() << i << endl;

        if( item->getTag() == "item" )
        {
            url = item->getChild("description").getValue();
            int pos = url.find("http");
            int end = url.find(".jpg");
            if (end == -1)
                end = url.find(".png");
            if (end == -1)
                break;
            url = url.substr(pos, end - 6);
            console() << url << endl;    
            gl::Texture tex = gl::Texture(loadImage(loadUrl(url)));
            console() << "img loaded" << endl;
            GalleryImage gImage = GalleryImage(tex, tex);
            images.push_back(gImage);
        }
        i++;
    }

    numImages = images.size();
}

void ImageRainApp::parseXML()
{    
    try {
        XmlTree doc(loadUrl( "http://amnhnyc.tumblr.com/rss" ) );
    }
    catch ( ... ) {
        console() << "Unable to load XML document. Check for syntax errors." << endl;
    }
}

void ImageRainApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1760.0, 960.0 );
	settings->setFrameRate( 60.0f );
}

void ImageRainApp::mouseDown( MouseEvent event )
{
    switch (state) {
        case 1:
            zScatter();
            break;
            
        default:
            stack();
            break;
    }
}
void ImageRainApp::mouseUp( MouseEvent event )
{
    switch (state) {     
        case 1:
       //     scatter();
            break;
        default:
            grid();
            break;
    }
}

void ImageRainApp::yRotate()
{
    for (int i = 0; i < numImages; i++)
    {
        GalleryImage * gImagePtr = &images.at(i);
        float rotModifier = 180.0f;
        if(i%2==0) rotModifier*=-1.0f;
      //  timeline().apply(&gImagePtr->rot, gImagePtr->rot + rotModifier,2.0f,EaseOutQuad());
        gImagePtr->rotate(180);
    }
}

void ImageRainApp::scatter()
{
    for (int i = 0; i < numImages; i++)
    {
        GalleryImage * gImagePtr = &images.at(i);
        int x = rand() % getWindowWidth();
        int y = rand() % getWindowHeight();
        float s = (rand() % 2) / 2.0f + 0.5f;
        console() << s << endl;
        timeline().apply(&gImagePtr->scale,.5f,1.0f,EaseOutQuad());
        timeline().apply(&gImagePtr->x,(float)(x), 2.0f, EaseOutQuad());
        timeline().apply(&gImagePtr->y,(float)(y), 2.0f, EaseOutQuad());
        timeline().apply(&gImagePtr->scale, s, 2.0f, EaseOutQuad());
    }
}

void ImageRainApp::zScatter()
{    for (int i = 0; i < numImages; i++)
    {
        GalleryImage * gImagePtr = &images.at(i);
        
        int x = rand() % getWindowWidth();
        int y = rand() % getWindowHeight();
        int z = rand() % 1000;
        //timeline().apply(&gImagePtr->scale,.5f,1.0f,EaseOutQuad());
        if (gImagePtr->a() > 0.75f)
            timeline().apply(&gImagePtr->a, .5f, .50f, EaseInQuad());
        
        timeline().apply(&gImagePtr->x,(float)(x), 2.0f, EaseOutQuad()).appendTo(&gImagePtr->a);
        timeline().apply(&gImagePtr->y,(float)(y), 2.0f, EaseOutQuad()).appendTo(&gImagePtr->a);
        timeline().apply(&gImagePtr->z, (float)(z), 2.0f, EaseOutQuad()).appendTo(&gImagePtr->a);
        timeline().appendTo(&gImagePtr->a, 1.0f, .5f, EaseOutQuad()).appendTo(&gImagePtr->x);
        
    }
}

void ImageRainApp::stack()
{
    for(int i=0;i<numImages;i++){
        GalleryImage * gImagePtr = &images.at(i);
        timeline().apply(&gImagePtr->scale,0.1f,1.0f,EaseOutQuad());
        timeline().apply(&gImagePtr->x,getWindowCenter().x,(float)i/50.0f,EaseOutElastic((float)i+1.0f,10.0f));
        timeline().apply(&gImagePtr->y,getWindowCenter().y,2.0f-(float)i/50.0f,EaseOutElastic(101.0f-(float)i,1.0f));
    }
}

void ImageRainApp::grid()
{
    for(int i=0;i<numImages;i++){
        GalleryImage * gImagePtr = &images.at(i);
        
        timeline().apply(&gImagePtr->scale,1.0f,1.0f,EaseOutQuad());
        timeline().apply(&gImagePtr->x,(float)(i%10*100),(float)i/50.0f,EaseOutElastic(2.0f,(float)i+1.0f));
        timeline().apply(&gImagePtr->y,(float)floor(i/10)*100,(float)i/25.0f,EaseOutElastic((float)i+1.0f,1.0f));
    }
}

void ImageRainApp::keyDown(cinder::app::KeyEvent event){
    if (event.getChar() == 'r')
        yRotate();
    else if (event.getChar() == 's')
        setup();
    else
        setFullScreen(!isFullScreen());
}
void ImageRainApp::tweenApp(){
    
    // nothing to see here.
}

void ImageRainApp::update()
{
    frameCount++;
}

void ImageRainApp::draw()
{
    gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
	gl::enableAlphaBlending();
    gl::enableDepthRead();

    switch(state){
            
        default:
            for(int i=0;i<numImages;i++){
                GalleryImage * gImagePtr = &images.at(i);
                gImagePtr->draw();
            }
    }
}


CINDER_APP_BASIC( ImageRainApp, RendererGl )
