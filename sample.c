#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"
// プロトタイプ宣言
// #include "mv.c"
#include "enlarge.c"

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
　img_out = enlarge(img_in);


  /* グレー画像を作成 */
	if ( writeGrayimg( img_out, name_img_out ) == HAS_ERROR ) {
    	printError( name_img_out ) ;
    	return(1) ;
  	}

	return(0) ;
}
