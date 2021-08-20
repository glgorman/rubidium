// padc_console.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "messages.h"
#include "globalunit.h"
#include "compatibility.h"
#include "debugdisplayunit.h"
#include "propserial.h"
#include "ComThread.h"

#include "stdio.h"
//#include "propeller.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FFT_SIZE 128

// enough for 4096 test bits
int testbits [FFT_SIZE];

#define EVEN_BITS		(0x55555555)
#define ODD_BITS   		(0xaaaaaaaa)
#define EVEN_PAIRS		(0x33333333)
#define ODD_PAIRS		(0xcccccccc)
#define EVEN_NIBBLES	(0x0f0f0f0f)
#define ODD_NIBBLES		(0xf0f0f0f0)
#define EVEN_BYTES		(0x00ff00ff)
#define ODD_BYTES		(0xff00ff00)

#define UINT unsigned int

#define MATH_TYPE   float
#define bitstream   int

#if 0
#define DWORD       unsigned int
#define HIWORD(arg)   (((arg)&(0xffff0000))>>16)
#define LOWORD(arg)   ((arg)&(0x0000ffff))
#endif

DWORD GF32 (DWORD p);
short GF16 (short p);
 
/////////////////////////////////////////////////////////////////
class propeller_adc
{
protected:
	float my_data [128*2];

public:
	unsigned int iterate (bool sample);
	void init_data1 (int f, float val);
	void init_data2 ();
	void report (int sz, float *data);

public:
	bool carry;
	unsigned int REG[8];
	
	void reset ();
//  static unsigned char count_bits (DWORD);
	unsigned char count_bits (DWORD input);
	DWORD decimate1A (DWORD input);
	DWORD decimate1B (DWORD input);
	DWORD decimate2 (DWORD input);
	DWORD decimate3  (DWORD input);
	void print_bytes (int regid);
	void print_bytes2 (int regid);
	void hex_dump ();
	void print_nibbles ();
	int main();
};

class test
{
public:
  static void print_testbits ();
  static void print_test_nibbles ();
  static void create_testbits1 (float *);
  static void create_testbits2 ();
  static void create_testbits3 ();
  static void run_tests (propeller_adc *, char *log);
};

class fft
{
protected:
  static unsigned bitrev (unsigned int x, int log2n);
  static void bitReverseArray (float *data, int m);
  
public:
  static void cdft(int nn, int dir, float *data);
  static void fft4 (bitstream);  
};

void fft::fft4 (bitstream z)
{
// perform a 4 point fft 
// treating the input as a 16 one bit complex values
// add the high word to the low word without performing any
// carroes; resulting in 32 two bit values
   int s0, s1;
   s0 = HIWORD(z)^LOWORD(z);
   s1 = HIWORD(z)&LOWORD(z);
  
  
}

unsigned fft::bitrev (unsigned int x, int log2n)
{
  int n = 0;
  int mask = 0x1;
  for (int i=0; i < log2n; i++) 
  {
    n <<= 1;
    n |= (x & 1);
    x >>= 1;
  }
  return n;
}

void fft::bitReverseArray (float *data, int m)
{
	float tempr;
	int i,j,k,n = m;
   // Do the bit reversal 
	j = 1;
	k = 0;
	while (j<n){
		k++;
		j<<=1;
	}
	for (i=1;i<n-1;i++)
	{
		j = bitrev (i,k);
		if (i < j) 
		{
			tempr = data[i];
			data[i] = data[j];
			data[j] = tempr;
		}
	}
}

void fft::cdft(int nn, int dir, float *data)
{
	int pass;
	int i,i1,j,l1,l2;
	float c1,c2,u1,u2,z;
	float tempr,tempi;
	bitReverseArray (data,nn);

 // Compute the FFT 
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (pass=1;pass<nn;pass<<=1) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            tempr = u1 * data[2*i1] - u2 * data[2*i1+1];
            tempi = u1 * data[2*i1+1] + u2 * data[2*i1];
            data[2*i1] = data[2*i] - tempr; 
            data[2*i1+1] = data[2*i+1] - tempi;
            data[2*i] += tempr;
            data[2*i+1] += tempi;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

 // Scaling for forward transform
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         data[2*i] /= nn;
         data[2*i+1] /= nn;
      }
   }	
}

///////////////////////////////////////////////////////

