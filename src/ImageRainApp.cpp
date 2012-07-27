#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageRainApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    gl::Texture		mTexture;	
    vector<gl::Texture> textures;
    int frameCount;
    int state;
    int numStates;
    void tweenApp();
    void keyDown(KeyEvent event);
    int numImages;

};

void ImageRainApp::setup()
{
    numImages = 1024;
    numStates = 2;
    state = -1;
    for(int i=0;i<numImages;i++){
        std::ostringstream oss;
        oss << "img_sml" << (i%100+1) << ".jpg";
    try {
		//fs::path path = getOpenFilePath( "", ImageIo::getLoadExtensions() );
        
        // if( ! path.empty() ) {
        //mTexture = gl::Texture( loadImage( path ) );
        
       
      //  std::cout << oss.str();
      //  gl::Texture thisTexture = gl::Texture(loadImage(loadResource("bar_clean.png")));
        
         gl::Texture thisTexture = gl::Texture(loadImage(loadResource(oss.str())));
        textures.push_back(thisTexture);
        //  }
	}
	catch( ... ) {
		console() << "unable to load the texture file at " << oss.str() << std::endl;
	}
    }
    state = 0;
}

void ImageRainApp::mouseDown( MouseEvent event )
{
    state ++;
    if(state>numStates-1){
        state = 0;
    }
}
void ImageRainApp::keyDown(cinder::app::KeyEvent event){
  //  tweenImages();
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
            case '1':
            for(int i=0;i<numImages;i++){
                gl::Texture thisTexture = textures.at(0);
                gl::pushMatrices();
                gl::translate(i*10+sin((float)i/100.0f*3.14*2)*300, i*10);
                gl::rotate(i+sin((float)frameCount/2/10.0f)*90);
                if( thisTexture )
                {
                    gl::color(255,255,255,0.5f);
                    gl::draw( thisTexture, Vec2f(-0.5f*thisTexture.getWidth(),-0.5f*thisTexture.getHeight()) );
                }
                gl::popMatrices();
            }
            break;
        default:
	for(int i=0;i<numImages;i++){
    gl::Texture thisTexture = textures.at(i);
        gl::pushMatrices();
        gl::translate(i+sin((float)i/100.0f*3.14*2)*300, getWindowHeight()/2+cos((float)i/50.0f)*i);
        gl::rotate((float)i/10.0f+sin((float)frameCount/2/10.0f)*90);
        if( thisTexture )
		{
            gl::color(ColorA(255,255,255,0.5f));
          // gl::color(ColorA((float)i/(float)numImages/2.0f+0.5f,0.75f,0.25f));
            gl::draw( thisTexture, Vec2f(-0.5f*thisTexture.getWidth(),-0.5f*thisTexture.getHeight()) );
        }
            gl::popMatrices();
    }
    }
}


CINDER_APP_BASIC( ImageRainApp, RendererGl )
