#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <windows.h>
#include "seatPoint.h"

using namespace cv;
using namespace std;


//좌석 기록 함수

int check[MAXSEAT] = { 0, };
int check_p[MAXSEAT] = { 0, };

void checkCenter(int centerX, int centerY, int numSeat) {
	if ((numSeat == 1) && (centerX <= SEAT1xT) && (centerX >= SEAT1xB) && (centerY <= SEAT1yT) && (centerY >= SEAT1yB)) {
		check[1] = 1;
	}
	if ((numSeat == 2) && (centerX <= SEAT2xT) && (centerX >= SEAT2xB) && (centerY <= SEAT2yT) && (centerY >= SEAT2yB)) {
		check[2] = 1;
	}
	// 추가필요
}

//글자색 지정 함수

void textcolor(int color_number) //콘솔창 출력색상을 나타내는 함수
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

//위치 지정

void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//색 추출 함수

Mat img_color;

Vec3b lower_1_1, upper_1_1, lower_1_2, upper_1_2, lower_1_3, upper_1_3, lower_2_1, upper_2_1, lower_2_2, upper_2_2, lower_2_3, upper_2_3;	 // 좌석 1 배경색 : 1,2
Vec3b lower_3_1, upper_3_1, lower_3_2, upper_3_2, lower_3_3, upper_3_3, lower_4_1, upper_4_1, lower_4_2, upper_4_2, lower_4_3, upper_4_3;	 // 좌석 2 배경색 : 3,4
Vec3b lower_5_1, upper_5_1, lower_5_2, upper_5_2, lower_5_3, upper_5_3, lower_6_1, upper_6_1, lower_6_2, upper_6_2, lower_6_3, upper_6_3;

int range = 10;	//색의 범위, +-의 범위를 뽑는다.
int record = 0;