class random
{
public:
	static short GF16 (short p);
	static DWORD GF32 (DWORD p);
	static MATH_TYPE frnd ();
	static MATH_TYPE range (int max);
};

short random::GF16 (short p)
{
	DWORD q;
	q = 1^p^(p<<3)^(p<<12)^(p<<16);
	return HIWORD(q)^LOWORD(q);
}

DWORD random::GF32 (DWORD p)
{
	int res, q;
	q = (int)p;
 	res = (q>>1)^((-(q&1)&0xd0000001));
	return (DWORD)res;
}

MATH_TYPE random::frnd ()
{
	MATH_TYPE result;
	MATH_TYPE fmax_int = (MATH_TYPE(0xffffffff));
	static UINT value1 = 1;
	static UINT value2 = 1;
	value1 = GF16 (value1);
	value2 = GF32 (value2);
	result = MATH_TYPE(value1^value2)/fmax_int;
	return result;
}

MATH_TYPE random::range (int max)
{
	MATH_TYPE result;
	result = frnd()*max;
	return result;
}

// Fake Muliprocesscing kernel for now
// todo - start and stop stuff on 
// indivudual cogs the way that Windoze
// does AfxBeginThread without all of the
// overhead of pthreads, etc, and named pipes

class process
{
public:
   static process pstack[8];
   static int max_process;
   int size;
   char process_id;
   char *path;
   
   static void init ();
   static void show_process_list ();
   
protected:
   void create_process ();
   void kill_process (); 
};

process process::pstack[8];
int process::max_process;

  
void process::create_process ()
{
  process_id=max_process;
  path =  "/dev/null";
  max_process++;
  size = 512;
}

void process::kill_process ()
{
  max_process--;
}


void process::show_process_list ()
{
  int i,j;
  char *path;
  int size;
  process *p;
  CComThread::OutputDebugString   ("\r\nPID - PATH     - SIZE - CPU"); 
  for (i=0;i<max_process;i++)
  {
    p = &pstack[i];
    j = p->process_id;
    path = p->path;
    size = p->size;
#if 0
    CComThread::OutputDebugString ("%d:  %s  %d    0\n",j,path,size);
#endif
  }
  CComThread::OutputDebugString ("\r\n");
}

void process::init ()
{
  max_process = 0;
  for (int i=0;i<8;i++)
     pstack[i].create_process();
}

////////////////////////////////////////////////////////////


void propeller_adc::init_data1 (int f, float val)
{
    int i;
    for (i=0;i<FFT_SIZE*2;i++)
      my_data[i]=0;
    my_data [f] = val;
}

// simple stair steps;

void propeller_adc::init_data2 ()
{
    int i;
    int val;
    int steps = 16;
    for (i=0;i<FFT_SIZE;i++)
    {
      val = (272.0/steps)*((i*steps)/FFT_SIZE)-128.0;
      my_data[2*i]= val;
      my_data[2*i+1]=0;
    }     
}
 
void propeller_adc::report (int sz, float *data)
{
	char outstr[64];
	for (int i=0;i<FFT_SIZE;i++)
	{
		sprintf_s
		(
			outstr,64,
			"\r\n%d: [%1.4f, %1.4f], ",
			i,my_data[2*i],my_data[2*i+1]
		);
	 CComThread::OutputDebugString (outstr);
 }
 CComThread::OutputDebugString ("\r\n"); 
}


// sine wave with random dither

void test::create_testbits1 (float *data)
{
   float d;
   unsigned int acc,i,j,k;
   CComThread::OutputDebugString ("\r\nCreating test bits");
   for (i=0;i<FFT_SIZE;i++)
     {
       acc=0;
       d = 128+0.5*data[2*i];
#if 0
	   CComThread::OutputDebugString ("\r\nDATA %0.2f\r\n....",d);
#endif
       for (j=0;j<32;j++)
       {
         k = random::range (256);
#if 0
		CComThread::OutputDebugString ("%d ",k);
#endif
         if (d>k)
           acc++;
         acc=acc<<1;
       }
	   CComThread::OutputDebugString ("\r\nTEST_BITS %08x \n",acc);
       testbits[i]=acc;       
     }
}

// bit field test one bit

