#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>

#define P_MAX 255

using namespace cv;
class otsu 
{
  public:
    long  counter;
    Mat   img;
    uchar modes;
    long  moment0[P_MAX+1];
    long  moment1[P_MAX+1];
    long  hist[P_MAX+1]    = {0};

    long  max_val;
    std::vector<int> best_subset;

    otsu(Mat, uchar);
    float calc_intra(int, int);
    void  calc_universalset();
    void  generate_thres_index(int, int, void (otsu::*)(std::vector<int>&));
    void  print_vector(std::vector<int>&);
    void  exec(std::vector<int>&);
  private:
    void  calc_hist();
    void  calc_record();
    std::vector<int> indices;
    std::vector<int> universal_set;
};

typedef void (otsu::*otsu_memfn_ptr)(std::vector<int>&);
