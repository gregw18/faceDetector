/* 
 * File:   main.cpp
 * Author: gregw
 *
 * Exercise 2 from chapter 22 of Learning OpenCV 3.
 * Detects a face in images from a webcam.
 * 
 * Created on April 1, 2019, 10:31 AM
 */

#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "funcs.h"

using namespace std;

void exer22_2(int, char**);


/*
 * 
 */
int main(int argc, char** argv)
{

    exer22_2(argc, argv);

    return 0;
}


// Example 22-2 from Learning OpenCV3.
void detectAndDraw(
        cv::Mat               &img,           // Image to look for faces in.
        cv::CascadeClassifier classifier,     // Preloaded classifier.
        double                scale = 1.0     // Factor to resize image by.
) {
    // Set up colors for drawing rectangles around found faces.
    enum {BLUE, AQUA, CYAN, GREEN};
    static cv::Scalar colors[] = {
        cv::Scalar(  0,   0, 255),
        cv::Scalar(  0, 128, 255),
        cv::Scalar(  0, 255, 255),
        cv::Scalar(  0, 255,   0)
    };
    
    // Image preparation.
    cv::Mat gray;
    cv::Mat small_img;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, small_img, cv::Size(), scale, scale, cv::INTER_LINEAR);
    cv::equalizeHist(small_img, small_img);
    
    // Detect object, if any.
    vector<cv::Rect> objects;
    classifier.detectMultiScale(
            small_img,          // Input image.
            objects,            // Results.
            1.1,                // Scale factor.
            2,                  // Minimum number of neighbours.
            0,                  // Not used.
            cv::Size(30, 30)    // Throw away detections smaller than this.
    );
    
    // Loop through found objects and draw boxes around them.
    int i = 0;
    for (vector<cv::Rect>::iterator r = objects.begin(); (r != objects.end()); ++r) {
        r->height *= scale;
        r->width *= scale;
        r->x *= scale;
        r->y *= scale;
        cv::rectangle(img, *r, colors[i%4]);
        ++i;
    }
    cout << "Found " << i << " faces." << endl;
}


// Exercise 22-2. Use the standard OpenCV Haar detector to detect a face in images
// from a webcam.
// Requires: Filename of classifier datafile to use, optionally filename of file to check
//          for faces. If no image filename provided, uses attached webcam.
// If using webcam, once image appears, press c to try to capture a face. Draws red box around
// sections of images where it identifies a face.
// Hit Esc to exit the program.
// Am starting with code from Example 22-1, then adding classifier that it needs.
// A:   Haar. Pretty good with scale change, until a major feature is cut off - i.e.  mouth, or
//          face gets too blurry. Pretty sensitive to tilting head. Tipping it forward and backward
//          didn't affect it too much (at least as far as I can tip my head.) Needs a fairly head-on
//          picture - didn't react well to turning away. Pretty frequently picked up extra heads -
//          throat, ear, sweater, etc.
//      LBP. Similar results - slightly less sensitive to some changes, more to others.
//      Tested using haarcascade_frontalface_default.xml and lbpcascade_frontalface_improved.xml.
//      (Both files distributed as part of OpenCV 3.)
void exer22_2(int numArgs, char** argv) {
    
    string classifierDataFilename = argv[1];
    cv::CascadeClassifier myHaar (classifierDataFilename);
    if (numArgs == 3) {
        // Received name of file to check.
        string imgFilename = argv[2];
        cv::Mat img = get_small_image(imgFilename);
        detectAndDraw(img, myHaar);
        cv::imshow("Faces", img);
        cv::waitKey();
    }
    else {
        // Didn't receive file to process, so loop through images from webcam.
        cv::VideoCapture cap(0);
        bool doCapture = true;
        for (;;) {
            cv::Mat img;
            cap >> img;
            if (doCapture) {
                detectAndDraw(img, myHaar);
                cv::imshow("Faces", img);
            }
            else {
                cv::imshow( "Raw", img);
            }
            char c = cv::waitKey(250);
            if (c == 27) break;
            else {
                if (c == 'c') doCapture = true;
                else doCapture = false;
            }
        }
    }
    
}

