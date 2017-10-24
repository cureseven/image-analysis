#include <stdio.h>
GRAY_img *mv(GRAY_img *img_in) ;
GRAY_img *mv(GRAY_img *img_in){
  GRAY_img *img_out;
  img_out = allocGrayimg(img_in -> cols, img_in -> rows);

  //背景を白に初期化
  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 255;
    }
  }

  //移動
int x0 = -20;
int y0 = -40;
for(int i = 0;i <( img_out -> cols);i++){
   for(int j = 0;j <( img_out -> rows);j++){
      int X = i + x0;
      int Y = j + y0;
      if(X<0||Y<0){
         img_out -> p[j][i] = 255;
      }else{
    　img_out -> p[j][i] = img_in -> p[j][i];
      }
   }
 }



  //回転
  // int sx = 20;
  // int sy = 30;
  //
  // for(int i = 0; img_out -> cols; i++){
  //   for(int j = 0; img_out -> rows; j++){
  //     img_out -> p[j][i] = (1-sy)*{(1-sx)*(img_in -> p[j][i]) +
  //       sx*(img_in[j][i+1])}+sy{(1-sx)(img_in -> p[j+1][i] +
  //         sx(img_in[j+1][i+1]))};
  //   }
  // }


  return img_out;
}
