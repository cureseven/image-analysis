/* File: pnmio.h, Time-stamp: <98/04/01 02:32:05 sumi>
 *
 * pnmio.h:	file and img data header file for PGM/PPM imgs.
 *
 *		written by Kazuhiko Sumi
 *
 *		NOTE: this program is an contribution to PRMU program
 *		contest and is not related to the products. The author
 *		is not responsible to any problem using this code.
 */

typedef unsigned long  DWORD ;
typedef unsigned short WORD ;
typedef unsigned char  BYTE ;

typedef struct gray_img {
  int     cols ;
  int     rows ;
  BYTE  **p ;
  BYTE   *data_p ;
} GRAY_img ;

typedef struct gray_long_img {
  int     cols ;
  int     rows ;
  long  **p ;
  long   *data_p ;
} GRAY_LONG_img ;

typedef struct gray_double_img {
  int      cols ;
  int      rows ;
  double **p ;
  double  *data_p ;
} GRAY_DOUBLE_img ;

typedef struct rgb_packed_pixel {
  BYTE r ;
  BYTE g ;
  BYTE b ;
} RGB_PACKED_PIXEL ;

typedef struct rgb_packed_img {
  int                cols ;
  int                rows ;
  RGB_PACKED_PIXEL **p ;
  RGB_PACKED_PIXEL  *data_p ;
} RGB_PACKED_img ;

typedef struct rgb_planar_img {
  int     cols ;
  int     rows ;
  BYTE  **r ;
  BYTE  **g ;
  BYTE  **b ;
  BYTE   *data_r ;
  BYTE   *data_g ;
  BYTE   *data_b ;
} RGB_PLANAR_img ;

/*
 *  define type of PBM/PGM/PPM file format
 */
#define PGM	2
#define PPM	4
#define BINARY	8
#define ASCII	0

/*
 *  define library error code
 */
#define NO_ERROR	0
#define HAS_ERROR	-1

#define MAX_ERRORS	11
#define E_NULL		0
#define E_UNKNOWN	1
#define E_MALLOC	2
#define E_FILE_OPEN	3
#define E_FILE_READ	4
#define E_FILE_WRITE	5
#define E_FILE_NOTPNM	6
#define E_FILE_FORM	7
#define E_FILE_EOF	8
#define E_FILE_INVALID	9
#define E_FILE_MAXVAL	10
#define E_FILE_PBM	11

/*
 *  library function prototypes (if not in ANSI mode, don't protize arguments)
 */
#ifndef __P
#if defined(__STDC__) || defined(__cplusplus)
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif
#endif /* defined __P */

char			*nextWord __P(( FILE* ));
char 			*readPbmComment __P(( FILE* ));
int			readPnmHeader __P(( FILE*, int*, int*, int* ));

GRAY_img 		*allocGrayimg __P(( int, int ));
void			freeGrayimg __P(( GRAY_img* ));
GRAY_img 		*readGrayimg __P(( char* ));
int			writeGrayimg __P(( GRAY_img*, char* ));

GRAY_LONG_img		*allocGrayLongimg __P(( int, int ));
void			freeGrayLongimg __P(( GRAY_LONG_img* ));

GRAY_DOUBLE_img	*allocGrayDoubleimg __P(( int, int ));
void			freeGrayDoubleimg __P(( GRAY_DOUBLE_img* ));

RGB_PACKED_img 	*allocRGBPackedimg __P(( int, int ));
void			freeRGBPackedimg __P(( RGB_PACKED_img* ));
RGB_PACKED_img 	*readRGBPackedimg __P(( char* ));
int			writeRGBPackedimg __P(( RGB_PACKED_img*, char* ));

RGB_PLANAR_img 	*allocRGBPlanarimg __P(( int, int ));
void			freeRGBPlanarimg __P(( RGB_PLANAR_img* ));

int			setError __P(( int ));
void			clearError __P(( void ));
int			checkError __P(( void ));
void			printError __P(( char* ));

void	setRGBMarkColor __P(( int, int, int ));
void	markDotRGBPackedimg __P(( RGB_PACKED_img*, int, int ));
void	drawLineRGBPackedimg __P(( RGB_PACKED_img*, int, int, int, int ));
void	drawPolygonRGBPackedimg __P(( RGB_PACKED_img*, int*, int*, int ));
void	drawRotatedRectangleRGBPackedimg __P(( RGB_PACKED_img*,
						int, int, int, int, int ));

