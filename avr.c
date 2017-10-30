#include <stdio.h>
#include <math.h>

GRAY_img *avr(GRAY_img *img_in) ;
GRAY_img *avr(GRAY_img *img_in){
  GRAY_img *img_out;
  img_out = allocGrayimg(img_in -> cols, img_in -> rows);

  //背景を白に初期化
  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 255;
    }
  }

  // 処理
  int x,y;
  // double rx,ry;
  double d;

  for(int i = 0;i <( img_out -> cols);i++){
     for(int j = 0;j <( img_out -> rows);j++){
        if( i<=0 || j<=0 || i >= img_out -> cols - 1 || j >= img_out -> rows - 1 ){
           img_out -> p[j][i] = 0;
        }else{
        d = (1.0/9.0)*img_in->p[j-1][i-1] + (1.0/9.0)*img_in->p[j-1][i] + (1.0/9.0)*img_in->p[j-1][i+1] + (1.0/9.0)*img_in->p[j][i-1] + (1.0/9.0)*img_in->p[j][i]
        + (1.0/9.0)*img_in->p[j][i+1] + (1.0/9.0)*img_in->p[j+1][i-1] + (1.0/9.0)*img_in->p[j+1][i] + (1.0/9.0)*img_in->p[j+1][i+1];
        if(d<0){
          img_out-> p[j][i] = 0;
        }else if(d > 255){
          img_out-> p[j][i] = 255;
        }else{
          img_out -> p[j][i] = d;
        }
        img_out->p[j][i] = 255 - img_out->p[j][i];

        }
     }
}


  return img_out;
  }
