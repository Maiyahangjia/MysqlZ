#include "MysqlConn.hpp"


MysqlConn::MysqlConn()
{
    //��ȡһ��MYSQL���
    m_conn = mysql_init(nullptr);
    VLog("mysql_init!!!!");

    
    //�����ַ���
    mysql_set_character_set(m_conn, "utf8");
    VLog("mysql_set_character_set!!!!");
}

MysqlConn:: ~MysqlConn()
{
    if (m_conn != nullptr)
    {
        mysql_close(m_conn);
        VLog("�رյ�ǰ���ݿ�����!!!!");
    }
    VLog("�������ݿ���!!!!");

    freeResult();
}

bool MysqlConn::connect(string ip, string user, string passwd, string dbName, unsigned int port)
{
    /*
    MYSQL *mysql_real_connect(MYSQL *mysql, const char *host,
    const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long clientflag)
    */
    MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0);
    return p != nullptr;
}

bool MysqlConn::undate(string sql)
{
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }

    return true;
}

bool MysqlConn::query(string sql)
{
    freeResult();
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }
    //�õ������
    m_result = mysql_store_result(m_conn);

    return true;
}

bool MysqlConn::next()
{
    if (m_result != nullptr)
    {
        m_row = mysql_fetch_row(m_result);  //��ȡһ��
        if (m_row != nullptr)
        {
            return true;
        }
    }

    return false;
}

string MysqlConn::value(int index)
{

    int rowCount = mysql_num_fields(m_result);  //���ؽ�������ֶ���Ŀ
    if (index >= rowCount || index < 0)
    {
        return string();
    }

    char* ans = m_row[index];
    unsigned long length = mysql_fetch_lengths(m_result)[index];

    return string(ans, length);

}

bool MysqlConn::transaction()
{
    return mysql_autocommit(m_conn, false);     //�Զ��ύ��Ϊ�Զ��ύ
}

bool MysqlConn::commit()
{
    return mysql_commit(m_conn);
}

bool MysqlConn::rollback()
{
    return mysql_rollback(m_conn);
}

void MysqlConn::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void MysqlConn::refreshAliveTime()
{
    m_aliveTime = steady_clock::now();
}

//�������ӿ���ʱ��
long long MysqlConn::getAliveTime()
{
    nanoseconds  res = steady_clock::now() - m_aliveTime;       //nanosecods ����
    //milliseconds mil = duration_cast<microseconds>(res);        //������ת��΢��

    //return mil.count();
    return 1000;
}