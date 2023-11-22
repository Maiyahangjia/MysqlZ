#include "Config.hpp"
using namespace std;
class Vlog {
public:
	void init();

};
enum  {
	DEBUG,
	INFO,
	WARRING,
	ERR
};
string GetProgramDir();
string GetCurrentTimeA(tm in);
int VLog(string msg);
int sVLog(int level, string file,  int line, string func, string msg);
int sVLogFormt(int level, string file, int line, string func, string msg, string msg1);
