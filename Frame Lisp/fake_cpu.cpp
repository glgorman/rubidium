// fake_cpu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef PROPELLER
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <propeller.h>
#endif

#include "fake_cpu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;
#ifdef PROPELLERt
typedef DWORD size_t;
ypedef enum {false, true} bool;
#endif

//#define DEBUG_ADD 1
#define DEBUG_ALU 1
#define MEMSIZE 1024
#define BYTESIZE	8
#define WORDSIZE (sizeof(short)*BYTESIZE)

int ticks=0;
bool m_carry;
bool m_zero;
DWORD lut1[256];
DWORD mem[MEMSIZE];
DWORD m_pc, m_ptra, m_ptrb;

void makesq()
{
	int i,j,k;
	i=0;j=1;k=0; 
	while(i<256)
	{
	  lut1[++i]=j++;j+=(k+=2);
	} 
}
	
inline DWORD nor(DWORD r1, DWORD r2)
{
	DWORD result;
	result = ~(r1|r2);
	return result;
}

inline DWORD not(DWORD r1)
{
	DWORD result;
	result = nor(r1,r1);
	return result;
}

inline DWORD or(DWORD r1, DWORD r2)
{
	DWORD result;
	result = not(nor(r1,r2));
	return result;
}

inline DWORD and(DWORD r1, DWORD r2)
{
	DWORD result;
	result = nor(not(r1),not(r2));
	return result;
}

inline DWORD nand(DWORD r1, DWORD r2)
{
	DWORD result;
	result = not(nor(not(r1),not(r2)));
	return result;
}

inline DWORD xor(DWORD r1, DWORD r2)
{
	DWORD result;
	result = and(or(r1,r2),not(and(r1,r2)));
	return result;
}

inline DWORD swap_words (DWORD r1)
{
	DWORD result;
	result = ((0x0000ffff&r1)<<16)|((0xffff0000&r1)>>16);
	return result;
}

inline DWORD swap_bytes (DWORD r1)
{
	DWORD result;
	result = ((0x00ff00ff&r1)<<8)|((0xff00ff00&r1)>>8);
	return result;
}

inline DWORD swap_nibbles (DWORD r1)
{
	DWORD result;
	result = ((0x0f0f0f0f&r1)<<4)|((0xf0f0f0f0&r1)>>4);
	return result;
}

inline DWORD swap_pairs (DWORD r1)
{
	DWORD result;
	result =  ((0x33333333&r1)<<2)|((0xcccccccc&r1)>>2);
	return result;
}

inline DWORD swap_bits (DWORD r1)
{
	DWORD result;
	result = ((0x55555555&r1)<<1)|((0xaaaaaaaa&r1)>>1);
	return result;
}

inline DWORD bitrev32 (DWORD r1)
{
	DWORD result;
	result = swap_bits(swap_pairs(swap_nibbles(swap_bytes(swap_words(r1)))));
	return result;
}

inline void swap (unsigned int &r1, unsigned int &r2)
{
	unsigned int temp;
	temp = r1;
	r1 = r2;
	r2 = temp;
}

inline DWORD add(DWORD r1, DWORD r2)
{
//	int ticks = 0;
	DWORD result;
	DWORD s1, s2, c1, c2, c3;
	DWORD old_c3, new_c3;
	DWORD ind;
#ifdef DEBUG_ADD
printf ("t=%d, adding %d+%d .....\n",ticks, r1,r2);
#endif
	ticks++;
	c1 = and(r1,r2);
	s1 = nor(nor(r1,r2),c1);
	c3 = 0; // or 1 if there is input carry
//	c3 = 0xdeadbeef;
//	c3 = 0xcccccccc;
//	c3 = 0xfadedcab;
//	c3 = or(s1,c1);
	c3 = 0;
	ind = s1;
	char carry_status[64];
	for(int i=0;i<32;i++)
	{
		// Ah! little endian!
		if (ind&(1<<(31-i)))
			carry_status[i]='X';
		else
			carry_status[i]=(c1&(1<<(31-i))?'1':'0');
	}
	carry_status[32]=0;
	ticks++;
#ifdef DEBUG_ADD
	printf ("Tick=%2d, initial sum %08x, initial carry %08x (%s)\n",ticks,s1,c1,carry_status);
#endif
	do {
		old_c3 = c3;
		c2 = and(s1,c3);
#if 1
		s2 = nor(nor(s1,c3),c2);
#endif
		c3 = or(c1,c2)<<1;
		new_c3 = c3; // redundant for clarity 
		ticks++;
#ifdef DEBUG_ADD
printf ("Tick=%2d: S1=%08x, C1=%08x, S2=%08x, C2=%08x, C3=%08x\n",
			ticks,s1,c1,s2,c2,c3);
#endif
	}
// debugging note - BE VERY AFRAID  or at leaat have
// a lot of spare time on your hands if you venture
// into these parts, for as many ancient maps suggest
// ... there be dragons!
	while (old_c3!=new_c3);
	s2 = nor(nor(s1,c3),c2);
	ticks++;
#ifdef DEBUG_ADD
	printf ("Adding %d+%d=%d took %d ticks.\n",
			r1,r2,s2,ticks);
#endif
	result = s2;
	ASSERT (result==r1+r2);
	return result;
}

