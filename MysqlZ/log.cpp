#include "log.hpp"


string GetProgramDir()
{
	char FullPath[MAX_PATH]; // ����·��
	string strPath = "";

	GetModuleFileNameA(NULL, FullPath, MAX_PATH);  // ��ȡ��ǰ���г���ľ���·��
	strPath = (string)FullPath;    // תΪstring��

	int pos = static_cast<int>(strPath.find_last_of('\\', strPath.length()));
	return strPath.substr(0, pos);  // ���ص�ǰ�ļ��У������ļ���
}

string GetCurrentTimeA(tm in)
{
	tm* ct = &in;
	int year, month, day, hour, minute, second;// ������ʱ���롣
	year = ct->tm_year + 1900;                 // ��ݻ�����1900��ʼ�ģ�����Ҫ����
	month = ct->tm_mon + 1;                    // �·���0-11����Ӧ1-12��
	day = ct->tm_mday;
	hour = ct->tm_hour;
	minute = ct->tm_min;
	second = ct->tm_sec;

	char temp[100];                            // �����ַ����顣
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d: ", year, month, day, hour, minute, second);// ʱ����Ϣ�ϲ���
	string out(temp);                          // ת��Ϊstring��
	return move(out);                          // ��move��string���ٶȿ�ܶࡣ
}

int VLog(string msg)
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	string dtime = GetCurrentTimeA(*local);
	ofstream outfile;
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //�ļ��������ַ���ļ��Ĵ򿪷�ʽ, ���û�л��Զ�����
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
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //�ļ��������ַ���ļ��Ĵ򿪷�ʽ, ���û�л��Զ�����
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
	outfile.open(GetProgramDir() + "\\" + "log.txt", ios::app); //�ļ��������ַ���ļ��Ĵ򿪷�ʽ, ���û�л��Զ�����
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