#pragma once
#include "MysqlConn.hpp"


class ConnectionPool
{
public:
    //对外接口,获取线程池
    static ConnectionPool* getConnectPool();    //静态局部变量是线程安全的
    //获取线程池中的连接
    shared_ptr<MysqlConn>   getConnection();
    //防止外界通过拷贝构造函数和移动拷贝构造函数
    ConnectionPool(const ConnectionPool& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool& obj) = delete;
    ~ConnectionPool();
private:
    //构造函数私有化
    ConnectionPool();
    //解析配置文件
    bool parseJsonFile();

    //任务函数
    void produceConnection();   //生产数据库连接
    void recycleConnection();   //销毁数据库连接
    void addConnect();          //添加连接

private:
    string m_user;
    string m_passwd;
    string m_ip;
    string m_dbName;
    unsigned short m_port;
    //连接的上限和下限,自动维护线程池的连接数
    int m_minSize;
    int m_maxSize;
    //连接的超时时长
    int m_timeout;
    int m_maxIdleTime;
    //线程同步  
    mutex m_mutexQ;                     //互斥锁
    condition_variable m_cond;          //条件变量
    queue<MysqlConn*>m_connectionQ;    //共享资源

};