using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.Data;
using System.Windows.Forms;
using System.Collections;

namespace HSRobot
{
    class DBManager
    {
        private OleDbConnection dbconn; //数据库连接
        private OleDbDataAdapter da;

        public void InitDBManager(string path_DB)
        {
            dbconn = new OleDbConnection("provider=microsoft.jet.oledb.4.0; Data Source="+path_DB); 
        }
        public string GetAnswer(string ZIDUAN,string question, string def)
        {
            string kx = "";
            try
            {
                if (dbconn.State == ConnectionState.Closed)
                {
                    dbconn.Open();
                }

                string order = @"select * from TBL_CMD where CONTEXT = '" + question + @"'";
                da = new OleDbDataAdapter(order, dbconn); //引用数据库连接dbconn并依据SQL语句"select * from kaizhi"创建OleDbDataAdapter对象da
                DataSet ds = new DataSet(); //创建DataSet对象
                da.Fill(ds); //用OleDbDataAdapter对象da填充、更新刚创建的DataSet对象                
                kx = ds.Tables[0].Rows[0][ZIDUAN].ToString();
                dbconn.Close();
            }
            catch (System.Exception ex)
            {
                //MessageBox.Show(ex.Message);
            }
            if (kx == "" || kx == null)
            {
                kx = def;
            }
            return kx;
        }
        public string GetAnswer(string question,string def)
        {
            string kx = "";
            try
            {
                if (dbconn.State == ConnectionState.Closed)
                {
                    dbconn.Open();
                }

                string order = @"select * from TBL_CMD where CONTEXT='" + question + @"'";
                da = new OleDbDataAdapter(order, dbconn); //引用数据库连接dbconn并依据SQL语句"select * from kaizhi"创建OleDbDataAdapter对象da
                DataSet ds = new DataSet(); //创建DataSet对象
                da.Fill(ds); //用OleDbDataAdapter对象da填充、更新刚创建的DataSet对象                
                kx = ds.Tables[0].Rows[0]["RPS_TTS"].ToString();
                dbconn.Close();
            }
            catch (System.Exception ex)
            {
                //MessageBox.Show(ex.Message);
            }
            if (kx==""||kx==null)
            {
                kx = def;
            }
            return kx;
        }

        public ArrayList GetDatas(string field)
        {
            ArrayList array_Data = new ArrayList();
            
            try
            {
                if (dbconn.State == ConnectionState.Closed)
                {
                    dbconn.Open();
                }
                OleDbCommand cmd = new OleDbCommand();
                cmd.Connection = dbconn;
                cmd.CommandText = @"select * from TBL_CMD";
                cmd.CommandType = CommandType.Text;
                OleDbDataReader odr = cmd.ExecuteReader();
                while (odr.Read())
                {
                    array_Data.Add(odr[field].ToString());
                }                
                dbconn.Close();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }            

            return array_Data;
        }
    }
}
