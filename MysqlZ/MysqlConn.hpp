
#ifndef MYSQLCONN_HPP
#define MYSQLCONN_HPP
#include <mysql.h>
#include "log.hpp"
using namespace std::chrono_literals;
//using namespace std;
using namespace chrono;

class MysqlConn
{
public:
    //创建一个MYSQL实例对象并设置字符集
    MysqlConn();
    //是放在资源
    ~MysqlConn();
    //连接指定的数据库
    bool connect(string ip, string user, string passwd, string bdName, unsigned int port);
    //更新:增加、删除、修改
    bool undate(string sql);
    //查询
    bool query(string sql);
    //遍历得到的结果集
    bool next();
    //获取结果集里的值
    string value(int index);
    //事务处理提交方式
    bool transaction();
    //事务提交
    bool commit();
    //事务回滚
    bool rollback();
    //更新空闲时间点
    void refreshAliveTime();
    //计算连接空闲时长
    long long getAliveTime();
private:
    //每次搜索都需要更新结果集
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row;

    steady_clock::time_point m_aliveTime;

};

#endif