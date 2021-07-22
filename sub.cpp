#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img_color;

void mouse_callback(int event, int x, int y, int flags, void* param);

int main()
{
	namedWindow("img_color");
	setMouseCallback("img_color", mouse_callback);

	VideoCapture cap(1);																																												// 카메라 설정 부분 : 현재 0번째 카메라

	if (!cap.isOpened()) {
		cout << "카메라를 열 수 없습니다." << endl;
		return -1;
	}

	while (1) {
		cap.read(img_color);
		imshow("img_color", img_color);

		if (waitKey(1) > 0)
			break;
	}
	return 0;
}

void mouse_callback(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "해당 위치의 좌표" << x << endl;
		cout << " x = " << x << endl;
		cout << " y = " << y << endl;
	}
}