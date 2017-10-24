#include <stdio.h>
#include <math.h>

GRAY_img *rotation(GRAY_img *img_in) ;
GRAY_img *rotation(GRAY_img *img_in){
  GRAY_img *img_out;
  img_out = allocGrayimg(img_in -> cols, img_in -> rows);

  //背景を白に初期化
  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 255;
    }
  }

  // 回転
  int x0 = 50;
  int y0 = 50;
  int rad = 45;
  for(int i = 0;i <( img_out -> cols);i++){
     for(int j = 0;j <( img_out -> rows);j++){
        int X = (int)((i-x0)*cos(rad)) - (int)((j-y0)*sin(rad)) + x0;
        int Y = (int)((i-x0)*sin(rad)) + (int)((j-y0)*cos(rad)) + y0;
        if( X<0 || Y<0 || X >= img_out -> cols || Y >= img_out -> rows ){
           img_out -> p[j][i] = 255;
        }else{
      　img_out -> p[j][i] = img_in -> p[(int)(floor(Y))][(int)(floor(X))];
        }
     }
  }
  return img_out;
  }
