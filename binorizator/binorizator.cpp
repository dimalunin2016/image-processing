#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

Mat gray_scale_image(const Mat& image, double red_coeff = 0.299, double green_coeff = 0.587) {
     cv::Size s = image.size();
     int N = s.height;
     int M = s.width;
     Mat grey(image.size(), CV_64F);
     grey = 0;
     double blue_coeff = 1 - red_coeff - green_coeff;
     //std::cout <<<< "Here\n";
     for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
       //     std::cout <<i <<" "<<j <<"\n";
            Vec3b colour = image.at<Vec3b>(Point(j, i));
            grey.at<double>(Point(j, i)) = (red_coeff * static_cast<double>(colour[2]) / 255 +
                green_coeff * static_cast<double>(colour[1]) / 255 + blue_coeff * static_cast<double>(colour[0]) / 255);
        }
     }
     return grey;
}

Mat intagrate_image(const Mat& grey) {
    Mat I(grey.size(), CV_64F);
    cv::Size s = grey.size();
    int N = s.height;
    int M = s.width;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (i == 0 || j == 0) {
                I.at<double>(Point(j, i)) = grey.at<double>(Point(j, i));
            } else {
                I.at<double>(Point(j, i)) = grey.at<double>(Point(j, i)) + I.at<double>(Point(j - 1, i)) +
                    I.at<double>(Point(j, i - 1)) - I.at<double>(Point(j - 1, i - 1));
            }
        }
    }
    return I;
}

Mat make_Bradley_answer(const Mat &I, const Mat &grey, double t_proc = 0.85) {
    cv::Size s = grey.size();
    int N = s.height;
    int M = s.width;
    Mat answer(grey.size(), CV_8UC1);
    answer = 0;
    double dh = 1.0 / 8 * N;
    double dw = 1.0 / 8 * M;
    int d = static_cast<int>(min(dh, dw) / 2);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
//            std::cout << i << " "<<j<<"\n";
            int imax = min(i + d, N - 1);
            int jmax = min(j + d, M - 1);
            int imin = max(i - d, 0);
            int jmin = max(j - d, 0);
            int size = (imax - imin) * (jmax - jmin);
            double summ = I.at<double>(Point(jmax, imax));
            if (imin >= 1) {
                summ -= I.at<double>(Point(jmax, imin - 1));
            }
            if (jmin >= 1) {
                summ -= I.at<double>(Point(jmin - 1, imax));
            }
            if (imin >= 1 && jmin >= 1) {
                summ += I.at<double>(Point(jmin - 1, imin - 1));
            }
            if (grey.at<double>(Point(j, i)) >= (summ / size) * t_proc) {
                //cout <<i <<" "<<j<<"here\n";
                answer.at<uchar>(Point(j, i)) = 255;
                //cout <<"there\n";
            }
        }
    }
    return answer;
}

Mat Get_Bradley_Binorization(const Mat &image, double t_proc = 0.85, double red_coeff = 0.299, double green_coeff = 0.587) {
    Mat grey = gray_scale_image(image, red_coeff, green_coeff);
    Mat I = intagrate_image(grey);
    Mat answer = make_Bradley_answer(I, grey, t_proc);
    return answer;
}

int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout <<"Please, enter file to binarize and name of the result" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image: " << argv[1] << std::endl ;
        return -1;
    }
    const clock_t begin_time = clock();
    Mat grey = Get_Bradley_Binorization(image, 0.9);
    double dt = static_cast<double>( clock () - begin_time ) /  CLOCKS_PER_SEC;
    double megapixels = static_cast<double>(image.size().height * image.size().width) / 1000000.0;
    std::cout <<"image: "<< argv[1]<<" - "<< "sec/megapixel: " << dt / megapixels <<"\n";
    imwrite(argv[2], grey);
    return 0;
}
