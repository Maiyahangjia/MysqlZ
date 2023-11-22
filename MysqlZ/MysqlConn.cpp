#include "MysqlConn.hpp"


MysqlConn::MysqlConn()
{
    //获取一个MYSQL句柄
    m_conn = mysql_init(nullptr);
    VLog("mysql_init!!!!");

    
    //设置字符集
    mysql_set_character_set(m_conn, "utf8");
    VLog("mysql_set_character_set!!!!");
}

MysqlConn:: ~MysqlConn()
{
    if (m_conn != nullptr)
    {
        mysql_close(m_conn);
        VLog("关闭当前数据库连接!!!!");
    }
    VLog("析构数据库类!!!!");

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
    //得到结果集
    m_result = mysql_store_result(m_conn);

    return true;
}

bool MysqlConn::next()
{
    if (m_result != nullptr)
    {
        m_row = mysql_fetch_row(m_result);  //获取一行
        if (m_row != nullptr)
        {
            return true;
        }
    }

    return false;
}

string MysqlConn::value(int index)
{

    int rowCount = mysql_num_fields(m_result);  //返回结果集中字段数目
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
    return mysql_autocommit(m_conn, false);     //自动提交改为自动提交
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

//计算连接空闲时长
long long MysqlConn::getAliveTime()
{
    nanoseconds  res = steady_clock::now() - m_aliveTime;       //nanosecods 纳秒
    //milliseconds mil = duration_cast<microseconds>(res);        //将纳秒转成微妙

    //return mil.count();
    return 1000;
}