#ifndef __MY_CTYPE_H_
#define __MY_CTYPE_H_

#define	X_TRAIT_U	01
#define	X_TRAIT_L	02
#define	X_TRAIT_N	04
#define	X_TRAIT_S	010
#define X_TRAIT_P	020
#define X_TRAIT_C	040
#define X_TRAIT_X	0100
#define	X_TRAIT_B	0200

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
    extern int x_ctype_lookup (int c);
    extern int x_tolower(int c);
#define x_isalpha( c) (x_ctype_lookup((int)c)&(X_TRAIT_U|X_TRAIT_L))
#define x_isupper( c) ((x_ctype_lookup((int)c)&(X_TRAIT_U|X_TRAIT_L))==X_TRAIT_U)
#define x_islower( c) ((x_ctype_lookup((int)c)&(X_TRAIT_U|X_TRAIT_L))==X_TRAIT_L)
#define x_isdigit( c) (x_ctype_lookup((int)c)&X_TRAIT_N)
#define x_isxdigit( c) (x_ctype_lookup((int)c)&(X_TRAIT_X|X_TRAIT_N))
#define x_isspace( c) (x_ctype_lookup((int)c)&X_TRAIT_S)
#define x_ispunct( c) (x_ctype_lookup((int)c)&X_TRAIT_P)
#define x_isalnum( c) (x_ctype_lookup((int)c)&(X_TRAIT_U|X_TRAIT_L|X_TRAIT_N))
#define x_isprint( c) (x_ctype_lookup((int)c)&(X_TRAIT_P|X_TRAIT_U|X_TRAIT_L|X_TRAIT_N|X_TRAIT_B))
#define x_isgraph( c) (x_ctype_lookup((int)c)&(X_TRAIT_P|X_TRAIT_U|X_TRAIT_L|X_TRAIT_N))
#define x_iscntrl( c) (x_ctype_lookup((int)c)&X_TRAIT_C)
#define x_isascii( c) ((unsigned)(c)<=0177)
#define x_toascii( c) ((c)&0177)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* X_CTYPE_H_ */
