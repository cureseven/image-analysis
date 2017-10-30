#include <stdio.h>
#include <math.h>

GRAY_img *liner(GRAY_img *img_in) ;
GRAY_img *liner(GRAY_img *img_in){
  GRAY_img *img_out;
  img_out = allocGrayimg(img_in -> cols, img_in -> rows);

  //背景を白に初期化
  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 255;
    }
  }

  // 回転
  int X,Y;/*変換後の座標*/
	int x,y;/*元画像の座標*/
	int a = 60;
	int b = 28;
  double d = 45;
	// double sin(double d);
	// double cos(double d);
	float sx,sy;

	for(X=0;X<img_in->cols;X++){
	  for(Y=0;Y<img_in->rows;Y++){
	    x = (int)((X-a)*cos(d)) - (int)((Y-b)*sin(d)) + a;
	    y = (int)((X-a)*sin(d)) + (int)((Y-b)*cos(d)) + b;

	    sx = x - floor(x);
	    sy = y - floor(y);

	    x = (int)(floor(x));
	    y = (int)(floor(y));

	    if(x<0 || y<0 || x>img_in->cols-1 || y>img_in->rows-1){
	      img_out->p[Y][X] = 255;
	    }
	    else{
	    img_out->p[Y][X] = (1-sy)*((1-sx)*img_in->p[y][x]+sx*img_in->p[y][x+1])
      + sy*((1-sx)*img_in->p[y+1][x]+sx*img_in->p[y+1][x+1]);
	    }
	  }
	}
  
  return img_out;
  }
