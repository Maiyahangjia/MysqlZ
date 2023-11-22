#include "ConnectionPool.hpp"
#include <fstream>
#include <thread>
//#include <jsoncpp/json/json.h>
#if 0
bool ConnectionPool::parseJsonFile()
{
    //��ȡ�����ļ�
    fstream ifs("../dbconf.json");
    //�������ļ�
    Json::Reader rd;
    Json::Value  root;
    rd.parse(ifs, root);     //���������ļ�
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
    //���������ļ�
    if (!parseJsonFile())
    {
        return;
    }

    //��������������
    for (int i = 0; i < m_minSize; ++i)
    {
        addConnect();
    }

    //�������߳����ڼ�Ⲣ�����µ�����
    thread producer(&ConnectionPool::produceConnection, this);
    //��������,��Ⲣ��������
    thread recycler(&ConnectionPool::recycleConnection, this);

    //�����̷߳���
    producer.detach();
    recycler.detach();
}

//���߳�-->������
void ConnectionPool::produceConnection()
{
    //ͨ����ѯ�ķ�ʽ���ϵ�ȥ���
    while (true)
    {
        //����������Դ,��Ҫ����
        unique_lock<mutex> locker(m_mutexQ);
        //�ж��������Ƿ�ﵽ����,������ڵ�����������Ҫ����һ��ʱ��
        while (m_connectionQ.size() >= m_maxSize)
        {
            m_cond.wait(locker);
        }
        addConnect();
        m_cond.notify_all();        //����������
    }
}
void ConnectionPool::recycleConnection()
{
    while (true)
    {
        //����һ����ʱ��
        this_thread::sleep_for(chrono::milliseconds(500));
        unique_lock<mutex> locker(m_mutexQ);
        //���̳߳������ٱ������� m_minSize���߳�
        while (m_connectionQ.size() > m_minSize)
        {
            MysqlConn* recyConn = m_connectionQ.front();
            //�����ʱ������
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

//����ӿ�,��ȡ�̳߳�
ConnectionPool* ConnectionPool::getConnectPool()
{
    static ConnectionPool pool;
    return &pool;
}
//��ȡ�̳߳��е�����
shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    //��Ҫ����������Դ
    unique_lock<mutex> locker(m_mutexQ);
    //�ж����ӳض���Ϊ��
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
    //�Զ���shared_ptr��������,���½����ӷŻص����ӳ���,����������
    shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn) {
        unique_lock<mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
        });
    //����,�ŵ��˶�β
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}
#endif