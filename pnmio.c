/* File: pnmio.c, Time-stamp: <98/04/01 02:35:23 sumi>
 *
 * pnmio.c:	img creation/read/write functions
 *
 *		written by Kazuhiko Sumi
 *
 *		NOTE: this program is an contribution to PRMU program
 *		contest and is not thoroughly tested. The author
 *		is not responsible to any problem using this code.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include "pnmimg.h"

#define RBUFLEN		1026
static char readBuf[ RBUFLEN ] ;

#ifdef __STDC__
char *
nextWord( FILE *fp )
#else
char *
nextWord( fp )
     FILE *fp ;
#endif
{
  int idx ;
  int ch ;

  /* skip spaces */
  while( isspace( ch = getc( fp ))) {}

  /* push back one character, it is not a space */
  ungetc( ch, fp ) ;

  /* read a word into readBuf */
  for( idx = 0 ; idx < RBUFLEN - 1 ; idx++ ) {
    ch = getc( fp ) ;
    if ( ch == EOF ) {
      if ( idx ) { readBuf[idx] = NULL ; return( readBuf ) ; }
      return((char*)NULL) ;
    }
    if ( isspace( ch )) {
      readBuf[ idx ] = '\0' ;
      return( readBuf ) ;
    }
    else if ( ch == '#' ) {
      readBuf[0] = '#' ; readBuf[1] = '\0' ; return( readBuf ) ;
    }
    else {
      readBuf[ idx ] = (char) ch ;
    }
  }
  /* warning: buffer overflowed */
  readBuf[ RBUFLEN - 1 ] = '\0' ;
  return( readBuf ) ;
}

#ifdef __STDC__
char *
readPbmComment( FILE *fp )
#else
char *
readPbmComment( fp )
     FILE *fp ;
#endif
{
  int idx ;
  int ch ;

  /* read a line into readBuf */
  for( idx = 0 ; idx < RBUFLEN - 1 ; idx++ ) {
    ch = getc( fp ) ;
    if ( ch == EOF ) {
      readBuf[ idx ] = '\0' ; return( readBuf ) ;
    }
    if ( ch == '\n' ) {
      readBuf[ idx++ ] = '\n' ; readBuf[ idx ] = '\0' ; return( readBuf ) ;
    }
    readBuf[ idx ] = (char) ch ;
  }
  readBuf[ RBUFLEN - 1 ] = '\0' ;
  return( readBuf ) ;
}

#ifdef __STDC__
int
readPnmHeader( FILE *fp, int *cols, int *rows, int *maxval )
#else
int
readPnmHeader( fp, cols, rows, maxval )
     FILE *fp ;
     int *cols, *rows, *maxval ;
#endif
{
  char *key ;
  int  step ;
  int  encording ;
  int  type ;

  if ( !( key = nextWord( fp )))
    return( setError( E_FILE_EOF )) ;
  if ( strncmp( key, "P2", 2 ) == 0 ) {
    type = PGM ; encording = ASCII ; 
  } else if ( strncmp( key, "P5", 2 ) == 0 ) {
    type = PGM ; encording = BINARY ;
  } else if ( strncmp( key, "P3", 2 ) == 0 ) {
    type = PPM ; encording = ASCII ;
  } else if ( strncmp( key, "P6", 2 ) == 0 ) {
    type = PPM ; encording = BINARY ;
  } else if (( strncmp( key, "P1", 2 ) == 0 )
           ||( strncmp( key, "P4", 2 ) == 0 )) {
    return( setError( E_FILE_PBM )) ;
  } else {
    return( setError( E_FILE_NOTPNM )) ;
  }

  for ( step = 0 ; step < 3 ; ) {
    key = nextWord( fp ) ;
    if ( !key ) {
      return( setError( E_FILE_EOF )) ;
    }
    if ( *key == '#' ) { /* beginning of comment, read and discard it */
      readPbmComment( fp ) ;
    }
    else if ( isdigit( *key )) {
      switch( step ) {
        case 0 : sscanf( key, "%d", cols ) ; break ;
        case 1 : sscanf( key, "%d", rows ) ; break ;
        case 2 : sscanf( key, "%d", maxval ) ; break ;
      }
      step ++ ;
    }
    else {
      return( setError( E_FILE_FORM )) ;
    }
  }
  return( type | encording ) ;
}

