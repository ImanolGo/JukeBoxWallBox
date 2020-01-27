/*
 *  JukeboxPlayerApp.cpp
 *  Jukebox Player App
 *
 *  Created by Imanol Gomez on 01/10/18.
 *
 */



#include "AppManager.h"

#include "JukeboxPlayerApp.h"

//--------------------------------------------------------------
void JukeboxPlayerApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void JukeboxPlayerApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void JukeboxPlayerApp::draw(){
    AppManager::getInstance().draw();
}

void JukeboxPlayerApp::exit()
{
    ofLogNotice() <<"JukeboxPlayerApp::exit";

}

//--------------------------------------------------------------
void JukeboxPlayerApp::keyPressed(int key){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::keyReleased(int key){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void JukeboxPlayerApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void JukeboxPlayerApp::dragEvent(ofDragInfo dragInfo){

}
