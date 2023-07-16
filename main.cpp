/*
    Starting skeleton was provided by Dr. Mohamed Shalan.
*/

#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

#define DBG 1                        // debugging: What is DBG ?
#define DRAM_SIZE (64 * 1024 * 1024) // Computer memory address space = 64 Mbytes
#define CACHE_SIZE (16 * 1024)       // Cache Size = 16 Kbytes // fixed
int LineSize = 16;                   // in bytes // variable belongs to {16,32,64,128}
int NumberOfWays = 4;                // in units // variable belongs to {1,2,4,8,16}

int numLines = CACHE_SIZE / LineSize;
int numSets = numLines / NumberOfWays;

int numLineOffsetBits = log2(LineSize);
int numIndexBits = log2(numSets);
int numTagBits = 32 - numLineOffsetBits - numIndexBits;

unsigned int LineOffset;
unsigned int Index;
unsigned int Tag;

struct Line
{
    bool valid = 0;
    unsigned int Tag = 0;
    unsigned int Data = 0;
};

vector<vector<Line>> cacheSA;
vector<int> firstIn;

enum cacheResType
{
    MISS = 0,
    HIT = 1
};

void calc(unsigned int addr)
{
    LineOffset = 0;
    unsigned int temp = 0b1;
    for (int i = 0; i < numLineOffsetBits; i++)
    {
        LineOffset |= temp;
        temp <<= 1;
    }
    LineOffset &= addr;

    Index = 0;
    temp = (1 << numLineOffsetBits);
    for (int i = 0; i < numIndexBits; i++)
    {
        Index |= temp;
        temp <<= 1;
    }
    Index &= addr;
    Index >>= numLineOffsetBits;

    Tag = (addr >> (numLineOffsetBits + numIndexBits));
}

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55; /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902; /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}

unsigned int memGen1()
{
    static unsigned int addr = 0;
    return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
    static unsigned int addr = 0;
    return rand_() % (24 * 1024);
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

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
    // This function accepts the memory address for the memory transaction and
    // returns whether it caused a cache miss or a cache hit

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

// Set Associative Cache Simulator
cacheResType cacheSimSA(unsigned int addr)
{
    // This function accepts the memory address for the read and
    // returns whether it caused a cache miss or a cache hit

    // The current implementation assumes there is no cache; so, every transaction is a miss
    return MISS;
}

char *msg[2] = {"Miss", "Hit"};

#define NO_OF_Iterations 10000 // CHange to 1,000,000

int main()
{
    unsigned int hit = 0;
    cacheResType r;
    ofstream output;
    output.open("output.txt");

    cacheSA.resize(numSets);
    for (int i = 0; i < numSets; i++)
        cacheSA[i].resize(numLines);

    firstIn.resize(numSets, 0);

    unsigned int addr;
    cout << "Set Associative Cache Simulator\n";

    cout << numTagBits << " " << numIndexBits << " " << numLineOffsetBits << endl;

    for (int inst = 0; inst < NO_OF_Iterations; inst++)
    {
        addr = memGen1();
        // addr = memGen2();
        // addr = memGen3();
        // addr = memGen4();
        // addr = memGen5();
        // addr = memGen6();

        calc(addr);

        // cout << bitset<32>(addr) << "   " << hex << (Tag) << " " << (Index) << " " << (LineOffset) << endl;
        output << bitset<32>(addr) << "   " << hex << (Tag) << " " << (Index) << " " << (LineOffset) << endl;

        r = cacheSimSA(addr);
        if (r == HIT)
            hit++;
        // cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
        output << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
    }
    cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
}
