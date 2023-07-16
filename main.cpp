#include <iostream>
#include  <iomanip>
#include <cmath>


using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)

enum cacheResType { MISS = 0, HIT = 1 };

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGen1()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return  rand_() % (24 * 1024);
}

unsigned int memGen3()
{
	return rand_() % (DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr = 0;
	return (addr++) % (4 * 1024);
}

unsigned int memGen5()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
	static unsigned int addr = 0;
	return (addr += 32) % (64 * 4 * 1024);
}
int clsArr[4] = { 16,32,64,128 };
int nway[5] = { 1,2,4,8,16 };
int Mcls = 0;
int Mnway=0;
void getWYndCLS(int &COUNTclsizeWAY4,int &COUNTWAYclsize32) {
	unsigned int clsize=0;
	unsigned int numway=0;
	if (COUNTclsizeWAY4 > 3) {
		if (COUNTWAYclsize32 < 5) {
			numway = nway[COUNTWAYclsize32];
			clsize = 32;
			COUNTWAYclsize32++;
		}
		else {
			cout << "Program Complete!" << endl;
			exit(0);
		}
	}
	else {
		clsize = clsArr[COUNTclsizeWAY4];
		COUNTclsizeWAY4++;
		numway = 4;

	}
	Mcls = clsize;
	Mnway = numway;
}

void getsectionaddr(unsigned int addr,unsigned int &index,unsigned int &tag) {
	unsigned int offset=0;
	
	int temp = 0;
	int indexbits, tagbits;
	offset = log2(Mcls);
	indexbits = log2(CACHE_SIZE / Mcls);
	tagbits = 26 - indexbits - offset;
	temp = addr >> offset;
	unsigned  mask;
	mask = (1 << indexbits) - 1;
	index = temp & mask;
	temp = temp >> indexbits;
	mask = (1 << tagbits) - 1;
	tag = temp & mask;




}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)

{
	unsigned int index = 0;
	unsigned int tag = 0;

	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit

	getsectionaddr(tag, index, addr);

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}

// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}
string miss = "miss";
string hit = "hit";

#define		NO_OF_Iterations	1000000		// CHange to 1,000,000
int main()
{
	unsigned int hit = 0;
	unsigned int index = 0; unsigned int tag = 0;
	cacheResType r;
	int COUNTclsizeWAY4 = 0;
	int COUNTWAYclsize32 = 0;
	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";
	for (int i = 0; i < 9; i++) {
		getWYndCLS(COUNTclsizeWAY4, COUNTWAYclsize32);
		for (int inst = 0; inst < NO_OF_Iterations; inst++)
		{
			
			addr = memGen2();
			getsectionaddr(addr,index,tag);
			r = cacheSimDM(addr);
			if (r == HIT) hit++;
			cout << "0x" << setfill('0') << setw(8) << hex << addr << " (";
			if (r)  cout << hit << ")\n";
			else cout << miss << ")\n";
		}
		cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
	}
}