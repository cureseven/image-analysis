#include <stdio.h>
GRAY_img *enlarge(GRAY_img *img_in) ;
GRAY_img *enlarge(GRAY_img *img_in){
  GRAY_img *img_out;
  img_out = allocGrayimg(img_in -> cols, img_in -> rows);

  //背景を白に初期化
  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 255;
    }
  }

//拡大
float rx = 2.3;
float ry = 2.3;
for(int i = 0;i <( img_out -> cols);i++){
   for(int j = 0;j <( img_out -> rows);j++){
      int X = i / rx;
      int Y = j / ry;
      if( X<0 || Y<0 || X >= img_out -> cols || Y >= img_out -> rows ){
         img_out -> p[j][i] = 255;
      }else{
    　img_out -> p[j][i] = img_in -> p[Y][X];
      }
   }
}

return img_out;
}
