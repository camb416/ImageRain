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
    //gl::Texture		mTexture;	
    vector<GalleryImage> textures;
    int frameCount;
    int state;
    int numStates;
    void tweenApp();
    void keyDown(KeyEvent event);
    int numImages;
    
    XmlTree doc;
    
};

void ImageRainApp::setup()
{
    
    numImages = 100;
    numStates = 2;
    state = -1;
    
    
    // XmlTree doc( loadUrl( "http://amnhnyc.tumblr.com/rss" ) );
    

    for(int i=0;i<numImages;i++){
        std::ostringstream oss;
        oss << "img_sml" << (i+1) << ".jpg";
        try {
            //fs::path path = getOpenFilePath( "", ImageIo::getLoadExtensions() );
            
            // if( ! path.empty() ) {
            //mTexture = gl::Texture( loadImage( path ) );
            
            
            //  std::cout << oss.str();
            //  gl::Texture thisTexture = gl::Texture(loadImage(loadResource("bar_clean.png")));
            
            // gl::Texture thisTexture = gl::Texture(loadImage(loadResource(oss.str())));
            GalleryImage gImage = GalleryImage(oss.str(),CENTER);
            textures.push_back(gImage);
            //  }
        }
        catch( ... ) {
            console() << "unable to load the texture file at " << oss.str() << std::endl;
        }
    }
    state = 0;
    
    
    
}
void ImageRainApp::parseXML()
{    
    try {
        XmlTree doc(loadUrl( "http://amnhnyc.tumblr.com/rss" ) );
       // cout << doc.
        // XmlTree areas = doc.getChild("project");
      //  background = areas.getAttributeValue<string>("background");
      //  mTexture = gl::Texture( loadImage( loadResource( background ) ) );
        /*
        for( XmlTree::Iter area = areas.begin(); area!= areas.end(); ++area )
        {
            XmlTree a = *area;
        //    PhidgetConnector *pc = &pConnector;
          //  mAreas.push_back( UserArea(a, pc) ) ;
        } */
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
    /*
     state ++;
     if(state>numStates-1){
     state = 0;
     }*/
    for(int i=0;i<numImages;i++){
        //   gl::Texture thisTexture = textures.at(i).tex;
        //    gl::pushMatrices();
        //   gl::rotate((float)i/10.0f+sin((float)frameCount/2/10.0f)*90);
        GalleryImage * gImagePtr = &textures.at(i);
        //   gImagePtr->x =(i%10*100);
        //   gImagePtr->y = floor(i/10)*100;
        
        //    Anim<float> f = 0.0f;
        //   float d = 1000.0f;
        timeline().apply(&gImagePtr->scale,1.0f,1.0f,EaseOutQuad());
        timeline().apply(&gImagePtr->x,(float)(i%10*100),(float)i/50.0f,EaseOutElastic(2.0f,(float)i+1.0f));
        timeline().apply(&gImagePtr->y,(float)floor(i/10)*100,(float)i/25.0f,EaseOutElastic((float)i+1.0f,1.0f));
        //  timeline().appendTo(&f,d,2.0f);
        // timeline().apply(&f,d,2.0f);
        //        gImagePtr->draw();
        //      gl::popMatrices();
    }
    
}
void ImageRainApp::mouseUp( MouseEvent event )
{
    /*
     state ++;
     if(state>numStates-1){
     state = 0;
     }*/
    for(int i=0;i<numImages;i++){
        GalleryImage * gImagePtr = &textures.at(i);
        timeline().apply(&gImagePtr->scale,0.1f,1.0f,EaseOutQuad());
        timeline().apply(&gImagePtr->x,getWindowCenter().x,(float)i/50.0f,EaseOutElastic((float)i+1.0f,10.0f));
        timeline().apply(&gImagePtr->y,getWindowCenter().y,2.0f-(float)i/50.0f,EaseOutElastic(101.0f-(float)i,1.0f));
     }
    
}


void ImageRainApp::keyDown(cinder::app::KeyEvent event){
    // toggle fullscreen
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
    
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) ); 
    gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
	gl::enableAlphaBlending();
    
    
    switch(state){
            
        default:
            for(int i=0;i<numImages;i++){
             //   gl::Texture thisTexture = textures.at(i).tex;
             //   gl::pushMatrices();
            //    gl::translate(i%10*100,floor(i/10)*100);
             //   gl::rotate((float)i/10.0f+sin((float)frameCount/2/10.0f)*90);
                GalleryImage * gImagePtr = &textures.at(i);
                gImagePtr->draw();
             //    gl::popMatrices();
            }
               
            }
    
}


CINDER_APP_BASIC( ImageRainApp, RendererGl )