void mouse_callback(int event, int x, int y, int flags, void* param)
{
	int saturationControlL, valueControlL;
	int saturationControlH, valueControlH;

	if (event == EVENT_LBUTTONDOWN && record == 0)
	{	
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);
		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
		int hue = hsv_color.at<Vec3b>(0, 0)[0];
		int saturation = hsv_color.at<Vec3b>(0, 0)[1];
		int value = hsv_color.at<Vec3b>(0, 0)[2];

		gotoxy(1, 4); cout << " hue = " << hue << endl;
		gotoxy(1, 5); cout << " saturation = " << saturation << endl;
		gotoxy(1, 6); cout << " value = " << value << endl;

		if (value < 80) {
			lower_1_1 = Vec3b(0, 0, 0); upper_1_1 = Vec3b(180, 255, 80);
			lower_1_2 = Vec3b(0, 0, 0); upper_1_2 = Vec3b(180, 255, 80);
			lower_1_3 = Vec3b(0, 0, 0); upper_1_3 = Vec3b(180, 255, 80);
		}
		else if (saturation < 30) {
			lower_1_1 = Vec3b(0, 0, 30); upper_1_1 = Vec3b(180, 30, 255);
			lower_1_2 = Vec3b(0, 0, 30); upper_1_2 = Vec3b(180, 30, 255);
			lower_1_3 = Vec3b(0, 0, 30); upper_1_3 = Vec3b(180, 30, 255);
		}
		else {
			if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
			else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
			else saturationControlL = 110, saturationControlH = 180;

			if (value < 140) valueControlL = 80, valueControlH = 140;
			else if (value > 200) valueControlL = 200, valueControlH = 255;
			else valueControlL = 140, valueControlH = 200;

			if (hue < 10)
			{
				lower_1_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
				upper_1_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_1_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_1_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_1_3 = Vec3b(hue, saturationControlL, valueControlL);
				upper_1_3 = Vec3b(hue + range, saturationControlH, valueControlH);
			}
			else if (hue > 170)
			{
				lower_1_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_1_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_1_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_1_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
				lower_1_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_1_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
			else
			{
				lower_1_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_1_1 = Vec3b(hue + range, saturationControlH, valueControlH);
				lower_1_2 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_1_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_1_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_1_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
		}
		gotoxy(1, 7); cout << " #1 = " << lower_1_1 << "~" << upper_1_1 << endl;
		gotoxy(1, 8); cout << " #2 = " << lower_1_2 << "~" << upper_1_2 << endl;
		gotoxy(1, 9); cout << " #3 = " << lower_1_3 << "~" << upper_1_3 << endl;
		record = 1;

	}
	else if (event == EVENT_LBUTTONDOWN && record == 1)
	{
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);
		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
		int hue = hsv_color.at<Vec3b>(0, 0)[0];
		int saturation = hsv_color.at<Vec3b>(0, 0)[1];
		int value = hsv_color.at<Vec3b>(0, 0)[2];

		gotoxy(41, 4);  cout << " hue = " << hue << endl;
		gotoxy(41, 5);  cout << " saturation = " << saturation << endl;
		gotoxy(41, 6);  cout << " value = " << value << endl;

		if (value < 80) {
			lower_2_1 = Vec3b(0, 0, 0); upper_2_1 = Vec3b(180, 255, 80);
			lower_2_2 = Vec3b(0, 0, 0); upper_2_2 = Vec3b(180, 255, 80);
			lower_2_3 = Vec3b(0, 0, 0); upper_2_3 = Vec3b(180, 255, 80);
		}
		else if (saturation < 30) {
			lower_2_1 = Vec3b(0, 0, 30); upper_2_1 = Vec3b(180, 30, 255);
			lower_2_2 = Vec3b(0, 0, 30); upper_2_2 = Vec3b(180, 30, 255);
			lower_2_3 = Vec3b(0, 0, 30); upper_2_3 = Vec3b(180, 30, 255);
		}
		else {
			if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
			else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
			else saturationControlL = 110, saturationControlH = 180;

			if (value < 140) valueControlL = 80, valueControlH = 140;
			else if (value > 200) valueControlL = 200, valueControlH = 255;
			else valueControlL = 140, valueControlH = 200;

			if (hue < 10)
			{
				lower_2_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
				upper_2_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_2_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_2_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_2_3 = Vec3b(hue, saturationControlL, valueControlL);
				upper_2_3 = Vec3b(hue + range, saturationControlH, valueControlH);
			}
			else if (hue > 170)
			{
				lower_2_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_2_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_2_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_2_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
				lower_2_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_2_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
			else
			{
				lower_2_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_2_1 = Vec3b(hue + range, saturationControlH, valueControlH);
				lower_2_2 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_2_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_2_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_2_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
		}
		gotoxy(41, 7);  cout << " #1 = " << lower_2_1 << "~" << upper_2_1 << endl;
		gotoxy(41, 8);  cout << " #2 = " << lower_2_2 << "~" << upper_2_2 << endl;
		gotoxy(41, 9);  cout << " #3 = " << lower_2_3 << "~" << upper_2_3 << endl;


		record = 2;
	}
	else if (event == EVENT_LBUTTONDOWN && record == 2)
	{
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);
		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
		int hue = hsv_color.at<Vec3b>(0, 0)[0];
		int saturation = hsv_color.at<Vec3b>(0, 0)[1];
		int value = hsv_color.at<Vec3b>(0, 0)[2];

		gotoxy(81, 4); cout << " hue = " << hue << endl;
		gotoxy(81, 5); cout << " saturation = " << saturation << endl;
		gotoxy(81, 6); cout << " value = " << value << endl;

		if (value < 80) {
			lower_3_1 = Vec3b(0, 0, 0); upper_3_1 = Vec3b(180, 255, 80);
			lower_3_2 = Vec3b(0, 0, 0); upper_3_2 = Vec3b(180, 255, 80);
			lower_3_3 = Vec3b(0, 0, 0); upper_3_3 = Vec3b(180, 255, 80);
		}
		else if (saturation < 30) {
			lower_3_1 = Vec3b(0, 0, 30); upper_3_1 = Vec3b(180, 30, 255);
			lower_3_2 = Vec3b(0, 0, 30); upper_3_2 = Vec3b(180, 30, 255);
			lower_3_3 = Vec3b(0, 0, 30); upper_3_3 = Vec3b(180, 30, 255);
		}
		else {
			if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
			else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
			else saturationControlL = 110, saturationControlH = 180;

			if (value < 140) valueControlL = 80, valueControlH = 140;
			else if (value > 200) valueControlL = 200, valueControlH = 255;
			else valueControlL = 140, valueControlH = 200;

			if (hue < 10)
			{
				lower_3_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
				upper_3_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_3_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_3_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_3_3 = Vec3b(hue, saturationControlL, valueControlL);
				upper_3_3 = Vec3b(hue + range, saturationControlH, valueControlH);
			}
			else if (hue > 170)
			{
				lower_3_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_3_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_3_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_3_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
				lower_3_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_3_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
			else
			{
				lower_3_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_3_1 = Vec3b(hue + range, saturationControlH, valueControlH);
				lower_3_2 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_3_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_3_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_3_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
		}
		gotoxy(81, 7); cout << " #1 = " << lower_3_1 << "~" << upper_3_1 << endl;
		gotoxy(81, 8); cout << " #2 = " << lower_3_2 << "~" << upper_3_2 << endl;
		gotoxy(81, 9); cout << " #3 = " << lower_3_3 << "~" << upper_3_3 << endl;

		record = 3;
	}
	else if (event == EVENT_LBUTTONDOWN && record == 3)
	{
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);
		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
		int hue = hsv_color.at<Vec3b>(0, 0)[0];
		int saturation = hsv_color.at<Vec3b>(0, 0)[1];
		int value = hsv_color.at<Vec3b>(0, 0)[2];

		gotoxy(1, 13); cout << " hue = " << hue << endl;
		gotoxy(1, 14); cout << " saturation = " << saturation << endl;
		gotoxy(1, 15); cout << " value = " << value << endl;

		if (value < 80) {
			lower_4_1 = Vec3b(0, 0, 0); upper_4_1 = Vec3b(180, 255, 80);
			lower_4_2 = Vec3b(0, 0, 0); upper_4_2 = Vec3b(180, 255, 80);
			lower_4_3 = Vec3b(0, 0, 0); upper_4_3 = Vec3b(180, 255, 80);
		}
		else if (saturation < 30) {
			lower_4_1 = Vec3b(0, 0, 30); upper_4_1 = Vec3b(180, 30, 255);
			lower_4_2 = Vec3b(0, 0, 30); upper_4_2 = Vec3b(180, 30, 255);
			lower_4_3 = Vec3b(0, 0, 30); upper_4_3 = Vec3b(180, 30, 255);
		}
		else {
			if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
			else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
			else saturationControlL = 110, saturationControlH = 180;

			if (value < 140) valueControlL = 80, valueControlH = 140;
			else if (value > 200) valueControlL = 200, valueControlH = 255;
			else valueControlL = 140, valueControlH = 200;

			if (hue < 10)
			{
				lower_4_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
				upper_4_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_4_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_4_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_4_3 = Vec3b(hue, saturationControlL, valueControlL);
				upper_4_3 = Vec3b(hue + range, saturationControlH, valueControlH);
			}
			else if (hue > 170)
			{
				lower_4_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_4_1 = Vec3b(180, saturationControlH, valueControlH);
				lower_4_2 = Vec3b(0, saturationControlL, valueControlL);
				upper_4_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
				lower_4_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_4_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
			else
			{
				lower_4_1 = Vec3b(hue, saturationControlL, valueControlL);
				upper_4_1 = Vec3b(hue + range, saturationControlH, valueControlH);
				lower_4_2 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_4_2 = Vec3b(hue, saturationControlH, valueControlH);
				lower_4_3 = Vec3b(hue - range, saturationControlL, valueControlL);
				upper_4_3 = Vec3b(hue, saturationControlH, valueControlH);
			}
		}
		gotoxy(1, 16); cout << " #1 = " << lower_4_1 << "~" << upper_4_1 << endl;
		gotoxy(1, 17); cout << " #2 = " << lower_4_2 << "~" << upper_4_2 << endl;
		gotoxy(1, 18); cout << " #3 = " << lower_4_3 << "~" << upper_4_3 << endl;

		record = 4;
	}
	else if (event == EVENT_LBUTTONDOWN && record == 4)
	{
	Vec3b color_pixel = img_color.at<Vec3b>(y, x);
	Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
	Mat hsv_color;
	cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
	int hue = hsv_color.at<Vec3b>(0, 0)[0];
	int saturation = hsv_color.at<Vec3b>(0, 0)[1];
	int value = hsv_color.at<Vec3b>(0, 0)[2];

	gotoxy(41, 13); cout << " hue = " << hue << endl;
	gotoxy(41, 14); cout << " saturation = " << saturation << endl;
	gotoxy(41, 15); cout << " value = " << value << endl;

	if (value < 80) {
		lower_5_1 = Vec3b(0, 0, 0); upper_5_1 = Vec3b(180, 255, 80);
		lower_5_2 = Vec3b(0, 0, 0); upper_5_2 = Vec3b(180, 255, 80);
		lower_5_3 = Vec3b(0, 0, 0); upper_5_3 = Vec3b(180, 255, 80);
	}
	else if (saturation < 30) {
		lower_5_1 = Vec3b(0, 0, 30); upper_5_1 = Vec3b(180, 30, 255);
		lower_5_2 = Vec3b(0, 0, 30); upper_5_2 = Vec3b(180, 30, 255);
		lower_5_3 = Vec3b(0, 0, 30); upper_5_3 = Vec3b(180, 30, 255);
	}
	else {
		if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
		else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
		else saturationControlL = 110, saturationControlH = 180;

		if (value < 140) valueControlL = 80, valueControlH = 140;
		else if (value > 200) valueControlL = 200, valueControlH = 255;
		else valueControlL = 140, valueControlH = 200;

		if (hue < 10)
		{
			lower_5_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
			upper_5_1 = Vec3b(180, saturationControlH, valueControlH);
			lower_5_2 = Vec3b(0, saturationControlL, valueControlL);
			upper_5_2 = Vec3b(hue, saturationControlH, valueControlH);
			lower_5_3 = Vec3b(hue, saturationControlL, valueControlL);
			upper_5_3 = Vec3b(hue + range, saturationControlH, valueControlH);
		}
		else if (hue > 170)
		{
			lower_5_1 = Vec3b(hue, saturationControlL, valueControlL);
			upper_5_1 = Vec3b(180, saturationControlH, valueControlH);
			lower_5_2 = Vec3b(0, saturationControlL, valueControlL);
			upper_5_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
			lower_5_3 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_5_3 = Vec3b(hue, saturationControlH, valueControlH);
		}
		else
		{
			lower_5_1 = Vec3b(hue, saturationControlL, valueControlL);
			upper_5_1 = Vec3b(hue + range, saturationControlH, valueControlH);
			lower_5_2 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_5_2 = Vec3b(hue, saturationControlH, valueControlH);
			lower_5_3 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_5_3 = Vec3b(hue, saturationControlH, valueControlH);
		}
	}
	gotoxy(41, 16); cout << " #1 = " << lower_5_1 << "~" << upper_5_1 << endl;
	gotoxy(41, 17); cout << " #2 = " << lower_5_2 << "~" << upper_5_2 << endl;
	gotoxy(41, 18); cout << " #3 = " << lower_5_3 << "~" << upper_5_3 << endl;

	record = 5;
	}
	else if (event == EVENT_LBUTTONDOWN && record == 5)
	{
	Vec3b color_pixel = img_color.at<Vec3b>(y, x);
	Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);
	Mat hsv_color;
	cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);
	int hue = hsv_color.at<Vec3b>(0, 0)[0];
	int saturation = hsv_color.at<Vec3b>(0, 0)[1];
	int value = hsv_color.at<Vec3b>(0, 0)[2];

	gotoxy(81, 13); cout << " hue = " << hue << endl;
	gotoxy(81, 14); cout << " saturation = " << saturation << endl;
	gotoxy(81, 15); cout << " value = " << value << endl;

	if (value < 80) {
		lower_6_1 = Vec3b(0, 0, 0); upper_6_1 = Vec3b(180, 255, 80);
		lower_6_2 = Vec3b(0, 0, 0); upper_6_2 = Vec3b(180, 255, 80);
		lower_6_3 = Vec3b(0, 0, 0); upper_6_3 = Vec3b(180, 255, 80);
	}
	else if (saturation < 30) {
		lower_6_1 = Vec3b(0, 0, 30); upper_6_1 = Vec3b(180, 30, 255);
		lower_6_2 = Vec3b(0, 0, 30); upper_6_2 = Vec3b(180, 30, 255);
		lower_6_3 = Vec3b(0, 0, 30); upper_6_3 = Vec3b(180, 30, 255);
	}
	else {
		if (saturation < 110) saturationControlL = 30, saturationControlH = 110;
		else if (saturation > 180) saturationControlL = 180, saturationControlH = 255;
		else saturationControlL = 110, saturationControlH = 180;

		if (value < 140) valueControlL = 80, valueControlH = 140;
		else if (value > 200) valueControlL = 200, valueControlH = 255;
		else valueControlL = 140, valueControlH = 200;

		if (hue < 10)
		{
			lower_6_1 = Vec3b(hue - range + 180, saturationControlL, valueControlL);
			upper_6_1 = Vec3b(180, saturationControlH, valueControlH);
			lower_6_2 = Vec3b(0, saturationControlL, valueControlL);
			upper_6_2 = Vec3b(hue, saturationControlH, valueControlH);
			lower_6_3 = Vec3b(hue, saturationControlL, valueControlL);
			upper_6_3 = Vec3b(hue + range, saturationControlH, valueControlH);
		}
		else if (hue > 170)
		{
			lower_6_1 = Vec3b(hue, saturationControlL, valueControlL);
			upper_6_1 = Vec3b(180, saturationControlH, valueControlH);
			lower_6_2 = Vec3b(0, saturationControlL, valueControlL);
			upper_6_2 = Vec3b(hue + range - 180, saturationControlH, valueControlH);
			lower_6_3 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_6_3 = Vec3b(hue, saturationControlH, valueControlH);
		}
		else
		{
			lower_6_1 = Vec3b(hue, saturationControlL, valueControlL);
			upper_6_1 = Vec3b(hue + range, saturationControlH, valueControlH);
			lower_6_2 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_6_2 = Vec3b(hue, saturationControlH, valueControlH);
			lower_6_3 = Vec3b(hue - range, saturationControlL, valueControlL);
			upper_6_3 = Vec3b(hue, saturationControlH, valueControlH);
		}
	}
	gotoxy(81, 16); cout << " #1 = " << lower_6_1 << "~" << upper_6_1 << endl;
	gotoxy(81, 17); cout << " #2 = " << lower_6_2 << "~" << upper_6_2 << endl;
	gotoxy(81, 18); cout << " #3 = " << lower_6_3 << "~" << upper_6_3 << endl;

	record = 0;
	}
}