void test::create_testbits2 ()
{
   unsigned int acc, i;
   for (i=0;i<FFT_SIZE;i++)
     {
       acc= (0x01<<(i%32));
       if (i>31)
         acc|= (0x01<<((i+1)%32));
         if (i>63)
         acc|= (0x01<<((i+2)%32));
         if (i>95)  
         acc|= (0x01<<((i+3)%32));
       testbits[i]=acc;       
     }
}

void test::create_testbits3 ()
{
   unsigned int i,j;
   for (i=0;i<FFT_SIZE;i++)
     {
       j = i/8;
       j|= j<<4;
       j|= j<<8;
       j|= j<<16;       
       testbits[i]=j;       
     }
}

void test::print_testbits ()
{
	char bits[64];
	unsigned int i,j;
	for (i=0;i<FFT_SIZE;i++)
	{
		for (j=0;j<32;j++)
			bits[j]='0'+(testbits[i]>>j)&0x01;
		bits[32]=0;
		CComThread::OutputDebugString (bits);
		CComThread::OutputDebugString ("\r\n");
	} 
}

void test::print_test_nibbles ()
{
   int i,j;
   unsigned int k;
   for (i=0;i<FFT_SIZE;i++)
     {
       CComThread::OutputDebugString ("\r\n"); 
       for (j=28;j>=0;j-=4)
       {
          k = (testbits[i]>>j)&0x0f;
          CComThread::OutputDebugString ("%2x ",k);
        } 
     }
     CComThread::OutputDebugString ("\r\n",k); 
}

void propeller_adc::print_nibbles ()
{
	char bits[64];
	int j;
	unsigned int k;
	CComThread::OutputDebugString (" R%d:",0);
	for (j=31;j>=0;j--)
	{
		k = (REG[0]>>j)&0x01;
		bits[j] = '0'+k;
	}
	bits[32]=0;
	CComThread::OutputDebugString (bits);
	CComThread::OutputDebugString (" R1/2: ");
	for (j=28;j>=0;j-=4)
	{
		k = (REG[1]>>j)&0x0f;
		CComThread::OutputDebugString ("%2x",k);
		k = (REG[2]>>j)&0x0f;
		CComThread::OutputDebugString ("%2x",k);
	}
	CComThread::OutputDebugString ("\r\n"); 
}

void propeller_adc::print_bytes (int regid)
{
	int j;
	char bits[64];
	unsigned int k;
	CComThread::OutputDebugString (" R%d:",0);
	for (j=31;j>=0;j--)
	{
		k = (REG[0]>>j)&0x01;
		bits[j] = '0'+k;
	}
	bits[32]=0;
	CComThread::OutputDebugString (bits);
#if 0
    CComThread::OutputDebugString (" R%d/%d: ",regid,regid+1);
#endif
   for (j=24;j>=0;j-=8)
   {
      k = (REG[regid]>>j)&0xff;
      CComThread::OutputDebugString ("%2x ",k);
      k = (REG[regid+1]>>j)&0xff;
      CComThread::OutputDebugString ("%2x ",k);
   }
   CComThread::OutputDebugString ("\r\n"); 
}

void propeller_adc::print_bytes2 (int regid)
{
	char bits[64];
	int j;
	unsigned int k;
	CComThread::OutputDebugString (" R%d:",0);
	for (j=31;j>=0;j--)
	{
		k = (REG[0]>>j)&0x01;
		bits[j]='0'+k;
	}	
	bits[32]=0;
	CComThread::OutputDebugString (bits);
	for (j=31;j>=0;j--)
	{
		CComThread::OutputDebugString ("%1d",k);
	}
	CComThread::OutputDebugString (" R1/2: ");
	for (j=24;j>=0;j-=8)
	{
		k = (REG[regid]>>j)&0xff;
		CComThread::OutputDebugString ("%2x ",k);
	}
	CComThread::OutputDebugString ("\r\n"); 
}

//////////////////////////////////////////////////////////

unsigned char propeller_adc::count_bits (DWORD input)
{	
	DWORD sum;
	DWORD p1, p2;
    p1 = (input&ODD_BITS)>>1;
    p2 = (input&EVEN_BITS);
    sum = p2+p1;
    p1 = (sum&ODD_PAIRS)>>2;
    p2 = (sum&EVEN_PAIRS);
    sum = p2+p1;  
    p1 = (sum&ODD_NIBBLES)>>4;
    p2 = (sum&EVEN_NIBBLES);
	sum = p2+p1; 
    p1 = (sum&ODD_BYTES)>>8;
    p2 = (sum&EVEN_BYTES);
    sum = p2+p1;
    p1 = HIWORD(sum);
    p2 = LOWORD(sum);
    sum = p2+p1;
	return (unsigned char) sum;
}

