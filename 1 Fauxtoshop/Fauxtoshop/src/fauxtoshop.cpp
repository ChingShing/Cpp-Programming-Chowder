/*
 * fauxtoshop.cpp
 * Start code from Standford CS106X (Authum 2015) Assignment 1
 * Including all rewritten code for the assignment
 * Rewritten by Ching_Shing, junior student in SJTU
 * CS106B/X C++ style guideline adopted (ref:http://stanford.edu/class/archive/cs/cs106b/cs106b.1158/styleguide.shtml)
 *
 * This file is to generate a photo editor with 4 choices:
 * 1 - Scatter(Finished)
 * 2 - Edge detection(Finished)
 * 3 - "Green screen" with another image(Unfinished)
 * 4 - Compare image with another image(Unfinished)
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
static const int    RED   = 0xFF0000;
static const int    GREEN = 0x00FF00;
static const int    BLUE  = 0x0000FF;
static const short    OPEN_FILE = 0;
static const short    SAVE_FILE = 1;

static bool     openImageFromFilename(GBufferedImage &img, string filename);
static bool 	saveImageToFilename(const GBufferedImage &img, string filename);
static void     getMouseClickLocation(int &row, int &col);
static void     quitOrNotInputCheck(short mode, bool &quitFlag, string &filename);
static void     fauxtoshop(GBufferedImage &img,  int choice);
static void     scatter(GBufferedImage &img);
static void     edgeDetection(GBufferedImage &img);
static void     greenScreen( GBufferedImage &img);
static void     compareImage(GBufferedImage &img);
static bool     isEdge(const GBufferedImage &img, const double &x, const double &y,
                   const double &i, const double &j,const int &threshold);

/* Depending on how you approach your problem decomposition, you
 * may want to rewrite some of these lines, move them inside loops,
 * or move them inside helper functions, etc.
 *
 * TODO: rewrite this comment.
 */
int main() {
    while(1)
    {
        GWindow gw;
        gw.setTitle("Fauxtoshop");
        gw.setVisible(true);
        bool quitFlag;
        GBufferedImage img;
        string fileName;
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
    if(isspace(quitChar))//Case: quit or skip
    {
        quitFlag = true;
        cout << endl;
    } else//Case: continue
    {
        cin.putback(quitChar);
        cin >>fileName;
        quitFlag = false;
        if(mode == OPEN_FILE)
        {
            cout << "Opening image file, may take a minute..."<< endl;
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
        compareImage(img);
        break;

    default:
        break;
    }
}

/* IMAGE PROCESSING HELPER FUNCTION ONE - SCATTER
 *
 * Take the original image and “scatter” its pixels,
 * making something that looks like a sand drawing that was shaken
 *
 * By changing each pixel after randomly picking another pixel within its degree domain
 */
static void scatter(GBufferedImage &img){
    int degree;
    do{
        cout << "Enter degree of scatter [1-100]: ";
        cin >> degree;
    }while(!(degree >= 1) || !(degree <= 100));

    double hei = img.getHeight();
    double wid = img.getWidth();
    setRandomSeed(1);
    for(double y = 0; y < hei; y++)
    {
        for(double x = 0; x < wid; x++)
        {
            double randomX, randomY;
            do{
                randomX = randomInteger(x - degree, x + degree);
                randomY = randomInteger(y - degree, y + degree);
            } while(!img.inBounds(randomX, randomY));
            int xyrgb = img.getRGB(x,y);
            img.setRGB(randomX,randomY,xyrgb);
        }
    }

}

/* IMAGE PROCESSING HELPER FUNCTION TWO - EDGE DETECTION
 *
 * Show the edge of the input image using user-defined threshold
 *
 * Ask user a threshold and to each pixel:
 * If at least one of its neighbors has a difference of greater than threshold from it,
 * blacken the picel,
 * otherwise whiten it.
 *
 */
static void edgeDetection(GBufferedImage &img){
    int threshold;
    do{
        cout << "Enter threshold of edge detection: ";
        cin >> threshold;
    }while(!(threshold > 0));

    double hei = img.getHeight();
    double wid = img.getWidth();
    Grid<int> markGrid(hei, wid);
    setRandomSeed(1);
    for(double y = 0; y < hei; y++)
    {
        for(double x = 0; x < wid; x++)
        {
            for(double i = -1; i < 2; i++)
            {
                for(double j = -1; j < 2; j++)
                {
                    if(img.inBounds(x + i, y + j ))
                    {
                        if(isEdge(img, x, y, x + i, y + j, threshold))
                        {
                            markGrid.set((int)y,(int)x,1);
                        }
                    }
                    break;
                }
                break;
            }
            if(markGrid.get((int)y, (int)x) != 1) markGrid.set((int)y,(int)x,0);
        }
    }
    for(int y = 0; y < hei; y++)
    {
        for(int x = 0; x < wid; x++)
        {
            if(markGrid.get(y, x) == 1) img.setRGB(x, y, BLACK);
            else img.setRGB(x, y, WHITE);
        }
    }
}

/* EDGE DETECTION HELPER FUNCTION  - ISEDGE
 *
 * Return true if (x, y) and (i,j) has an edge
 *
 * If two pixels have differences in RGB greater than threshold,
 * between them is an edge,
 * otherwise isn't.
 */
static bool isEdge(const GBufferedImage &img, const double &x, const double &y,
                   const double &i, const double &j,const int &threshold){
    int xyrgb = img.getRGB(x,y);
    int ijrgb = img.getRGB(i,j);
    int  xyColor = (xyrgb & RED) >> 16;
    int ijColor = (ijrgb & RED) >> 16;
    int redDifference = xyColor - ijColor;
    if(abs(redDifference) <= threshold) return false;

    xyColor = (xyrgb & GREEN) >> 8;
    ijColor = (ijrgb & RED) >> 8;
    int greenDifference = xyColor - ijColor ;
    if(abs(greenDifference) <= threshold) return false;

    xyColor = xyrgb & BLUE;
    ijColor = ijrgb & BLUE;
    int blueDifference = xyColor - ijColor ;
    if(abs(blueDifference) <= threshold) return false;

    return true;
}

/* IMAGE PROCESSING HELPER FUNCTION THREE
 *
 *
 */
static void greenScreen(GBufferedImage &img){
    return;
}

/* IMAGE PROCESSING HELPER FUNCTION
 *
 *
 */
static void compareImage(GBufferedImage &img){
    return;
}
