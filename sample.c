#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include "pnmimg.h"
// プロトタイプ宣言
// #include "mv.c"
// #include "enlarge.c"
// #include "rotation.c"

#ifdef __STDC__
int
main( int argc, char *argv[] )
#else
int
main( argc, argv )
     int argc ;
     char *argv[] ;
#endif
{
	char *name_img_in = "sample.pgm" ;   /* 入力画像ファイル名 */
	char *name_img_out = "output.pgm" ;  /* 出力画像ファイル名 */
	GRAY_img *img_in ;
	GRAY_img *img_out ;


	/* コマンドラインでファイル名が与えられた場合の処理 */
	if ( argc >= 2 ) name_img_in = argv[1] ;
	if ( argc >= 3 ) name_img_out = argv[2] ;

  /* カラー画像を読み込んでくる */
	if (!( img_in = readGrayimg( name_img_in ))) {
		printError( name_img_in ) ;
		return(1) ;
	}

  /* 空の画像を作る */
	img_out = allocGrayimg(img_in -> cols, img_in -> rows);


  // 処理
  int x0 = 50;
  int y0 = 50;
  int rad = 45;
  float rx = 2.3;
  float ry = 2.3;
  for(int i = 0;i <( img_out -> cols);i++){
     for(int j = 0;j <( img_out -> rows);j++){
        int X = (int)(((i-x0)*cos(rad)) - (int)((j-y0)*sin(rad)) + x0)/rx;
        int Y = (int)(((i-x0)*sin(rad)) + (int)((j-y0)*cos(rad)) + y0)/ry;
        if( X<0 || Y<0 || X >= img_out -> cols || Y >= img_out -> rows ){
           img_out -> p[j][i] = 255;
        }else{
      　img_out -> p[j][i] = img_in -> p[(int)(floor(Y))][(int)(floor(X))];
        }
     }
  }




  /* グレー画像を作成 */
	if ( writeGrayimg( img_out, name_img_out ) == HAS_ERROR ) {
    	printError( name_img_out ) ;
    	return(1) ;
  	}

	return(0) ;
}
