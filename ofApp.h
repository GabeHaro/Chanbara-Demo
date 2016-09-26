#pragma once

#include "ofMain.h"
#include "ofxSimpleSerial.h"

class Player {
    public:
        void setup(string, float, string);
        void update();
        void draw(int, int);
        void applyStrike();
    
        string state;
        string name;
        string side;
    
        float spawnTime;
    
        float spriteStartTime;
        float timerSprite;
    
        float hurtStartTime;
        float timerHurt;
    
        float recoverStartTime;
        float timerRecover;
    
        float hp;
    
        bool spriteSetter;
    
        vector<ofImage> sprite;
    
        ofSoundPlayer sound;
    
        bool isLoser();
    
        void setWin();
    
    
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        string message;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void onNewMessage(string & message);
    
        ofxSimpleSerial serial;
    
        vector<bool> katana;
		
        int width;
        int height;
    
        ofImage background;
    
        Player playerLeft;
        Player playerRight;
    
        ofTrueTypeFont font;

};