#ifdef __STDC__
GRAY_img *
allocGrayimg( int cols, int rows )
#else
GRAY_img *
allocGrayimg( cols, rows )
     int cols ;
     int rows ;
#endif
{
  GRAY_img *img ;
  int i ;

  if (!( img = (GRAY_img*)malloc( sizeof(GRAY_img)))) {
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->p = (BYTE**)malloc( rows * sizeof(BYTE*)))) {
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->data_p = (BYTE*)malloc( cols * rows * sizeof(BYTE)))) {
    free( img->p ) ;
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  img->cols = cols ;
  img->rows = rows ;
  for ( i = 0 ; i < rows ; i++ ) {
    img->p[i] = &( img->data_p[ i * cols ] ) ;
  }
  return( img ) ;
}

#ifdef __STDC__
void
freeGrayimg( GRAY_img *img )
#else
void
freeGrayimg( img )
     Grayimg *img ;
#endif
{
  free( img->data_p ) ;
  free( img->p ) ;
  free( img ) ;
}

#ifdef __STDC__
GRAY_img *
readGrayimg( char *name )
#else
GRAY_img *
readGrayimg( name )
     char *name ;
#endif
{
  int rows ;
  int cols ;
  int maxval ;
  int size ;
  int ret ;
  int numio ;
  int nread ;
  int idx ;
  char *arg ;
  GRAY_img *img ;
  FILE *fp ;
  int ch ;

/*  if (!( fp = fopen( name, "r" ))) {*/
  if (!( fp = fopen( name, "rb" ))) {
/*  DOS�ǤΥե����륪���ץ�⡼�ɤ��������ʤ�褦���������ޤ�����
                    (�����Ŵ� �渦  �����Ȫ) */
    setError( E_FILE_OPEN ) ;
    printError( name ) ;
    return(0) ;
  }
  if (( ret = readPnmHeader( fp, &cols, &rows, &maxval )) == HAS_ERROR ) {
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  if (( ret & PGM ) != PGM ) {
    setError( E_FILE_FORM ) ;
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  if ( maxval < 16 || maxval >= 256 ) {
    setError( E_FILE_MAXVAL ) ;
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  if (!( img = allocGrayimg( cols, rows ))) {
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  size = rows * cols ;
  if ( ret & BINARY ) {
    /*
     * input BINARY encorded PGM img
     */
    for( numio = 0 ; numio < size ; numio += nread ){
      nread = fread( &img->data_p[numio], 1, size - numio, fp ) ;
      if ( nread <= 0 ) {
	setError( E_FILE_READ ) ;
	printError( name ) ;
	freeGrayimg( img ) ;
	fclose( fp ) ;
	return(0) ;
      }
    }
  }
  else {
    /*
     * input ASCII encorded PGM img
     */
    for ( idx = 0 ; idx < size ; ) {
      if (( arg = nextWord( fp )) == (char*)NULL ) {
	setError( E_FILE_EOF ) ;
      }
      else if ( ! isdigit( *arg )) {
        if ( *arg == '#' ) { /* skip over comment line */
          while(( ch = getc( fp )) != '\n' && ch != EOF )
	    ;
        }
	else {	/* data must be digit numbers. can't happen */
	  setError( E_FILE_INVALID ) ;
	}
      }
      else
	img->data_p[idx++] = atoi( arg ) ;
      if ( checkError()) {
	printError( name ) ;
	freeGrayimg( img ) ;
	fclose( fp ) ;
	return(0) ;
      }
    }
  }
  fclose( fp ) ;
  return( img ) ;
}

#ifdef __STDC__
int
writeGrayimg( GRAY_img *img, char *name )
#else
int
writeGrayimg( img, name )
     GRAY_img *img ;
     char *name ;
#endif
{
  FILE *fp ;
  int idx ;
  BYTE *ptr ;

/*  if (!( fp = fopen( name, "w" ))) {*/
  if (!( fp = fopen( name, "wb" ))) {
/*  DOS�ǤΥե����륪���ץ�⡼�ɤ��������ʤ�褦���������ޤ�����
                    (�����Ŵ� �渦  �����Ȫ) */
    setError( E_FILE_OPEN ) ;
    printError( name ) ;
    return( HAS_ERROR ) ;
  }

  fprintf( fp, "P5\n%d %d\n255\n", img->cols, img->rows ) ;
  ptr = img->data_p ;
  for ( idx = 0 ; idx < img->rows ; idx++, ptr+=img->cols ) {
    if ( fwrite( ptr, 1, img->cols, fp ) != img->cols ) {
      setError( E_FILE_WRITE ) ;
      fclose( fp ) ;
      return( HAS_ERROR ) ;
    }
  }
  fclose( fp ) ;
  return( NO_ERROR ) ;
}

#ifdef __STDC__
GRAY_LONG_img *
allocGrayLongimg( int cols, int rows )
#else
GRAY_LONG_img *
allocGrayLongimg( cols, rows )
     int cols ;
     int rows ;
#endif
{
  GRAY_LONG_img *img ;
  int i ;

  if (!( img = (GRAY_LONG_img*)malloc( sizeof(GRAY_LONG_img)))) {
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->p = (long**)malloc( rows * sizeof(long*)))) {
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->data_p = (long*)malloc( cols * rows * sizeof(long)))) {
    free( img->p ) ;
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  img->cols = cols ;
  img->rows = rows ;
  for ( i = 0 ; i < rows ; i++ ) {
    img->p[i] = &( img->data_p[ i * cols ] ) ;
  }
  return( img ) ;
}

#ifdef __STDC__
void
freeGrayLongimg( GRAY_LONG_img *img )
#else
void
/*freeGrayimg( img )*/
freeGrayLongimg( img )
/* �ؿ�̾���ְ�äƤ���Τ�ľ���ޤ�����(�����Ŵ� �渦  �����Ȫ)*/
     GRAY_LONG_img *img ;
#endif
{
  free( img->data_p ) ;
  free( img->p ) ;
  free( img ) ;
}

#ifdef __STDC__
GRAY_DOUBLE_img *
allocGrayDoubleimg( int cols, int rows )
#else
GRAY_DOUBLE_img *
allocGrayDoubleimg( cols, rows )
     int cols ;
     int rows ;
#endif
{
  GRAY_DOUBLE_img *img ;
  int i ;

  if (!( img = (GRAY_DOUBLE_img*)malloc( sizeof(GRAY_DOUBLE_img)))) {
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->p = (double**)malloc( rows * sizeof(double*)))) {
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->data_p = (double*)malloc( cols * rows * sizeof(double)))) {
    free( img->p ) ;
    free( img ) ;
    setError( E_MALLOC ) ;
    return(0) ;
  }
  img->cols = cols ;
  img->rows = rows ;
  for ( i = 0 ; i < rows ; i++ ) {
    img->p[i] = &( img->data_p[ i * cols ] ) ;
  }
  return( img ) ;
}

#ifdef __STDC__
void
freeGrayDoubleimg( GRAY_DOUBLE_img *img )
#else
void
/*freeGrayimg( img )*/
freeGrayDoubleimg( img )
/* �ؿ�̾���ְ�äƤ���Τ�ľ���ޤ�����(�����Ŵ� �渦  �����Ȫ)*/
     GRAY_DOUBLE_img *img ;
#endif
{
  free( img->data_p ) ;
  free( img->p ) ;
  free( img ) ;
}

#ifdef __STDC__
RGB_PACKED_img *
allocRGBPackedimg( int cols, int rows )
#else
RGB_PACKED_img *
allocRGBPackedimg( cols, rows )
     int cols ;
     int rows ;
#endif
{
  RGB_PACKED_img *img ;
  int i ;

  if (!( img = (RGB_PACKED_img*)malloc( sizeof(RGB_PACKED_img)))) {
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->p = 
	(RGB_PACKED_PIXEL**)malloc( rows * sizeof(RGB_PACKED_PIXEL*)))) {
    setError( E_MALLOC ) ;
    free( img ) ;
    return(0) ;
  }
  if (!( img->data_p = 
	(RGB_PACKED_PIXEL*)malloc( cols * rows * sizeof(RGB_PACKED_PIXEL)))) {
    setError( E_MALLOC ) ;
    free( img->p ) ;
    free( img ) ;
    return(0) ;
  }
  img->cols = cols ;
  img->rows = rows ;
  for ( i = 0 ; i < rows ; i++ ) {
    img->p[i] = &( img->data_p[ i * cols ] ) ;
  }
  return( img ) ;
}

