#include "ConnectionPool.hpp"
#include <fstream>
#include <thread>
//#include <jsoncpp/json/json.h>
#if 0
bool ConnectionPool::parseJsonFile()
{
    //获取配置文件
    fstream ifs("../dbconf.json");
    //读配置文件
    Json::Reader rd;
    Json::Value  root;
    rd.parse(ifs, root);     //解析配置文件
    if (root.isObject())
    {
        m_ip = root["ip"].asString();
        m_user = root["username"].asString();
        m_passwd = root["password"].asString();
        m_dbName = root["dbName"].asString();
        m_port = root["port"].asInt();
        m_minSize = root["minSize"].asInt();
        m_maxSize = root["maxSize"].asInt();
        m_timeout = root["timeout"].asInt();
        m_maxIdleTime = root["maxTdleTime"].asInt();
        return true;
    }
    return false;
}

void ConnectionPool::addConnect()
{

    MysqlConn* conn = new MysqlConn;
    /*
        bool MysqlConn::connect(std::string ip, std::string user, std::string passwd,
        std::string bdName, unsigned int port)
    */
    conn->connect(m_ip, m_user, m_passwd, m_dbName, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

ConnectionPool::ConnectionPool()
{
    //加载配置文件
    if (!parseJsonFile())
    {
        return;
    }

    //创建最少连接数
    for (int i = 0; i < m_minSize; ++i)
    {
        addConnect();
    }

    //创建子线程用于检测并创建新的连接
    thread producer(&ConnectionPool::produceConnection, this);
    //销毁连接,检测并销毁连接
    thread recycler(&ConnectionPool::recycleConnection, this);

    //设置线程分离
    producer.detach();
    recycler.detach();
}

//子线程-->任务函数
void ConnectionPool::produceConnection()
{
    //通过轮询的方式不断的去检测
    while (true)
    {
        //操作共享资源,需要加锁
        unique_lock<mutex> locker(m_mutexQ);
        //判断连接数是否达到容量,如果大于等于容量则需要阻塞一段时间
        while (m_connectionQ.size() >= m_maxSize)
        {
            m_cond.wait(locker);
        }
        addConnect();
        m_cond.notify_all();        //唤醒消费者
    }
}
void ConnectionPool::recycleConnection()
{
    while (true)
    {
        //休眠一定的时长
        this_thread::sleep_for(chrono::milliseconds(500));
        unique_lock<mutex> locker(m_mutexQ);
        //让线程池中最少保持用于 m_minSize个线程
        while (m_connectionQ.size() > m_minSize)
        {
            MysqlConn* recyConn = m_connectionQ.front();
            //如果超时则销毁
            if (recyConn->getAliveTime() >= m_maxIdleTime)
            {
                m_connectionQ.pop();
                delete recyConn;
            }
            else
            {
                break;
            }
        }

    }
}

ConnectionPool::~ConnectionPool()
{
    while (!m_connectionQ.empty())
    {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}

//对外接口,获取线程池
ConnectionPool* ConnectionPool::getConnectPool()
{
    static ConnectionPool pool;
    return &pool;
}
//获取线程池中的连接
shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    //需要操作共享资源
    unique_lock<mutex> locker(m_mutexQ);
    //判断连接池队列为空
    while (m_connectionQ.empty())
    {
        if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout)))
        {
            if (m_connectionQ.empty())
            {
                continue;
            }
        }
    }
    //自定义shared_ptr析构方法,重新将连接放回到连接池中,而不是销毁
    shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn) {
        unique_lock<mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
        });
    //弹出,放到了队尾
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}
#endif