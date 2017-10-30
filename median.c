#include <stdio.h>
#include <math.h>

GRAY_img *median(GRAY_img *img_in) ;
GRAY_img *median(GRAY_img *img_in){
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
  int s,k;
  // double rx,ry;
  double d;
  int q = 0;
  BYTE c;
  BYTE m[9];


  for(int i = 0;i <( img_out -> cols);i++){
     for(int j = 0;j <( img_out -> rows);j++){
        if( i<=0 || j<=0 || i >= img_out -> cols - 1 || j >= img_out -> rows - 1 ){
           img_out -> p[j][i] = 0;
        }else{
        for(s = i-1;s < i+2 ; s++){
          for(k = j-1; k<j+2; k++){
            m[q] = img_in->p[k][s];
            q++;
          }
        }
        q = 0;
        for(s = 1; s < 8; s++){
          for(k = 8; k>s; k--){
            if(m[k-1]>m[k]){
              c = m[k];
              m[k] = m[k-1];
              m[k-1] = c;
            }
          }
          img_out ->p[j][i] = m[4];
        }
     }
   }
}


  return img_out;
  }