#ifdef NODEBUG
inline DWORD add(DWORD r1, DWORD r2)
{
	DWORD s1, s2, c1, c2, c3;
	DWORD old_c3;
	c1 = and(r1,r2);
	c3 = 0; // or 1 if there is input carry
	s1 = nor(nor(r1,r2),c1);
	do {
		old_c3 = c3;
		c2 = and(s1,c3);
		c3 = or(c1,c2)<<1; 
	}
	while (old_c3!=c3);
	s2 = nor(nor(s1,c3),c2);
	return s2;
}
#endif

inline DWORD sub(DWORD r1, DWORD r2)
 {
	int result;
	result = add(r1,add(not(r2),1));
	return result;
}

inline DWORD neg(DWORD r1)
{
	DWORD result;
	result = add(not(r1),1);
	return result;
}

inline DWORD inc(DWORD r1)
{
	DWORD result;
	result = add(r1,1);
	return result;
}

inline DWORD dec(DWORD r1)
{
	DWORD result;
	result = sub(r1,1);
	return result;
}

void cmp (DWORD r1, DWORD r2)
{
	DWORD test;
	test = sub(r1,r2);
	if (test==0)
		m_zero=true;
	else
		m_zero=false;
	if (test&0x80000000)
		m_carry=true;
	else
		m_carry=false;
}

inline unsigned short mult8 (unsigned char c1, unsigned char c2)
{
	unsigned short s0, s1, s2, s3, s4;
	if ((c1==0)|(c2==0))
		return 0;
	if (c1==1)
		return c2;
	if (c2==1)
		return c1;
	s0 = (unsigned short) add(c1,c2);
	s1 = (unsigned short) sub(c1,c2);
	bool m_odd = (s0&0x1?true:false);
	bool m_neg = (s1&0x8000?true:false); 
	if (m_odd) {
		s0 = (unsigned short) add(s0,1);
		s1 = (unsigned short) add(s1,1);
	}
	s2 = (unsigned short)(m_neg?add(not(s1),1):s1);
	s0>>=1;
	s2>>=1;
	s3 = (unsigned short) sub(lut1[s0],lut1[s2]);
	if (m_odd)
		s4 = (unsigned short) sub(s3,c2);
	else
		s4 = s3;
		
	ASSERT(s4==c1*c2);
	return s4;
}

inline DWORD square(unsigned short r1)
{
	DWORD t0,result;
	DWORD t1, t2, t3;
	unsigned char highbyte, lowbyte;
	lowbyte = r1&0xff;
	highbyte = (r1&0xff00)>>BYTESIZE;
	t0 = (lut1[highbyte]<<WORDSIZE)|(lut1[lowbyte]);
	t1 = mult8(highbyte,lowbyte);
	t2 = t1<<(BYTESIZE+1);	
	t3 = add(t0,t2);
	result = t3;
	ASSERT(r1*r1==result);
	return result;
}

inline DWORD mult16(unsigned short r1, unsigned short r2)
{
	DWORD result;
	DWORD t1, t2, t3, t4;
	t1 = add((DWORD)r1,(DWORD)r2);
	t2 = sub((DWORD)r1,(DWORD)r2);
	t3 = square(t1);
	t4 = square(t2);
	result = sub(t3,t4)>>2;
	return result;
}

void test_mult8()
{
	int i, j, k;
	DWORD t0 = GetTickCount();
	for (i=0;i<256;i++)
	for (j=0;j<256;j++)
	{
		k = mult8((unsigned char)i,(unsigned char)j);
		ASSERT(k==i*j);
//	printf ("%d*%d=%d\n",i,j,k);
	}
	DWORD t1 = GetTickCount();
	printf ("Test  mult8 took %d msec\n",t1-t0);
}

void test_square16()
{
	int i, k;
	DWORD t0 = GetTickCount();
	for (i=0;i<65536;i++)
	{
		k = square(i);
		ASSERT(k==i*i);
//	printf ("%d*%d=%d\n",i,j,k);
	}
	DWORD t1 = GetTickCount();
	printf ("Test square16 took %d msec\n",t1-t0);
}

void test_mult16(unsigned short r1, unsigned short r2)
{
	unsigned short x, y;
	DWORD z;
	x = r1;
	y = r2;
	z = mult16(x,y);
	printf ("mult16(%d,%d) returned %d\n",x,y,z);
}

int _pmain ()
{
	DWORD w, x, y, z;
	DWORD y1, y2;
	makesq();
	int i;

	for (i=0;i<256;i++)
	  printf ("%d*%d=%d\n",i,i,lut1[i]);
//	w = add(12345,9876);
//	w = add(12121212,33333333);
//	w = add(1048575,1048575);
//	w = add(0xdeadbeef,0xfadedcab);
//	w = add(0x7fffffff,0x1);

	test_mult8();
	test_square16();
	w = mult8(43,57);
	x = square(1001);
	printf ("square(1001) returned %d\n",x);
	y = square(125);
	printf ("square(125) returned %d\n",y);

//	test_mult16(1776,1492);
	y1 = 1776;
	y2 = 1492;
	z = mult16(y1,y2);
	printf ("mult16(%d,%d) returned %d\n",y1,y2,z);

	y2 = 2525;
	y1 = 7510;
	z = mult16(y1,y2);
	printf ("mult16(%d,%d) returned %d\n",y1,y2,z);

	printf ("Total arithmetic operations - %d\n",ticks);
	return 0;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		_pmain();
	}

	return nRetCode;
}
