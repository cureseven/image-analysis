#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

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
	char *name_img_in = "sample.ppm" ;   /* 入力画像ファイル名 */
	char *name_img_out = "output.pgm" ;  /* 出力画像ファイル名 */
	RGB_PACKED_img *img_in ;
	GRAY_img *img_out ;


	/* コマンドラインでファイル名が与えられた場合の処理 */
	if ( argc >= 2 ) name_img_in = argv[1] ;
	if ( argc >= 3 ) name_img_out = argv[2] ;

	if (!( img_in = readRGBPackedimg( name_img_in ))) {　/* カラー画像を読み込んでくる */
		printError( name_img_in ) ;
		return(1) ;
	}

	img_out = allocGrayimg(img_in -> cols, img_in -> rows); /* 空の画像を作る */

	/** ここに画像の処理を書き込む **/
  printf("R:%d\n", img_in -> p[20][100].r);
  printf("G:%d\n", img_in -> p[20][100].g);
  printf("B:%d\n", img_in -> p[20][100].b);
  printf("%d\n", img_out -> p[20][100]);
  // img_out -> p[20][100] = 100;
  printf("縦のサイズ：%d\n", img_in -> rows);
  printf("横のサイズ：%d\n", img_in -> cols);

  for(int i = 0; i < img_out -> cols; i++){
    for(int j = 0; j < img_out -> rows; j++){
      img_out -> p[j][i] = 0.2126*(img_in -> p[j][i].r) + 0.7152*(img_in -> p[j][i].g) + 0.0722*(img_in -> p[j][i].b);
    }
  }

	if ( writeGrayimg( img_out, name_img_out ) == HAS_ERROR ) { /* グレー画像を作成 */
    	printError( name_img_out ) ;
    	return(1) ;
  	}

	return(0) ;
}
