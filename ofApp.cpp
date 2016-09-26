#include "ofApp.h"

//--------------------------------------------------------------
void Player::setup(string s, float t = ofGetElapsedTimef(), string n = "Player"){
    
    //Set player side
    if (s == "left")
        side = "left";
    else if (s == "right")
        side = "right";
    
    //Set player name
    name = n;
    
    //Begin in ready state
    state = "ready";
    
    //Set HP
    hp = 10;
    
    //Load sprites
    ofImage img;

    img.load(side + "1.png");
    sprite.push_back(img);
    
    img.load(side + "2.png");
    sprite.push_back(img);
    
    img.load(side + "3.png");
    sprite.push_back(img);
    
    img.load(side + "4.png");
    sprite.push_back(img);
    
    img.load(side + "5.png");
    sprite.push_back(img);
    
    img.load(side + "6.png");
    sprite.push_back(img);
    
    img.load(side + "7.png");
    sprite.push_back(img);

    spawnTime = t;
    spriteStartTime = t;
    
    spriteSetter = true;
    
    //Load hit sound
    sound.load("swordsound.mp3");
    
}

//--------------------------------------------------------------
void Player::update(){
    
    //Update sprite animation timer
    timerSprite = ofGetElapsedTimef() - spriteStartTime;
    
    //Rotate between animated sprites
    if (timerSprite > .5) {
        if (spriteSetter == true)
            spriteSetter = false;
        else
            spriteSetter = true;
        spriteStartTime = ofGetElapsedTimef();
    }
    
    //Check for state changes
    if (state == "hurt") {
        timerHurt = ofGetElapsedTimef() - hurtStartTime;
        if (timerHurt > .5) {
            if ( hp < 1)
                state = "lose";
            else {
                state = "recover";
                recoverStartTime = ofGetElapsedTimef();
            }
        }
    }
    
    else if (state == "recover") {
        timerRecover = ofGetElapsedTimef() - recoverStartTime;
        if (timerRecover > 1.5) {
            state = "ready";
        }
    }
    
}



//--------------------------------------------------------------
void Player::draw(int w, int h) {
    
    //Set true x
    int x = w - (sprite[0].getWidth()/2);
    
    int width = 1024;
    
    //Set sprite location
    if (side == "left") {
        ofSetColor(0);
        ofDrawRectangle(55, 55, 420, 40);
        ofSetColor(255);
        if (hp < 4)
            ofSetColor(209, 2, 36);
        ofDrawRectangle(65, 65, 40*hp, 20);
        ofSetColor(255);
    }
    
    else {
        ofSetColor(0);
        ofDrawRectangle((width - 55), 55, -420, 40);
        ofSetColor(255);
        if (hp < 4)
            ofSetColor(209, 2, 36);
        ofDrawRectangle((width - 65), 65, -40*hp, 20);
        ofSetColor(255);
    }
    
    
    //Draw sprite for corresponding state
    if (state == "ready") {
        if (spriteSetter == true)
            sprite[0].draw(x,h);
        else
            sprite[1].draw(x,h);
    }
    
    else if (state == "hurt")
        sprite[2].draw(x,h);
    
    else if (state == "recover") {
        ofSetColor(255, 255, 255, 50);
        if (spriteSetter == true)
            sprite[0].draw(x,h);
        else
            sprite[1].draw(x,h);
        ofSetColor(255, 255, 255, 255);
    }
    
    else if (state == "lose") {
        if (spriteSetter == true)
            sprite[3].draw(x,h);
        else
            sprite[4].draw(x,h);
    }
    
    else if (state == "win") {
        if (spriteSetter == true)
            sprite[5].draw(x,h);
        else
            sprite[6].draw(x,h);
    }
    
}

//--------------------------------------------------------------
void Player::applyStrike(){
    
    //If in ready state, apply hit based on serial reading
    if (state == "ready") {
        state = "hurt";
        hurtStartTime = ofGetElapsedTimef();
        hp--;
        sound.play();
        
    }
    
}

//--------------------------------------------------------------
bool Player::isLoser(){
    //return if player has lost
    if (state == "lose")
        return true;
    else
        return false;
}

//--------------------------------------------------------------
void Player::setWin(){
    //Set the player to win state
    state = "win";
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    width = 1024;
    height = 768;
    
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    
    serial.setup("/dev/tty.usbserial-MWEUN2H", 115200);
    serial.startContinuousRead();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    
    message = "";
    
    background.load("background.png");
    
    float time = ofGetElapsedTimef();
    
    playerRight.setup("right", time, "Player 2");
    playerLeft.setup("left", time, "Player 1");
    
    font.load("joystix monospace.ttf", 32);
}

//--------------------------------------------------------------
void ofApp::onNewMessage(string & message)
{
   // if (message.size() > 0)
        cout << "onNewMessage, message: " << message[34] << "\n";

    if (message[34] == '1')
        playerLeft.applyStrike();
    
    else if (message[34] == '3')
        playerRight.applyStrike();
    
    
}
    
//--------------------------------------------------------------
void ofApp::update(){
    playerLeft.update();
    playerRight.update();
    
    if (playerLeft.isLoser())
        playerRight.setWin();
    
    else if (playerRight.isLoser())
        playerLeft.setWin();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    background.ofBaseDraws::draw(0, 0);
    playerLeft.draw(width/4, (height/2) - 100);
    playerRight.draw((width*3)/4, (height/2) - 100);
    ofSetColor(0, 0, 0);
    ofDrawRectangle(0, 565, width, height);
    ofSetColor(255, 255, 255);
    font.drawString(playerLeft.name, width/6 - 50, height - 85);
    font.drawString(playerRight.name, (4*width)/6 - 70, height - 85);
    
    if (playerLeft.isLoser())
        font.drawString(playerRight.name + " wins!", width/3 - 85, height/3 - 50);
    else if (playerRight.isLoser())
        font.drawString(playerLeft.name + " wins!", width/3 - 85, height/3 - 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    this->setup();


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
