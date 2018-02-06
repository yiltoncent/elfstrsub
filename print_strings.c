#include <stdio.h>
#include <stdlib.h>
#include "list.h"  

enum {
    _sch_isprint = 0x0010,
    _sch_isspace= 0x0040,
};

const unsigned short _sch_istable[256] =
{
  Z,  C,  C,  C,   C,  C,  C,  C,   /* NUL SOH STX ETX  EOT ENQ ACK BEL */
  C,  T,  V,  M,   M,  V,  C,  C,   /* BS  HT  LF  VT   FF  CR  SO  SI  */
  C,  C,  C,  C,   C,  C,  C,  C,   /* DLE DC1 DC2 DC3  DC4 NAK SYN ETB */
  C,  C,  C,  C,   C,  C,  C,  C,   /* CAN EM  SUB ESC  FS  GS  RS  US  */
  S,  P,  P,  P,   P,  P,  P,  P,   /* SP  !   "   #    $   %   &   '   */
  P,  P,  P,  P,   P,  P,  P,  P,   /* (   )   *   +    ,   -   .   /   */
  D,  D,  D,  D,   D,  D,  D,  D,   /* 0   1   2   3    4   5   6   7   */
  D,  D,  P,  P,   P,  P,  P,  P,   /* 8   9   :   ;    <   =   >   ?   */
  P, XU, XU, XU,  XU, XU, XU,  U,   /* @   A   B   C    D   E   F   G   */
  U,  U,  U,  U,   U,  U,  U,  U,   /* H   I   J   K    L   M   N   O   */
  U,  U,  U,  U,   U,  U,  U,  U,   /* P   Q   R   S    T   U   V   W   */
  U,  U,  U,  P,   P,  P,  P,  _,   /* X   Y   Z   [    \   ]   ^   _   */
  P, XL, XL, XL,  XL, XL, XL,  L,   /* `   a   b   c    d   e   f   g   */
  L,  L,  L,  L,   L,  L,  L,  L,   /* h   i   j   k    l   m   n   o   */
  L,  L,  L,  L,   L,  L,  L,  L,   /* p   q   r   s    t   u   v   w   */
  L,  L,  L,  P,   P,  P,  P,  C,   /* x   y   z   {    |   }   ~   DEL */

  /* high half of unsigned char is locale-specific, so all tests are
     false in "C" locale */
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,

  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
};

#define _sch_test(c, bit) (_sch_istable[(c) & 0xff] & (unsigned short)(bit))
#define ISPRINT(c)  _sch_test(c, _sch_isprint)
#define ISSPACE(c)  _sch_test(c, _sch_isspace)

#define  STRING_ISGRAPHIC(c) \
    (       (c) >= 0 \
    &&   (c) <= 255 \
    &&   ( (c) == '\t'  || ISPRINT(c) || ISSPACE(c) ) \
    )

typedef struct _strs_info
{
    char *src_start; //store original postion of a GRAPHIC string, including start and end. 
    char *src_end;
    int *file_start;   //all strings stored in one big char array, so we only want to know a string's length
    int len;
}strs_info;

LIST_HEAD(meta_strinfo_list); 

typedef struct _t_meta_strinfo_struct  
{  
    strs_info strinfo;
    struct list_head entry;  
} t_meta_strinfo_struct ; 

