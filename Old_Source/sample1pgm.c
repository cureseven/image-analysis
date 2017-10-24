#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

#include <math.h>


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
  char *name_out = "output1.pgm" ;  /* ���ϲ����ե�����̾ */
  GRAY_img *img ; /* ���ǻø�����ѹ�¤�� */
  int cols;  /* �����β��� */
  int rows;  /* �����ν��� */
  int x, y;


/** ���ޥ�ɥ饤��ǥե�����̾��Ϳ����줿���ν��� **/
  if ( argc >= 2 ) name_out = argv[1] ;

/** �������� **/
/* 1. �����ΰ���� */
  cols = 320;  rows = 240;  /* ��������������礭������� */
  img = allocGrayimg(cols, rows);  /* ����������� */

/* 2. �礭��200x200�ζ�������� */
  for(y=0; y<200; y++){
    for(x=0; x<200; x++){
      img->p[y][x] = 
	255*sqrt(x*x+y*y)/sqrt(200*200+200*200);  /* �����ͤ򥻥å� */
    }
  }


/** ���ϲ����ե�����Υ����ץ�Ȳ����ǡ����ν���� **/
  if ( writeGrayimg( img, name_out ) == HAS_ERROR ) {
    printError( name_out ) ;
    return(1) ;
  }

  return(0) ;
}

