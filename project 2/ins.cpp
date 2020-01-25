#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include "pin.H"
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;

ofstream OutFile;


std::ofstream *out = 0;
static UINT64 icountTotal = 0;
static UINT64 icountMemory = 0;
static UINT64 icountBranch = 0;
static UINT64 icountArithmetic = 0;

static std::unordered_map<string, int> freq;

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os;

    os << value;

    return os.str();
}
VOID docount(UINT32 i)
{
    //TASK 2
    if (i == 0)
    {
        icountMemory++;
    }
    else if (i == 1)
    {
        icountBranch++;
    }
    else if (i == 2)
    {
        icountArithmetic++;
    }

    //TASK 1
    icountTotal++;
}

VOID Instruction(INS ins, VOID *v)
{
    // TASK 3
    REG reg = INS_RegR(ins, 0);
    if (REG_valid(reg))
    {
        std::string reg_name = REG_StringShort(reg);
        std::unordered_map<string, int>::iterator it = freq.find(reg_name);
        if (it != freq.end())
        {
            it->second++;
        }
        else
        {
            freq.insert(std::make_pair(reg_name, 1));
        }
    }

    // TASK 2
    if (INS_IsMemoryRead(ins) || INS_IsMemoryWrite(ins))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_UINT32, 0, IARG_END);
    }

    else if (INS_IsBranch(ins))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_UINT32, 1, IARG_END);
    }

    else
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_UINT32, 2, IARG_END);
    }
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
                            "o", "ins.out", "specify output file name");

VOID Fini(INT32 code, VOID *v)
{
    OutFile.setf(ios::showbase);
    string output = "TASK 1\nTotal Instruction: " + to_string(icountTotal) +
                    "\n\n\nTASK 2\nMemory Instruction: " + to_string(icountMemory) +
                    "\nBranch Instructions: " + to_string(icountBranch) +
                    "\nArithmetic/Logic Operations: " + to_string(icountArithmetic) + "\n\n\nTASK 3\n";
    OutFile << output;
    for (auto &e : freq)
    {
        output = e.first + ": " + to_string(e.second) + "\n";
        OutFile << output;
    }
    OutFile.close();
}

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl
         << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}


int main(int argc, char *argv[])
{
    if (PIN_Init(argc, argv))
        return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    INS_AddInstrumentFunction(Instruction, 0);

    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram();

    return 0;
}
