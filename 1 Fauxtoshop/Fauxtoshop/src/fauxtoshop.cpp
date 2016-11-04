/*
 * fauxtoshop.cpp
 * Start code from Standford CS106X (Authum 2015) Assignment 1
 * Rewritten by Ching_Shing, junior student in SJTU
 * CS106B/X C++ style guideline adopted (ref:http://stanford.edu/class/archive/cs/cs106b/cs106b.1158/styleguide.shtml)
 *
 *
 * This file is to generate a photo editor.
 *
 */

#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"

using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const short    OPEN_FILE = 0;
static const short    SAVE_FILE = 1;

static bool     openImageFromFilename(GBufferedImage &img, string filename);
static bool 	saveImageToFilename(const GBufferedImage &img, string filename);
static void     getMouseClickLocation(int &row, int &col);
static void     quitOrNotInputCheck(short mode, bool &quitFlag, string &filename);
static void     fauxtoshop(const GBufferedImage &img, GBufferedImage &dst, int choice);
static void     scatter(const GBufferedImage &img, GBufferedImage &dst);
static void     edgeDetection(const GBufferedImage &img, GBufferedImage &dst);
static void     greenScreen(const GBufferedImage &img, GBufferedImage &dst);
static void     compare(const GBufferedImage &img, GBufferedImage &dst);

/* Depending on how you approach your problem decomposition, you
 * may want to rewrite some of these lines, move them inside loops,
 * or move them inside helper functions, etc.
 *
 * TODO: rewrite this comment.
 */
int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    bool quitFlag;
    GBufferedImage img;
    string fileName;

    while(1)
    {
       cout << "Welcome to Fauxtoshop!" << endl;
       cout << "Enter name of image file to open (or blank to quit):" ;
       quitOrNotInputCheck(OPEN_FILE, quitFlag, fileName);
       if(!quitFlag && openImageFromFilename(img, fileName))
       {
         gw.setSize(img.getWidth(), img.getHeight());
         gw.add(&img,0,0);
         int row, col;
         getMouseClickLocation(row, col);
         cout << "Which image filter would you like tp apply?"<< endl;
         cout << "          1 - Scatter"<< endl;
         cout << "          2 - Edge detection"<< endl;
         cout << "          3 - \"Green screen\" with another image"<< endl;
         cout << "          4 - Compare image with another image"<< endl;
         cout << "Your choice:"<< endl;
         int choice;
         cin >> choice;
         GBufferedImage dst=img;
         fauxtoshop(img,dst,choice);
         quitOrNotInputCheck(SAVE_FILE, quitFlag, fileName);
         if(!quitFlag)
         {
            saveImageToFilename(dst, fileName);
         }

       } else
       {
           return 0;
       }
       cout << endl;
    }

    return 0;
}


/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
static bool openImageFromFilename(GBufferedImage &img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
static bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try {
        img.save(filename);
    } catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
static void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

/* INPUT HELPER FUNCTION
 *
 * Check input under two modes: OPEN_FILE and SAVE_FILE
 * If input means to quit, set quitflag
 * Otherwise, set quitFlag and fileName
 *
 */
static void quitOrNotInputCheck(short mode, bool &quitFlag, string &fileName){
    if(mode == OPEN_FILE)
    {
        cout << "Enter name of image file to open (or blank to quit):";
    } else if (mode == SAVE_FILE)
      {
        cout << "Enter filename to safe image (or blank to skip saving): ";
      }

    char quitChar;
    cin.get(quitChar);

    if(quitChar == '\n' || quitChar == ' ' )
    {
      quitFlag == true;
    } else
    {
      cin.putback(quitChar);
      cin >>fileName;
      quitFlag == false;
      if(mode == OPEN_FILE)
      {
          cout << "Opening image file, may take a minute..."<< endl;
      }
    }

}

/* IMAGE PROCESSING HELPER FUNCTION
 *
 * choose for 4 different processing methods for image
 */
static void fauxtoshop(const GBufferedImage &img, GBufferedImage &dst, int choice){
    switch (choice) {
    case 1:
        scatter(img, dst);
        break;

    case 2:
        edgeDetection(img, dst);
        break;

    case 3:
        greenScreen(img, dst);
        break;

    case 4:
        compare(img, dst);
        break;

    default:
        break;
    }
}

/* IMAGE PROCESSING HELPER FUNCTION
 *
 * choose for 4 different processing methods for image
 */
static void scatter(const GBufferedImage &img, GBufferedImage &dst){
    cout << "Enter degree of scatter [1-100]: ";
    int degree;
    cin >> degree;
    if(!(degree >= 1) || !(degree <= 100))
    {
        // do nothing
        return;
    } else{

    }
}

static void edgeDetection(const GBufferedImage &img, GBufferedImage &dst){

}

static void greenScreen(const GBufferedImage &img, GBufferedImage &dst){

}

static void compare(const GBufferedImage &img, GBufferedImage &dst){

}
