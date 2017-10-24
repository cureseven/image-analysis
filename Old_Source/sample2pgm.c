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
  char *name_img = "sample.pgm" ;   /* ���ϲ����ե�����̾ */
  char *name_out = "output2.pgm" ;  /* ���ϲ����ե�����̾ */
  GRAY_img *img ; /* ���ǻø�����ѹ�¤�� */


/* ���ޥ�ɥ饤��ǥե�����̾��Ϳ����줿���ν��� */
  if ( argc >= 2 ) name_img = argv[1] ;
  if ( argc >= 3 ) name_out = argv[2] ;

/* ���ϲ����ե�����Υ����ץ�Ȳ����ǡ������� */
  if (!( img = readGrayimg( name_img ))) {
    printError( name_img ) ; /* (printError : "pnmerr.c"�ˤ��������Ƥ���) */
    return(1) ;
  }



/** �����˲����ν�����񤭹��� **/




/* ���ϲ����ե�����Υ����ץ�Ȳ����ǡ����ν���� */
  if ( writeGrayimg( img, name_out ) == HAS_ERROR ) {
    printError( name_out ) ;
    return(1) ;
  }

  return(0) ;
}

