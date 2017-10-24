/* File: pnmgrf.c, Time-stamp: <98/04/01 02:35:40 sumi>
 *
 * pnmgrf.c:	graphics functions for img structure.
 *
 *		written by Kazuhiko Sumi
 *
 *		NOTE: this program is an contribution to PRMU program
 *		contest and is not thoroughly tested. The author
 *		is not responsible to any problem using this code.
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

static RGB_PACKED_PIXEL mark_color ;

#ifdef __STDC__
void
setRGBMarkColor( int red, int green, int blue )
#else
void
setRGBMarkColor( red, green, blue )
     int red, green, blue ;
#endif
{
  mark_color.r = red ;
  mark_color.g = green ;
  mark_color.b = blue ;
}

#ifdef __STDC__
void
markDotRGBPackedimg( RGB_PACKED_img *img, int x, int y ) 
#else
void
markDotRGBPackedimg( img, x, y ) 
     RGB_PACKED_img *img ;
     int x, y ;
#endif
{
  if ( x >= 0 && x < img->cols && y >= 0 && y < img->rows ) {
    img->p[y][x].r = mark_color.r ;
    img->p[y][x].g = mark_color.g ;
    img->p[y][x].b = mark_color.b ;
  }
}

#ifdef __STDC__
void
drawLineRGBPackedimg( RGB_PACKED_img *img, int x1, int y1, int x2, int y2 )
#else
void
drawLineRGBPackedimg( img, x1, y1, x2, y2 )
     RGB_PACEKD_img *img ;
     int x1,y1;      /* start address */
     int x2,y2;      /* end address */
#endif
{
  int i, leng, xx, yy ;
  double x, y, xi, yi;
  
  leng = (abs(x2-x1)>abs(y2-y1)?abs(x2-x1):abs(y2-y1));
  xi = (double)(x2-x1)/(double)(leng);
  yi = (double)(y2-y1)/(double)(leng);
  x = (double)x1+0.5;
  y = (double)y1+0.5;
  
  for (i = 0; i <= leng; i++, x += xi, y += yi) {
    xx = x, yy = y ;
    markDotRGBPackedimg( img, xx, yy ) ;
  }
}

#ifdef __STDC__
void
drawPolygonRGBPackedimg( RGB_PACKED_img *img, int *xv, int *yv, int nv )
#else
void
drawPolygonRGBPackedimg( img, xv, yv, nv )
     RGB_PACKED_img *img ;
     int *xv, *yv, nv ;
#endif
{
  int i ;
  for ( i = 1 ; i < nv ; i++ ) {
    drawLineRGBPackedimg( img, xv[i-1], yv[i-1], xv[i], yv[i] ) ;
  }
  drawLineRGBPackedimg( img, xv[nv-1], yv[nv-1], xv[0], yv[0] ) ;
}

#ifdef __STDC__
void
drawRotatedRectangleRGBPackedimg( RGB_PACKED_img *img, 
				    int x0, int y0, int xs, int ys, int rot )
#else
void
drawRotatedRectangleRGBPackedimg( img, x0, y0, xs, ys, rot )
     RGB_PACKED_img *img ;
     int x0, y0;   /* center address */
     int xs, ys;   /* size of rectangular */
     int rot;      /* rotation in degree */
#endif
{
  int xv[4], yv[4] ; /* vertex */
  double sinR, cosR, dx, dy, dr ;
  double sin(), cos() ;

  dr = (double) rot * 3.14159265358979323846 / 180.0 ;
  dx = (double) xs / 2.0 ;
  dy = (double) ys / 2.0 ;
  sinR = sin( dr ) ;
  cosR = cos( dr ) ;

  xv[0] = x0 + (int)( - cosR * dx - sinR * dy ) ;
  yv[0] = y0 + (int)(   sinR * dx - cosR * dy ) ;
  xv[1] = x0 + (int)( - cosR * dx + sinR * dy ) ;
  yv[1] = y0 + (int)(   sinR * dx + cosR * dy ) ;
  xv[2] = x0 + (int)(   cosR * dx + sinR * dy ) ;
  yv[2] = y0 + (int)( - sinR * dx + cosR * dy ) ;
  xv[3] = x0 + (int)(   cosR * dx - sinR * dy ) ;
  yv[3] = y0 + (int)( - sinR * dx - cosR * dy ) ;

  drawPolygonRGBPackedimg( img, xv, yv, 4 ) ;
}


