/*
 * fauxtoshop.cpp
 * Start code from Standford CS106X (Authum 2015) Assignment 1
 * Including all rewritten code for the assignment
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

// For rewriting implementation
#include "random.h"

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
static void     fauxtoshop(GBufferedImage &img,  int choice);
static void     scatter(GBufferedImage &img);
static void     edgeDetection( GBufferedImage &img);
static void     greenScreen( GBufferedImage &img);
//static void     compare(GBufferedImage &img);

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
         cout << "Your choice:";
         int choice;
         cin >> choice;
         //GBufferedImage* dstPtr = new GBufferedImage(img.getWidth(), img.getHeight());
         //GBufferedImage& dst = *dstPtr;
         fauxtoshop(img,choice);
         cin.ignore(1,'\n');
         quitOrNotInputCheck(SAVE_FILE, quitFlag, fileName);
         if(!quitFlag)
         {
            saveImageToFilename(img, fileName);
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

    if(isspace(quitChar))
    {
      quitFlag = true;
      cout << endl;
    } else
    {
      cin.putback(quitChar);
      cin >>fileName;
      quitFlag = false;
      if(mode == OPEN_FILE)
      {
          cout << "Opening image file, may take a minute..."<< endl;
      }else if (mode == SAVE_FILE)
      {
        //cout << fileName <<endl;
        //cin.ignore(1,'\n');
      }
    }

}

/* IMAGE PROCESSING HELPER FUNCTION
 *
 * Choose for 4 different processing methods for image
 */
static void fauxtoshop(GBufferedImage &img, int choice){
    switch (choice) {
    case 1:
        scatter(img);
        break;

    case 2:
        edgeDetection(img);
        break;

    case 3:
        greenScreen(img);
        break;

    case 4:
        //compare(img);
        break;

    default:
        break;
    }
}

/* IMAGE PROCESSING HELPER FUNCTION ONE - SCATTER
 *
 * Take the original image and “scatter” its pixels,
 * making something that looks like a sand drawing that was shaken
 */
static void scatter(GBufferedImage &img){
    cout << "Enter degree of scatter [1-100]: ";
    int degree;
    cin >> degree;
    if(!(degree >= 1) || !(degree <= 100))
    {
        // do nothing
        return;
    } else{
        double hei = img.getHeight();
        double wid = img.getWidth();
        setRandomSeed(1);
        for(double y = 0; y < hei; y++)
        {
            for(double x = 0; x < wid; x++)
            {
                GBufferedImage* imgPtr = &img;
                double randomX;
                double randomY;
                do{
                    randomX = randomInteger(x - degree, x + degree);
                    randomY = randomInteger(y - degree, y + degree);
                } while(!imgPtr->inBounds(randomX, randomY));
                int xyrgb = imgPtr->getRGB(x,y);
                imgPtr -> setRGB(randomX,randomY,xyrgb);
            }
        }
    }
}

/* IMAGE PROCESSING HELPER FUNCTION TWO
 *
 * choose for 4 different processing methods for image
 */
static void edgeDetection(GBufferedImage &img){
return;
}

/* IMAGE PROCESSING HELPER FUNCTION THREE
 *
 * choose for 4 different processing methods for image
 */
static void greenScreen(GBufferedImage &img){
return;
}

/* IMAGE PROCESSING HELPER FUNCTION
 *
 * choose for 4 different processing methods for image
 */
/*static void compare(GBufferedImage &img, GBufferedImage &dst){
return;
}*/
