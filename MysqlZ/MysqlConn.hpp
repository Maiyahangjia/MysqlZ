
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
    //����һ��MYSQLʵ�����������ַ���
    MysqlConn();
    //�Ƿ�����Դ
    ~MysqlConn();
    //����ָ�������ݿ�
    bool connect(string ip, string user, string passwd, string bdName, unsigned int port);
    //����:���ӡ�ɾ�����޸�
    bool undate(string sql);
    //��ѯ
    bool query(string sql);
    //�����õ��Ľ����
    bool next();
    //��ȡ��������ֵ
    string value(int index);
    //�������ύ��ʽ
    bool transaction();
    //�����ύ
    bool commit();
    //����ع�
    bool rollback();
    //���¿���ʱ���
    void refreshAliveTime();
    //�������ӿ���ʱ��
    long long getAliveTime();
private:
    //ÿ����������Ҫ���½����
    void freeResult();
    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_result = nullptr;
    MYSQL_ROW m_row;

    steady_clock::time_point m_aliveTime;

};

#endif