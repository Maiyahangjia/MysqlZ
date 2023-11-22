#include "log.hpp"


string GetProgramDir()
{
	char FullPath[MAX_PATH]; // 声明路径
	string strPath = "";

	GetModuleFileNameA(NULL, FullPath, MAX_PATH);  // 获取当前运行程序的绝对路径
	strPath = (string)FullPath;    // 转为string型

	int pos = static_cast<int>(strPath.find_last_of('\\', strPath.length()));
	return strPath.substr(0, pos);  // 返回当前文件夹，不带文件名
}

string GetCurrentTimeA(tm in)
{
	tm* ct = &in;
	int year, month, day, hour, minute, second;// 年月日时分秒。
	year = ct->tm_year + 1900;                 // 年份基础从1900开始的，所以要加上
	month = ct->tm_mon + 1;                    // 月份是0-11，对应1-12月
	day = ct->tm_mday;
	hour = ct->tm_hour;
	minute = ct->tm_min;
	second = ct->tm_sec;

	char temp[100];                            // 创建字符数组。
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d: ", year, month, day, hour, minute, second);// 时间信息合并。
	string out(temp);                          // 转化为string型
	return move(out);                          // 用move（string）速度快很多。
}

int VLog(string msg)
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	string dtime = GetCurrentTimeA(*local);
	ofstream outfile;
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
	if (outfile.is_open())
	{
		outfile << msg << "\n";
		outfile.close();
		return 0;
	}
	else
	{
		return 1;
	}
}

int sVLog(int level,string file,int line, string func,string msg)
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	string dtime = GetCurrentTimeA(*local);
	ofstream outfile;
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
	if (outfile.is_open())
	{
		outfile <<level<< "|"<< dtime <<"|"<< file << "|" << line<<"|" <<func << "|" << msg << "\n";
		outfile.close();
		return 0;
	}
	else
	{
		return 1;
	}
}


int sVLogFormt(int level, string file, int line, string func, string msg,string msg1)
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	string dtime = GetCurrentTimeA(*local);
	ofstream outfile;
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
	if (outfile.is_open())
	{
		outfile << level << "|" << dtime << "|" << file << "|" << line << "|" << func << "|" << msg << msg1 << "\n";
		outfile.close();
		return 0;
	}
	else
	{
		return 1;
	}
}