#ifdef __STDC__
void
freeRGBPackedimg( RGB_PACKED_img *img )
#else
void
freeRGBPackedimg( img )
     RGB_PACKED_img *img ;
#endif
{
  free( img->data_p ) ;
  free( img->p ) ;
  free( img ) ;
}

#ifdef __STDC__
RGB_PACKED_img *
readRGBPackedimg( char *name )
#else
RGB_PACKED_img *
readRGBPackedimg( name )
     char *name ;
#endif
{
  int rows ;
  int cols ;
  int maxval ;
  int size ;
  int ret ;
  int numio ;
  int nread ;
  int idx ;
  char *arg ;
  RGB_PACKED_img *img ;
  FILE *fp ;
  int ch ;

/*  if (!( fp = fopen( name, "r" ))) {*/
  if (!( fp = fopen( name, "rw" ))) {
/*  DOS�ǤΥե����륪���ץ�⡼�ɤ��������ʤ�褦���������ޤ�����
                    (�����Ŵ� �渦  �����Ȫ) */
    setError( E_FILE_OPEN ) ;
    printError( name ) ;
    return(0) ;
  }
  if (( ret = readPnmHeader( fp, &cols, &rows, &maxval )) == HAS_ERROR ) {
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  if (( ret & PPM ) != PPM ) {
    setError( E_FILE_FORM ) ;
    printError( name ) ;
    fclose( fp ) ;
    return(0) ;
  }
  if ( maxval < 16 || maxval >= 256 ) {
    setError( E_FILE_MAXVAL ) ;
    printError( name ) ;
    fclose( fp ) ;
  }
  if (!( img = allocRGBPackedimg( cols, rows ))) {
    printError( name ) ;
    fclose( fp ) ;
  }
  size = rows * cols ;
  if ( ret & BINARY ) {
    /*
     * input BINARY encorded PPM img
     */
    for( numio = 0 ; numio < size ; numio += nread ){
      nread = fread( &img->data_p[numio], sizeof( RGB_PACKED_PIXEL), 
		     size - numio, fp ) ;
      if ( nread <= 0 ) {
	setError( E_FILE_READ ) ;
	printError( name ) ;
	freeRGBPackedimg( img ) ;
	fclose( fp ) ;
	return(0) ;
      }
    }

  }
  else {
    /*
     * input ASCII encorded PPM img
     */
    for ( idx = 0 ; idx < size ; idx++ ) {
      if (( arg = nextWord( fp )) == (char*)NULL ) /* read R value */
	setError( E_FILE_EOF ) ;
      else if ( ! isdigit( *arg )) {
        if ( *arg == '#' ) { /* skip over comment line */
          while(( ch = getc( fp )) != '\n' && ch != EOF ) {}
        }
	else 
	  setError( E_FILE_INVALID ) ;
      }
      else
	img->data_p[idx].r = atoi( arg ) ;
      if (( arg = nextWord( fp )) == (char*)NULL ) /* read G value */
	setError( E_FILE_EOF ) ;
      else if ( ! isdigit( *arg )) {
        if ( *arg == '#' ) { /* skip over comment line */
          while(( ch = getc( fp )) != '\n' && ch != EOF ) {}
        }
	else 
	  setError( E_FILE_INVALID ) ;
      }
      else
	img->data_p[idx].g = atoi( arg ) ;
      if (( arg = nextWord( fp )) == (char*)NULL ) /* read B value */
	setError( E_FILE_EOF ) ;
      else if ( ! isdigit( *arg )) {
        if ( *arg == '#' ) { /* skip over comment line */
          while(( ch = getc( fp )) != '\n' && ch != EOF ) {}
        }
	else 
	  setError( E_FILE_INVALID ) ;
      }
      else
	img->data_p[idx].b = atoi( arg ) ;
      if ( checkError()) {
	printError( name ) ;
	freeRGBPackedimg( img ) ;
	fclose( fp ) ;
	return(0) ;
      }
    }
  }
  fclose( fp ) ;
  return( img ) ;
}

#ifdef __STDC__
int
writeRGBPackedimg( RGB_PACKED_img *img, char *name )
#else
int
writeRGBPackedimg( img, name )
     RGB_PACKED_img *img ;
     char *name ;
#endif
{
  FILE *fp ;
  int size ;
  int idx ;
  RGB_PACKED_PIXEL *ptr ;

/*  if (!( fp = fopen( name, "w" ))) {*/
  if (!( fp = fopen( name, "wb" ))) {
/*  DOS�ǤΥե����륪���ץ�⡼�ɤ��������ʤ�褦���������ޤ�����
                    (�����Ŵ� �渦  �����Ȫ) */
    setError( E_FILE_OPEN ) ;
    printError( name ) ;
    return( HAS_ERROR ) ;
  }

  fprintf( fp, "P6\n%d %d\n255\n", img->cols, img->rows ) ;
  ptr = img->data_p ;
  size = img->cols ;
  for ( idx = 0 ; idx < img->rows ; idx++, ptr+=img->cols ) {
    if ( fwrite( ptr, sizeof(RGB_PACKED_PIXEL), size, fp ) != size ) {
      setError( E_FILE_WRITE ) ;
      printError( name ) ;
      fclose( fp ) ;
      return( HAS_ERROR ) ;
    }
  }
  fclose( fp ) ;
  return( NO_ERROR ) ;
}

#ifdef __STDC__
RGB_PLANAR_img *
allocRGBPlanarimg( int cols, int rows )
#else
RGB_PLANAR_img *
allocRGBPlanarimg( cols, rows )
     int cols ;
     int rows ;
#endif
{
  RGB_PLANAR_img *img ;
  int i ;

  clearError() ;
  if (!( img = (RGB_PLANAR_img*)calloc( 1, sizeof(RGB_PLANAR_img)))) {
    setError( E_MALLOC ) ;
    return(0) ;
  }
  if (!( img->r = (BYTE**)malloc( rows * sizeof(BYTE*))))
    setError( E_MALLOC ) ;
  if (!( img->g = (BYTE**)malloc( rows * sizeof(BYTE*))))
    setError( E_MALLOC ) ;
  if (!( img->b = (BYTE**)malloc( rows * sizeof(BYTE*))))
    setError( E_MALLOC ) ;
  if (!( img->data_r = (BYTE*)malloc( cols * rows * sizeof(BYTE))))
    setError( E_MALLOC ) ;
  if (!( img->data_g = (BYTE*)malloc( cols * rows * sizeof(BYTE))))
    setError( E_MALLOC ) ;
  if (!( img->data_b = (BYTE*)malloc( cols * rows * sizeof(BYTE))))
    setError( E_MALLOC ) ;
  if ( checkError()) {
    if ( img->data_g ) free( img->data_g ) ;
    if ( img->data_r ) free( img->data_r ) ;
    if ( img->b ) free( img->b ) ;
    if ( img->g ) free( img->g ) ;
    if ( img->r ) free( img->r ) ;
    free( img ) ;
    return(0) ;
  }
  img->cols = cols ;
  img->rows = rows ;
  for ( i = 0 ; i < rows ; i++ ) {
    img->r[i] = &( img->data_r[ i * cols ] ) ;
    img->g[i] = &( img->data_g[ i * cols ] ) ;
    img->g[i] = &( img->data_b[ i * cols ] ) ;
  }
  return( img ) ;
}

#ifdef __STDC__
void
freeRGBPlanarimg( RGB_PLANAR_img *img )
#else
void
freeRGBPlanarimg( img )
     RGB_PLANAR_img *img ;
#endif
{
  free( img->data_b ) ;
  free( img->data_g ) ;
  free( img->data_r ) ;
  free( img->b ) ;
  free( img->g ) ;
  free( img->r ) ;
  free( img ) ;
}

