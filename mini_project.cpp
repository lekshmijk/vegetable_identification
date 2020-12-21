// mini_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


class features
{
public:
	int contourIndex;
	int area;
	int perimeter;
	float circularity;
	float elongation;
	float compactness;

};
void main()
{
	//load images:

	//1.main images:
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\all1.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\all2.jpg");

	//2.Individual vegetables:

	//CARROT:
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\carrot.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\carrot1.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\carrots.jpg");

	//CUCUMBER:
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\cucumber.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\cucumber1.jpg");

	//TOMATO:
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\tomato3.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\tomato.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\tomatoes1.jpg");

	//GINGER:  
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\ginger_zoom.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\ginger_pot.jpg");
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\ginger.jpg");

	//POTATO:
	//Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\potato.jpg");
	Mat img1 = imread("C:\\Users\\lekshmi\\Desktop\\4th semester\\robot perception\\mini_project\\using\\potatoes.jpg");


	imshow("Image", img1);

	//Convert to grayscale:
	Mat gray = Mat(img1.rows, img1.cols, CV_8U);
	cvtColor(img1, gray, COLOR_BGR2GRAY);

	//Convert to binary:
	Mat bin = Mat(img1.rows, img1.cols, CV_8U);
	threshold(gray, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	//use morphology: first close to fill holes and open to remove noise
	Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Mat morph_close = Mat(img1.rows, img1.cols, CV_8U);
	morphologyEx(bin, morph_close, MORPH_CLOSE, elem);
	Mat morph_open = Mat(img1.rows, img1.cols, CV_8U);
	morphologyEx(morph_close, morph_open, MORPH_OPEN, elem);

	//Find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(morph_open, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	vector<features> featVec;
	

	for (int i = 0; i < contours.size(); i++) 
	{
		if (hierarchy[i][3] == -1) 
		{
			class features f;
			f.contourIndex = i;
			f.area = contourArea(contours[i]);
			f.perimeter = arcLength(contours[i], true);
			f.circularity = (4 * 3.14 * f.area) / pow(f.perimeter, 2);
			RotatedRect box = minAreaRect(contours[i]);
			f.elongation = max(box.size.width / box.size.height, box.size.height / box.size.width);
			f.compactness = pow(f.perimeter, 2) / f.area;

			//feature values:
			/*cout << "Object " << i << ":" << endl;
			cout << "AREA:" << f.area << endl;
			cout << "PERIMETER:" << f.perimeter << endl;
			cout << "CIRCULARITY:" << f.circularity << endl;
			cout << "ELONGATION:" << f.elongation << endl;
			cout << "COMPACTNESS:" << f.compactness << "\n" << endl; */

			featVec.push_back(f);
		}
	}
	Mat contimg = Mat(img1.size(), CV_8UC3, Scalar(0, 0, 0));

	for (int i = 0; i < featVec.size(); i++) 
	{

		//show potato in brown:
		if (featVec[i].circularity > 0.8 && featVec[i].perimeter < 285 && featVec[i].elongation > 1.2) 
		{
			drawContours(contimg, contours, featVec[i].contourIndex, Scalar(63, 133, 205), -1);
			cout << "Brown object: POTATO" << endl;
		}

		// show tomato in red:
		if (featVec[i].elongation < 1.2 && featVec[i].circularity > 0.7)
		{
			drawContours(contimg, contours, featVec[i].contourIndex, Scalar(0, 0, 255), -1);
			cout << "Red object: TOMATO" << endl;
		}

		//show ginger in beige:
		if (1.2 <= featVec[i].elongation <= 1.5 && featVec[i].circularity <= 0.6 && featVec[i].compactness < 30) 
		{
			drawContours(contimg, contours, featVec[i].contourIndex, Scalar(155,198,236), -1);
			cout << "Beige object: GINGER" << endl;
		}


		//show cucumber in green:
		if (featVec[i].elongation > 5 && featVec[i].circularity < 0.4) 
		{
			drawContours(contimg, contours, featVec[i].contourIndex, Scalar(0, 150, 0), -1);
			cout << "Green object : CUCUMBER" << endl;
		}


		//show carrot in orange:
		if (featVec[i].elongation < 5 && featVec[i].circularity < 0.4) 
		{
			drawContours(contimg, contours, featVec[i].contourIndex, Scalar(0, 165, 255), -1);
			cout << "Orange object : CARROT" << endl;
		}
	}

	/*imshow("Binary", bin);
   imshow("Morph", morph_open);*/
	imshow("Filled Contours", contimg);
	waitKey(0);
}









// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
