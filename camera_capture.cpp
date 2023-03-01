
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;
// #define HEIGHT_RGB 1080
// #define WIDTH_RGB 960
#define HEIGHT_RGB 896
#define WIDTH_RGB 1600

struct TimeCost {
	std::string function_name;
    uint64_t lasttime = 0;
	uint64_t min = 100000;
	uint64_t max = 0;
	uint64_t totalcost = 0;
    uint64_t firstcost = 0;
	uint64_t count = 0;
	bool isfirst;

	TimeCost(std::string function_name);
	void AveCost();
};

inline void logA(const char* format, ...)
{
    char buf[1024];
    wvsprintfA(buf, format, ((char*)&format) + sizeof(void*));
    OutputDebugStringA(buf);
}

uint64_t GetTickCountMS() {
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    return (uint64_t)(std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count());
}

TimeCost::TimeCost(std::string function_name):function_name(function_name){
	count = 0;
	totalcost = 0;
	isfirst = true;
	lasttime = GetTickCountMS();
}

void TimeCost::AveCost() {
    uint64_t time = GetTickCountMS();
    totalcost += time - lasttime;
	lasttime = time;
    count++;
    if (isfirst) {
		firstcost = totalcost;
        totalcost = 0;
		isfirst = false;
		std::cout << "function_name firstcost: " << firstcost/1000 << "ms" <<std::endl;
		logA("%s firstcost: %d ms\n", function_name, firstcost/1000);
    }
    if (0 == count%100) {
		std::cout << "function_name avgcost: " << totalcost/1000/count << "ms" <<std::endl;
		logA("%s avgcost: %d ms FPS: %f \n", function_name, totalcost/1000/count, 1000000.0/(totalcost/count));
    }
}

int main(int argc, char** argv)
{
    if(argc<2){
        cout << "please set arg camera id" << endl;
        return -1;
    }
    TimeCost cost(__FUNCTION__);
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[1], &p, 10);
    // Check for errors: e.g., the string does not represent an integer
    // or the integer is larger than int
    if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) {
        cout << "please set correct camera id" << endl;
        return -2;
    } else {
        // No error
        num = conv;
    }
    cout << "open camera " <<num<< endl;

    // cv::VideoCapture cap(num);
    cv::VideoCapture cap;
    cap.open(num);
    if(!cap.isOpened()) {
        return -3;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 2*WIDTH_RGB);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HEIGHT_RGB);
    cap.set(cv::CAP_PROP_FPS, 30);
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cout << "CAP_PROP_FRAME_WIDTH: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    std::unique_ptr<uint8_t[]>iarray(new uint8_t[2*WIDTH_RGB*HEIGHT_RGB*4]);
    cost.AveCost();
    while(true) {
    // for(int i = 0; i<120; i++) {
        cost.AveCost();
        cv::Mat zedmat(HEIGHT_RGB, 2*WIDTH_RGB, CV_8UC4, iarray.get());
        cv::Mat img;
        cap >> img;
        if(img.cols != width){
            cout << "img.cols: " << img.cols << endl;
            continue;
        }
        cv::resize(img, img, cv::Size(2*WIDTH_RGB, HEIGHT_RGB));
        cv::cvtColor(img,zedmat,cv::COLOR_BGR2BGRA);
        // cv::imshow("zedmat",zedmat);
        // cv::waitKey(1);
        // newname = path + os.sep + '{:0>2}'.format(n+1) +'.png';
        // print(newname);
        // cv2.imwrite(newname,zedmat);
    }
    cap.release();
    return 0;
}