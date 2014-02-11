#include "assert.h"
#include "otsu.h"

otsu::otsu(Mat _img, uchar _modes):
  img(_img), modes(_modes), max_val(0)
{
  calc_hist();
  calc_universalset();
  calc_record();

  counter = 0;
  generate_thres_index(0, modes, &otsu::print_vector);
  generate_thres_index(0, modes, &otsu::exec);
  std::cout<<std::endl;
  counter = 0;
  print_vector(best_subset);
}

void otsu::calc_hist()
{
  int col = img.cols;
  int row = img.rows;
  if(img.isContinuous())
  {
    col *= row;
    row = 1;
  }
  for(size_t i = 0; i < row; ++i)
  {
    uchar* p = img.ptr<uchar>(i);
    for(size_t j = 0; j < col; ++j)
      hist[p[j]]++;
  }
}

void otsu::calc_universalset()
{
  for(size_t i = 0; i <= P_MAX; ++i)
    if(hist[i])
      universal_set.push_back(i);
  counter = 0;
  print_vector(universal_set);
  std::cout<<std::endl<<"Size: "<<universal_set.size()<<std::endl;
}

void otsu::calc_record()
{
  //begin == 0
  moment0[0] = hist[0];
  moment1[0] = 0;

  for(int end = 1; end <= P_MAX; ++end)
  {
    moment0[end] = moment0[end-1] + hist[end];
    moment1[end] = moment1[end-1] + end*hist[end];
  }
}

float otsu::calc_intra(int begin, int end)
  //[begin, end)
{
  assert(begin >=0 && begin <= P_MAX);
  assert(end > 0 && end <= P_MAX+1);

  long m0 = moment0[end-1];
  long m1 = moment1[end-1];
  if(begin)
  {
    m0 -= moment0[begin-1];
    m1 -= moment1[begin-1];
  }
  return((float)m1/m0*m1);
}

void otsu::generate_thres_index(int _offset, int _mode, otsu_memfn_ptr _callback)
  //choose _mode-1 from [1,255]
  //thats 255C(_mode-1)
{
  assert(_mode >= 1);
  if(_mode == 1)
  {
    (this->*_callback)(indices);
    return;
  }
  for(int i = _offset; i <= universal_set.size() -_mode; ++i)
  {
    //the first index will always be in the combination, ignoring
    indices.push_back(universal_set[i+1]); 
    generate_thres_index(i+1, _mode-1, _callback);
    indices.pop_back();
  }
}

void otsu::print_vector(std::vector<int>&_vec)
  //debug function
{
  std::cout<<"["<<++counter<<"]: ";
  for(std::vector<int>::iterator i = _vec.begin(); i != _vec.end(); ++i)
    std::cout<<' ' <<*i<<'\t';
  //std::cou<<std::endl;
}

void otsu::exec(std::vector<int>&_vec)
{
  int begin = universal_set[0];
  long sum = 0;
  for(std::vector<int>::iterator end = _vec.begin(); end != _vec.end(); ++end)
  {
    sum += calc_intra(begin, *end);
    begin = *end;
  }
  sum += calc_intra(begin, P_MAX+1);
  if(sum > max_val)
  {
    max_val = sum;
    best_subset = _vec;
  }
}

int 
main(int argc, char const *argv[])
{
  size_t modes;

  assert(argc == 3);
  assert((modes = atoi(argv[2])) <= P_MAX+1);
  Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  otsu test(image, modes);
  return 0;
}
