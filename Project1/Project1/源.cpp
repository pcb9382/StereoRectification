

#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
//#include "cvaux.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

using namespace std;


int main()
{
	CvMat *Intrinsics_Camera_Left = (CvMat*)cvLoad("M1.xml");
	CvMat *Intrinsics_Camera_Right = (CvMat*)cvLoad("M2.xml");
	CvMat *Distortion_Camera_Left = (CvMat*)cvLoad("D1.xml");
	CvMat *Distortion_Camera_Right = (CvMat*)cvLoad("D2.xml");
	CvMat *Translation_matlab = (CvMat*)cvLoad("T.xml");
	CvMat *RotRodrigues_matlab = (CvMat*)cvLoad("R.xml");

	CvSize imageSize = { 0, 0 };
	IplImage* img = cvLoadImage("left1.jpg", 0);
	imageSize = cvGetSize(img);

	CvMat* mx1 = cvCreateMat(imageSize.height,
		imageSize.width, CV_32F);
	CvMat* my1 = cvCreateMat(imageSize.height,
		imageSize.width, CV_32F);
	CvMat* mx2 = cvCreateMat(imageSize.height,

		imageSize.width, CV_32F);
	CvMat* my2 = cvCreateMat(imageSize.height,
		imageSize.width, CV_32F);
	CvMat* img1r = cvCreateMat(imageSize.height,
		imageSize.width, CV_8U);
	CvMat* img2r = cvCreateMat(imageSize.height,
		imageSize.width, CV_8U);
	
	CvMat *pair;
	CvMat part;
	pair = cvCreateMat(imageSize.height, imageSize.width * 2,
		CV_8UC3);
	double R1[3][3], R2[3][3], P1[3][4], P2[3][4];
	CvMat _R1 = cvMat(3, 3, CV_64F, R1);
	CvMat _R2 = cvMat(3, 3, CV_64F, R2);

	CvMat _P1 = cvMat(3, 4, CV_64F, P1);
	CvMat _P2 = cvMat(3, 4, CV_64F, P2);
	CvMat *Q = cvCreateMat(4, 4, CV_64F);
	cvStereoRectify(Intrinsics_Camera_Left, Intrinsics_Camera_Right, Distortion_Camera_Left, Distortion_Camera_Right, imageSize,
		RotRodrigues_matlab, Translation_matlab,
		&_R1, &_R2, &_P1, &_P2, Q,
		CV_CALIB_ZERO_DISPARITY);

	cvInitUndistortRectifyMap(Intrinsics_Camera_Left, Distortion_Camera_Left, &_R1, &_P1, mx1, my1);
	cvInitUndistortRectifyMap(Intrinsics_Camera_Right, Distortion_Camera_Right, &_R2, &_P2, mx2, my2);
	
	cvSave("Q.xml",Q);

	IplImage* img1 = cvLoadImage("left1.jpg", 0);
	IplImage* img2 = cvLoadImage("right1.jpg", 0);
	if (img1 && img2)
	{
		
		cvRemap(img1, img1r, mx1, my1);
		cvRemap(img2, img2r, mx2, my2);
	}
	cvSaveImage("img1r.jpg", img1r);
	cvSaveImage("img2r.jpg", img2r);
	cvGetCols(pair, &part, 0, imageSize.width);
	cvCvtColor(img1r, &part, CV_GRAY2BGR);
	cvGetCols(pair, &part, imageSize.width,
		imageSize.width * 2);
	cvCvtColor(img2r, &part, CV_GRAY2BGR);
	for (int j = 0; j < imageSize.height; j += 16)
	{       
		
		cvLine(pair, cvPoint(0, j),
	    cvPoint(imageSize.width * 2, j),
			CV_RGB(0, 255, 0));
	}

	cvSaveImage("rectified.jpg", pair);
	cvNamedWindow("rectified",0);
	cvWaitKey(0);
	cvShowImage("rectified", pair);
	
	cvWaitKey(0);

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseMat(&mx1);
	cvReleaseMat(&my1);
	cvReleaseMat(&mx2);
	cvReleaseMat(&my2);
	cvReleaseMat(&img1r);
	cvReleaseMat(&img2r);
	
}