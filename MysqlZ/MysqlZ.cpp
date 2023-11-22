// MysqlZ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "MysqlConn.hpp"

//测试用的模块函数
int Mysql();//测试数据库连接
int Config1();//读取配置文件方式一
int Config2();//读取配置文件方式二



int main()
{
	sVLog(INFO, __FILE__, __LINE__, __func__, "程序开始执行,go go go！！！");
	Config1();
	MysqlConn M1;
	//M1.init();
	Mysql();
	sVLog(INFO, __FILE__, __LINE__, __func__, "程序执行结束,end！！！");
}

int Mysql() {

	char host[10] = "localhost";
	char user[10] = "root";
	char password[10] = "root";
	char database[10] = "zyc";

	MYSQL mysql;
	if (!mysql_init(&mysql)) {
		cout << "初始化失败！" << endl;
		sVLog(ERROR, __FILE__, __LINE__, __func__, "初始化失败");
		return EXIT_FAILURE;
		//exit(1);
	}

	if (mysql_real_connect(&mysql, host, user, password, database, 0, NULL, 0))
	{
		cout << "数据库连接成功" << endl;
		sVLog(ERROR, __FILE__, __LINE__, __func__, "数据库连接失败");
	}
	else {
		sVLog(ERROR, __FILE__, __LINE__, __func__, "数据库连接失败");
		cout << "数据库连接失败，错误：" << mysql_error(&mysql) << endl;
		mysql_close(&mysql);
		return EXIT_FAILURE;
		//exit(1); 
	}

	if (mysql_set_character_set(&mysql, "GBK"))	cout << "字符集设置错误！" << endl;
	else	cout << "字符集设置成功！" << endl;

	char query[30] = "select * from student";
	if (mysql_query(&mysql, query))	 cout << "查询失败，错误：" << mysql_error(&mysql) << endl;
	else	cout << "查询成功！" << endl;




	MYSQL_RES* res;
	res = mysql_store_result(&mysql);
	//res = mysql_use_result(&mysql);
	//如果前边使用的是连接句柄对象，则此处只能使用"mysql_store_result"函数
	MYSQL_ROW row;
	for (int i = 0; i < mysql_num_rows(res); i++) {
		row = mysql_fetch_row(res);
		for (int j = 0; j < mysql_num_fields(res); j++)
			cout << row[j] << "\t";
		cout << endl;
	}

	mysql_free_result(res);

	mysql_close(&mysql);
	return 0;
}


int Config1()
{
	/*
		config.txt的文件内容：
		ip=localhost
		port=3306
		username=root
		password=root
		database=zyc

	*/
	int port = 0;
	std::string ip;
	std::string ports;
	std::string username;
	std::string password;
	std::string database;
	sVLog(INFO, __FILE__, __LINE__, __func__, "读取配置文件开始");
	ports = getString("port");
	ip = getString("ip");
	username = getString("username");
	password = getString("password");
	database = getString("database");
	/*
	std::cout << "ip:" << ip << std::endl;
	std::cout << "port:" << ports << std::endl;
	std::cout << "username:" << username << std::endl;
	std::cout << "password:" << password << std::endl;
	std::cout << "database:" << database << std::endl;
	*/
	sVLogFormt(INFO, __FILE__, __LINE__, __func__, "ip:", ip);
	sVLogFormt(INFO, __FILE__, __LINE__, __func__, "port:", getString("port"));
	sVLogFormt(INFO, __FILE__, __LINE__, __func__, "username", username);
	sVLogFormt(INFO, __FILE__, __LINE__, __func__, "password", password);
	sVLogFormt(INFO, __FILE__, __LINE__, __func__, "database", database);
	sVLog(INFO, __FILE__, __LINE__, __func__, "读取配置文件结束");
	return 0;
}

 
int Config2()
{
	/*
		config.txt的文件内容： 
		ip=localhost
		port=3306
		username=root
		password=root
		database=zyc
	
	*/
	int port;
	std::string ip;
	std::string username;
	std::string password;
	std::string database;
	const char ConfigFile[] = "config.txt";
	Config configSettings(ConfigFile);

	port = configSettings.Read("port", 0);
	ip = configSettings.Read("ip", ip);
	username = configSettings.Read("username", username);
	password = configSettings.Read("password", password);
	database = configSettings.Read("database", password);
	
	std::cout << "ip:" << ip << std::endl;
	std::cout << "port:" << port << std::endl;
	std::cout << "username:" << username << std::endl;
	std::cout << "password:" << password << std::endl;
	std::cout << "database:" << password << std::endl;

	return 0;
}