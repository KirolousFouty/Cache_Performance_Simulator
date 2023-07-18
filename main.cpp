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

int numLineOffsetBits;
int numIndexBits;
int numTagBits;

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

// given the line size and number of ways, this function calculates the Tag, Index, and Offset.
void calc(unsigned int addr)
{
    numLines = CACHE_SIZE / LineSize;
    numSets = numLines / NumberOfWays;

    numLineOffsetBits = log2(LineSize);
    numIndexBits = log2(numSets);
    numTagBits = log2(DRAM_SIZE) - numLineOffsetBits - numIndexBits;

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

    Tag = (addr >> (numLineOffsetBits + numIndexBits)) & 0b111111111111;
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

// Set Associative Cache Simulator
cacheResType cacheSimSA(unsigned int addr)
{
    // compare TAGs of the lines in the set of the given Index
    for (int i = 0; i < NumberOfWays; i++)
        if (cacheSA[Index][i].valid && cacheSA[Index][i].Tag == Tag)
            return HIT;

    // If it is a miss, copy it in the cache, and properly assign the valid bit, TAG, and manage the FIFO index.
    cacheSA[Index][firstIn[Index]].valid = 1;
    cacheSA[Index][firstIn[Index]].Tag = Tag;
    firstIn[Index]++;
    firstIn[Index] %= NumberOfWays;

    return MISS;
}

// Direct Mapped Cache Simulator; however, it is redundant because we are going to use the cacheSimSA with special Line Size and Number of Ways conditions.
// cacheResType cacheSimDM(unsigned int addr)
// {
//     NumberOfWays = 1; // debugging: this part needs to enforce the special case that makes the SA cache a DM cache. Reviewing and testing are required.
//     calc(addr);
//     return cacheSimSA(addr);
// }

// Fully Associative Cache Simulator; however, it is redundant because we are going to use the cacheSimSA with special Line Size and Number of Ways conditions.
// cacheResType cacheSimFA(unsigned int addr)
// {
//     numSets = 1; // debugging: this part needs to enforce the special case that makes the SA cache a FA cache. Reviewing, testing, removing redundancies are required.
//     NumberOfWays = numLines;
//     calc(addr);
//     numSets = 1;
//     NumberOfWays = numLines;
//     return cacheSimSA(addr);
// }

const char *msg[2] = {"Miss", "Hit"}; // remove const before push

#define NO_OF_Iterations 1000000 // CHange to 1,000,000

int main()
{
    unsigned int hit = 0;
    unsigned int addr;
    cacheResType r;

    // set up files to manage the output
    ofstream myfile("results.csv");

    // debugging: uncomment only for debugging. Warning: output.txt becomes of immense size starting from 1 GB.
    // ofstream output;
    // output.open("output.txt");

    cout << "Set Associative Cache Simulator\n";

    myfile << "Line Size"
           << ","
           << "No. of Ways"
           << ","
           << "MemGen1"
           << ","
           << "MemGen2"
           << ","
           << "MemGen3"
           << ","
           << "MemGen4"
           << ","
           << "MemGen5"
           << ","
           << "MemGen6";

    for (int j = 0; j < 9; j++) // no ways = 4 line size --> 16,32,63,128; THEN, line size 32--> no of ways 1,2,4,8,16
    {
        myfile << endl;
        switch (j)
        {
        case 0:
            NumberOfWays = 4;
            LineSize = 16;
            cout << endl
                 << endl
                 << "Line Size: 16 " << endl
                 << "Number of Ways: 4 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 1:
            NumberOfWays = 4;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 4 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 2:
            NumberOfWays = 4;
            LineSize = 64;
            cout << endl
                 << endl
                 << "Line Size: 64 " << endl
                 << "Number of Ways: 4 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 3:
            NumberOfWays = 4;
            LineSize = 128;
            cout << endl
                 << endl
                 << "Line Size: 128 " << endl
                 << "Number of Ways: 4 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 4:
            NumberOfWays = 1;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 1 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 5:
            NumberOfWays = 2;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 2 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 6:
            NumberOfWays = 4;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 4 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 7:
            NumberOfWays = 8;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 8 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        case 8:
            NumberOfWays = 16;
            LineSize = 32;
            cout << endl
                 << endl
                 << "Line Size: 32 " << endl
                 << "Number of Ways: 16 " << endl
                 << endl;
            myfile << LineSize << "," << NumberOfWays << ",";
            break;
        }

        for (int i = 0; i < 6; i++)
        {
            // calculate the Tag, Index, and Offset
            calc(addr);

            // resize according to the calculated number of sets and number of ways
            cacheSA.resize(numSets);
            for (int k = 0; k < numSets; k++)
                cacheSA[k].resize(NumberOfWays);

            firstIn.resize(numSets, 0);

            // for each case, run the six generators
            for (int inst = 0; inst < NO_OF_Iterations; inst++)
            {
                switch (i)
                {
                case 0:
                    addr = memGen1();
                    break;
                case 1:
                    addr = memGen2();
                    break;
                case 2:
                    addr = memGen3();
                    break;
                case 3:
                    addr = memGen4();
                    break;
                case 4:
                    addr = memGen5();
                    break;
                case 5:
                    addr = memGen6();
                    break;
                }

                calc(addr);

                if (inst == 0) // debugging: change this condition to view the number of bits of the desired address (tracing)
                    cout << "#_Tag_bits = " << numTagBits << "    #_Index_bits = " << numIndexBits << "    #_Offset_bits = " << numLineOffsetBits << endl;

                r = cacheSimSA(addr);

                if (r == HIT)
                    hit++;

                // debugging: uncomment only for debugging. Warning: output.txt becomes of immense size starting from 1 GB.
                // output << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
            }

            // output the hit ratio
            cout << "memGen" << i + 1 << ": Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
            myfile << 100 * hit / NO_OF_Iterations << ",";

            // reset
            hit = 0;
            firstIn.clear();
        }
    }
    return 0;
}