DWORD propeller_adc::decimate1B (DWORD input)
{
  DWORD q2, q3;
  REG[0] = input;
  q2 = (input&EVEN_BITS)<<1;
  q3 = ((input&ODD_BITS)+((input&ODD_BITS)>>2))>>1;
  REG[1] = ((q2&ODD_PAIRS) + (q3&ODD_PAIRS))>>2;  
  REG[2] = (q2&EVEN_PAIRS) + (q3&EVEN_PAIRS);
  return 0;
}

DWORD propeller_adc::decimate2 (DWORD input)
{
// phase two - apply the same transformation
// to the 16 three bit values, yielding
// eight five bit values, having a range [0..32]
DWORD  q0, q1, q2, q3;
DWORD  s0, s1;

q0 = REG[1]&EVEN_NIBBLES;
q1 = (REG[1]&ODD_NIBBLES)>>4;
q2 = REG[2]&EVEN_NIBBLES;
q3 = (REG[2]&ODD_NIBBLES)>>4;

// HMMM... FIXME?

s0 = q1+q3+(q0<<1);
s1 = q1+q3+(q2<<1);
  REG[3]=s1;
  REG[4]=s0;
	return 0;
}

DWORD propeller_adc::decimate3 (DWORD input)
{
  DWORD acc;
// finally repack into a 32 bit register
// for in input rate of 250Mbps - this results
// in an initial output rate of 31.25 Mhz
DWORD  q0, q1, q2, q3;
DWORD  s0, s1;

q0 = REG[3]&EVEN_BYTES;
q1 = (REG[3]&ODD_BYTES)>>8; 
q2 = REG[4]&EVEN_BYTES;
q3 = (REG[4]&ODD_BYTES)>>8;

// HMMM... FIXME?

s0 = q0+q1+(q2<<1);
s1 = q0+q1+(q3<<1);
  REG[5]=s1;
  REG[6]=s0;
  
  acc = (s0<<16)+s1;
  REG[7]=acc;
	return acc;
}

 
void test::run_tests (propeller_adc *p2, char *logstr)
{
	int i;
	int sid = 0;
	unsigned int x;
	CComThread::OutputDebugString ("\r\nBITSTREAM TEST - NIBBLES:");
	CComThread::OutputDebugString (logstr);
	for (i=0;i<FFT_SIZE;i++)
	{
		x = testbits [i];
		p2->decimate1B (x);
		p2->decimate2 (0);
		p2->decimate3 (3);  
		p2->print_nibbles ();
	}
	CComThread::OutputDebugString ("\r\nBITSTREAM TEST - NIBBLES:");
	CComThread::OutputDebugString (logstr);
	for (i=0;i<FFT_SIZE;i++)
	{
		x = testbits [i];
		p2->decimate1B (x);
		p2->decimate2 (0);
		p2->decimate3 (3);   
		p2->print_bytes(3);
	}
}
 
int propeller_adc::main()                                
{
  unsigned char count32 = count_bits (0xFFFFFFFF);
  
   CComThread::OutputDebugString ("\r\nHello %d bit world!!\r\n", count32); 
// will use this later to launch threads
// one individual cogs RTFM???
   process::init ();
   process::show_process_list ();

// pack 3 cycles of a cosine wave into
// a 256 bin complex array by computing 
// the FFT of f[5]=256.0

   init_data1 (3,256);
   fft::cdft (FFT_SIZE,-1,my_data);
   test::create_testbits1 (my_data);
   test::run_tests (this,"\r\nsine wave 5 cycles/4096 samples, random dither\r\n");
   report (FFT_SIZE,my_data);

   init_data2();
//   test::report (FFT_SIZE,my_data);
   test::create_testbits1 (my_data);
   test::run_tests (this,"\r\n16 stair step function, random dither\r\n");  
//   print_testbits ();
   
   test::create_testbits2 ();
   test::run_tests (this,"\r\nsliding bit patterns, no dither"); 
   test::create_testbits3 ();
   test::run_tests (this,"\r\nrepeated nibble patterns, no dither");

   return 0;
}



