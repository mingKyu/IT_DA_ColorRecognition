#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "seatPoint.h"

using namespace cv;
using namespace std;

extern Vec3b lower_1_1, upper_1_1, lower_1_2, upper_1_2, lower_1_3, upper_1_3, lower_2_1, upper_2_1, lower_2_2, upper_2_2, lower_2_3, upper_2_3;		// function 함수 참조
extern Vec3b lower_3_1, upper_3_1, lower_3_2, upper_3_2, lower_3_3, upper_3_3, lower_4_1, upper_4_1, lower_4_2, upper_4_2, lower_4_3, upper_4_3;
extern Vec3b lower_5_1, upper_5_1, lower_5_2, upper_5_2, lower_5_3, upper_5_3, lower_6_1, upper_6_1, lower_6_2, upper_6_2, lower_6_3, upper_6_3;

extern Mat img_color;

extern int check[];
extern int check_p[];

extern void mouse_callback(int event, int x, int y, int flags, void* param);
extern void checkCenter(int centerX, int centerY, int numSeat);
extern void gotoxy(int x, int y);
extern void textcolor(int color_number);

int record[MAXSEAT] = { 0, };

int main()
{

	system("mode con cols=120 lines=40");

	textcolor(0);
	namedWindow("designation window");
	setMouseCallback("designation window", mouse_callback);

	Mat img_hsv;

	VideoCapture cap(0);																																															// 카메라 설정 부분 : 현재 0번째 카메라

	if (!cap.isOpened()) {
		cout << "카메라를 열 수 없습니다." << endl;
		return -1;
	}

	while (1)
	{
		cap.read(img_color);

		cvtColor(img_color, img_hsv, COLOR_BGR2HSV);

		textcolor(15);
		gotoxy(0, 1); printf("Base Color");
		gotoxy(1, 2); printf("SEAT1 ");
		gotoxy(1, 3); printf("Color 1");
		gotoxy(41, 3); printf("Color 2");
		gotoxy(81, 3); printf("Color 3");

		gotoxy(1, 11); printf("SEAT1 ");
		gotoxy(1, 12); printf("SEAT2 ");
		gotoxy(1, 12); printf("Color 4");
		gotoxy(41, 12); printf("Color 5");
		gotoxy(81, 12); printf("Color 6");

		Mat img_mask_b, seat_mask_01, seat_mask_02; //최종 화면
		Mat img_mask01, img_mask02, img_mask03, img_mask04, img_mask05, img_mask06; //반전 전 화면
		Mat img_mask11, img_mask12, img_mask13, img_mask14, img_mask15, img_mask16; //반전 이후 화면
		Mat i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16, i17, i18, i19, i20, i21, i22, i23, i24;

		inRange(img_hsv, Vec3b(0, 0, 0), Vec3b(180, 255, 255), img_mask_b);

		//좌석 1 색 설정
		inRange(img_hsv, lower_1_1, upper_1_1, i1);	inRange(img_hsv, lower_1_2, upper_1_2, i2); inRange(img_hsv, lower_1_3, upper_1_3, i3);

		img_mask01 = i1 | i2 | i3;
		img_mask11 = 255 - img_mask01;

		inRange(img_hsv, lower_2_1, upper_2_1, i4); inRange(img_hsv, lower_2_2, upper_2_2, i5); inRange(img_hsv, lower_2_3, upper_2_3, i6);

		img_mask02 = i4 | i5 | i6;
		img_mask12 = 255 - img_mask02;

		inRange(img_hsv, lower_3_1, upper_3_1, i7); inRange(img_hsv, lower_3_2, upper_3_2, i8); inRange(img_hsv, lower_3_3, upper_3_3, i9);

		img_mask03 = i7 | i8 | i9;
		img_mask13 = 255 - img_mask03;

		seat_mask_01 = img_mask11 & img_mask12 & img_mask13;

		//좌석 2 색 설정

		inRange(img_hsv, lower_4_1, upper_4_1, i10); inRange(img_hsv, lower_4_2, upper_4_2, i11); inRange(img_hsv, lower_4_3, upper_4_3, i12);

		img_mask04 = i10 | i11 | i12;
		img_mask14 = 255 - img_mask04;

		inRange(img_hsv, lower_5_1, upper_5_1, i13); inRange(img_hsv, lower_5_2, upper_5_2, i14); inRange(img_hsv, lower_5_3, upper_5_3, i15);

		img_mask05 = i13 | i14 | i15;
		img_mask15 = 255 - img_mask05;

		inRange(img_hsv, lower_6_1, upper_6_1, i16); inRange(img_hsv, lower_6_2, upper_6_2, i17); inRange(img_hsv, lower_6_3, upper_6_3, i18);

		img_mask06 = i16 | i17 | i18;
		img_mask16 = 255 - img_mask06;

		seat_mask_02 = img_mask14 & img_mask15 & img_mask16;

		int morph_size = 2;
		Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
		Mat img_labels, stats, centroids;

		morphologyEx(seat_mask_01, seat_mask_01, MORPH_OPEN, element);																												//좌석 1 객체 인식
		morphologyEx(seat_mask_01, seat_mask_01, MORPH_CLOSE, element);

		int numOfLabels = connectedComponentsWithStats(seat_mask_01(Range(SEAT1yB, SEAT1yT), Range(SEAT1xB, SEAT1xT)), img_labels, stats, centroids, 8, CV_32S);

		for (int j = 1; j < numOfLabels; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			int left = stats.at<int>(j, CC_STAT_LEFT);
			int top = stats.at<int>(j, CC_STAT_TOP);
			int width = stats.at<int>(j, CC_STAT_WIDTH);
			int height = stats.at<int>(j, CC_STAT_HEIGHT);

			int centerX = centroids.at<double>(j, 0);
			int centerY = centroids.at<double>(j, 1);

			if (area > 100 ) {																																											// 객체의 제한 크기 부분 : 10000 이상
				circle(img_color, Point(SEAT1xB + centerX, SEAT1yB + centerY), 5, Scalar(255, 0, 0), 1);
				rectangle(img_color, Point(SEAT1xB + left, SEAT1yB + top), Point(SEAT1xB + left + width, SEAT1yB + top + height), Scalar(0, 0, 255, 1));
				checkCenter(SEAT1xB + centerX, SEAT1yB + centerY, 1);
			}
		}

		morphologyEx(seat_mask_02, seat_mask_02, MORPH_OPEN, element);																												//좌석 2 객체 인식
		morphologyEx(seat_mask_02, seat_mask_02, MORPH_CLOSE, element);

		numOfLabels = connectedComponentsWithStats(seat_mask_02(Range(SEAT2yB, SEAT2yT), Range(SEAT2xB, SEAT2xT)), img_labels, stats, centroids, 8, CV_32S);

		for (int j = 1; j < numOfLabels; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			int left = stats.at<int>(j, CC_STAT_LEFT);
			int top = stats.at<int>(j, CC_STAT_TOP);
			int width = stats.at<int>(j, CC_STAT_WIDTH);
			int height = stats.at<int>(j, CC_STAT_HEIGHT);

			int centerX = centroids.at<double>(j, 0);
			int centerY = centroids.at<double>(j, 1);

			if (area > 100 ) {																																											// 객체의 제한 크기 부분 : 10000 이상
				circle(img_color, Point(SEAT2xB + centerX, SEAT2yB + centerY), 5, Scalar(255, 0, 0), 1);
				rectangle(img_color, Point(SEAT2xB + left, SEAT2yB + top), Point(SEAT2xB + left + width, SEAT2yB + top + height), Scalar(0, 0, 255, 1));
				checkCenter(SEAT2xB + centerX, SEAT2yB + centerY, 2);
			}
		}

		gotoxy(0, 21); printf("Seat State");
		gotoxy(1, 22); printf("좌석 명칭 | 좌석 상태 ");
		gotoxy(1, 23); printf("SAET1");
		gotoxy(1, 24); printf("SEAT2");
		for (int i = 1; i < MAXSEAT; i++) {																																										//정보 표현
			if (check[i] == 0 && check_p[i] == 1 && record[i]%10 == 5) {
				gotoxy(13, 22+i); cout <<"손님없음" << endl;
				check_p[i] = 0;
				record[i] = 0;
			}else if (check[i] == 0 && check_p[i] == 1) {
				gotoxy(13, 22 + i); cout << "측정중임" << endl;
				record[i]++;
			}else if (check[i] == 1 && check_p[i] == 0 && record[i]/10 == 5) {
				gotoxy(13, 22 + i); cout << "손님있음" << endl;
				check_p[i] = 1;
				record[i] = 0;
			}else if (check[i] == 1 && check_p[i] == 0) {
				gotoxy(13, 22 + i); cout << "측정중임" << endl;
				record[i] = record[i] + 10;
			}else {
				record[i] = 0;
			}
		}
		for (int i = 1; i < MAXSEAT; i++) {
			check[i] = 0;
		}

		putText(seat_mask_01, "01", Point(30, 30), 5, 1, Scalar(255, 0, 0), 1);
		putText(seat_mask_02, "02", Point(30, 30), 5, 1, Scalar(255, 0, 0), 1);

		Mat integrated;
		hconcat(seat_mask_01, seat_mask_02, integrated);

		imshow("designation window", img_color);																																										//창 설정
		imshow("management window", integrated);

		if (waitKey(1) > 0)
			break;
	}
	return 0;
}