#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

struct features
{
	int contourIndex;
	int area;
	int perimeter;
	float circularity;
	float elongation;
};

int main()
{
	vector<features> featVec;
	vector<int> areaVec;
	int counter = 0;
	Mat livecam;
	Mat referenceframe;
	VideoCapture cap(0);
	vector<Mat> reference;
	Mat diff;
	Mat res;
	Mat res2;
	Mat morphimg;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	for (int i = 0; i < 100; i++) {
		cap >> referenceframe;
		cout << i << endl;
		if (i == 99) {
			reference.push_back(referenceframe);
		}
	}
	while (true) {
		counter++;
		cout << counter << endl;
		cap >> livecam;
		//Mat mask(livecam.size(), CV_8UC1);
		Mat contourimg(livecam.size(), CV_8UC3, Scalar(255, 255, 255));
		waitKey(10);
		absdiff(reference[0], livecam, diff);
		//imshow("livecam", livecam);
		//imshow("diff", diff);
		if (counter % 2 == 1) {
			cap >> referenceframe;
			reference.clear();
			reference.push_back(referenceframe);
		}
		//imshow("reference", reference[0]);
		//int th = 10;
		/*for (int j = 0; j < diff.rows; ++j) {
			for (int i = 0; i < diff.cols; ++i) {
				Vec3b pix = diff.at<Vec3b>(j, i);
				int val = (pix[0] + pix[1] + pix[2]);
				if (val > th) {
					mask.at<unsigned char>(j, i) = 255;
				}
			}
		}
		bitwise_and(diff, diff, res, mask);*/
		inRange(diff, Scalar(35, 35, 35), Scalar(255, 255, 255), res2);
		imshow("result2", res2);
		Mat elem = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
		morphologyEx(res2, morphimg, MORPH_CLOSE, elem);
		//medianBlur(morphimg, morphimg, 11);
		findContours(morphimg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
		for (int i = 0; i < contours.size(); i++) {
			features f;
			f.contourIndex = i;
			f.area = contourArea(contours[i]);
			f.perimeter = arcLength(contours[i], true);
			f.circularity = (4 * 3.14 * f.area) / pow(f.perimeter, 2);
			areaVec.push_back(f.area);
			featVec.push_back(f);
		}

		sort(areaVec.begin(), areaVec.end(), greater<int>());
		for (int i = 0; i < featVec.size(); i++) {
			if (featVec[i].area == areaVec[0]) {
				drawContours(contourimg, contours, featVec[i].contourIndex, Scalar(0, 0, 255), 1);
			}
		}
		areaVec.clear();
		featVec.clear();
		//imshow("mask", mask);
		//imshow("result", res);
		//imshow("result2", res2);
		//imshow("morph", morphimg);
		imshow("contourImg", contourimg);
	}
	return 0;
}