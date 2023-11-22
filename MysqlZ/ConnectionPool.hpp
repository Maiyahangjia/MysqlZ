#pragma once
#include "MysqlConn.hpp"


class ConnectionPool
{
public:
    //����ӿ�,��ȡ�̳߳�
    static ConnectionPool* getConnectPool();    //��̬�ֲ��������̰߳�ȫ��
    //��ȡ�̳߳��е�����
    shared_ptr<MysqlConn>   getConnection();
    //��ֹ���ͨ���������캯�����ƶ��������캯��
    ConnectionPool(const ConnectionPool& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool& obj) = delete;
    ~ConnectionPool();
private:
    //���캯��˽�л�
    ConnectionPool();
    //���������ļ�
    bool parseJsonFile();

    //������
    void produceConnection();   //�������ݿ�����
    void recycleConnection();   //�������ݿ�����
    void addConnect();          //�������

private:
    string m_user;
    string m_passwd;
    string m_ip;
    string m_dbName;
    unsigned short m_port;
    //���ӵ����޺�����,�Զ�ά���̳߳ص�������
    int m_minSize;
    int m_maxSize;
    //���ӵĳ�ʱʱ��
    int m_timeout;
    int m_maxIdleTime;
    //�߳�ͬ��  
    mutex m_mutexQ;                     //������
    condition_variable m_cond;          //��������
    queue<MysqlConn*>m_connectionQ;    //������Դ

};