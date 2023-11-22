// MysqlZ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "MysqlConn.hpp"
int main1();

int main()
{
	MysqlConn M1;
	//M1.init();
	//main1();
}

int main1() {

	char host[10] = "localhost";
	char user[10] = "root";
	char password[10] = "root";
	char database[10] = "zyc";

	MYSQL mysql;
	if (!mysql_init(&mysql)) {
		cout << "初始化失败！" << endl;
		return EXIT_FAILURE;
		//exit(1);
	}

	if (mysql_real_connect(&mysql, host, user, password, database, 0, NULL, 0))
		cout << "数据库连接成功" << endl;
	else {
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
