using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO.Ports;
using CRC;

namespace HSRobot
{
    public partial class SendDataDlg : Form
    {
        //private const int WM_RECEIVEWRONGDATA = 0x297A;

        public 串口管理 m_SPManager;
        private string m_namePort;
        const string m_endFlag = "\r\n";
        const int m_length_dataSend = 16;
        private byte[] m_dataSend = new byte[m_length_dataSend]{
            0X0B,                       //0             开始标志位
            0X00,0X00,0X00,0X00,        //1,2,3,4       头部控制，第一位无效，其它（眼睛，脖子，手指）
            0X0A,0X86,0X00,0X00,0X00,   //5,6,7,8,9     底盘控制，前边两位主标示，其它（方向，圈/米，厘米/角度）
            0X00,0X00,                  //10,11         手臂控制，右，左
            0X00,0X00,                  //12,13         CRC校验位
            0X0D,0X0A                   //14,15         结束标志位
        };
        private byte[] m_const_dataSend = new byte[m_length_dataSend]{
            0X0B,                       //0             开始标志位
            0X00,0X00,0X00,0X00,        //1,2,3,4       头部控制，第一位无效，其它（眼睛，脖子，手指）
            0X0A,0X86,0X00,0X00,0X00,   //5,6,7,8,9     底盘控制，前边两位主标示，其它（方向，圈/米，厘米/角度）
            0X00,0X00,                  //10,11         手臂控制，右，左
            0X00,0X00,                  //12,13         CRC校验位
            0X0D,0X0A                   //14,15         结束标志位
        };
        //const string m_no_endFlag = "";
        public SendDataDlg(串口管理 SPManager)
        {
            InitializeComponent();
            m_SPManager = SPManager;            
        }

//         private void button1_Click(object sender, EventArgs e)
//         {
//             
//         }

        private void SendDataDlg_Load(object sender, EventArgs e)
        {
            ArrayList opend_ports=m_SPManager.GetOpendPorts();

            string[] existingPorts=串口管理.GetSerialPortNames();

            foreach(string existingPort in existingPorts)
            {
               listBox_existingPorts.Items.Add(existingPort);
            }
            foreach (SerialPort port in opend_ports)
            {
                comboBox_comOpendPortList.Items.Add(port.PortName);
            }
            if (opend_ports.Count!=0)
            {
                comboBox_comOpendPortList.SelectedIndex = 0;
            }
            this.radioButton_direction.Checked = true;
            this.textBox_setM.Text = "0";
            this.textBox_setCM.Text = "0";
            this.textBox_setTurn.Text = "0";
            this.textBox_setAngle.Text = "0";
        }

        private void 选择串口(object sender, EventArgs e)
        {
            m_namePort=(string)comboBox_comOpendPortList.SelectedItem;
        }

        private void listBox_existingPorts_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void button_sendData_Click(object sender, EventArgs e)
        {
            //string str = textBox_sendData.Text;
            //int len = str.Length;
            m_SPManager.发送数据(m_namePort, m_dataSend,m_length_dataSend);
        }
        private void CRCData(ref byte[] data)
        {
            byte[] toExamineData = new byte[m_length_dataSend - 4];
            for (int i = 0; i < m_length_dataSend - 4;i++ )
            {
                toExamineData[i] = data[i];
            }
            CRCModbus CRC = new CRCModbus();
            byte[] crcData =CRC.CRC16(toExamineData,m_length_dataSend-4);
            data[12] = crcData[0];
            data[13] = crcData[1];
        }
       
        //获得单个命令
        private void MakeSingleOrder(int offest,byte data)
        {
            ScaleDataSend(ref m_dataSend);
            m_dataSend[offest] = data;
            CRCData(ref m_dataSend);
            ShowSendData(m_dataSend);           
        }

        private void MakeOrder(ref byte [] dataSend,int offest, byte data)
        {           
            dataSend[offest] = data;
            CRCData(ref dataSend);
            for (int i = 0; i < m_length_dataSend;i++ )
            {
                m_dataSend[i] = dataSend[i];
            }
        }

        private void ScaleDataSend(ref byte[] dataSend)
        {
            for (int i = 0; i < m_length_dataSend;i++ )
            {
                dataSend[i]=m_const_dataSend[i];
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X00);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X01);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X02);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X03);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X04);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X05);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X06);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X07);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X08);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X09);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button12_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X0A);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button13_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(2, 0X0B);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button24_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(3, 0X00);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button23_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(3, 0X01);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button21_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(3, 0X02);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button15_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(3, 0X03);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button36_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X00);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button35_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X01);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button33_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X02);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button27_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X03);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button34_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X04);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button31_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(4, 0X05);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private bool DetectNumber(TextBox sender)
        {
            string text = sender.Text;
            if (text == ""||text==null)
            {
                return false;
            }
            int int_text = -1;
            try
            {
                int_text = int.Parse(text);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message + "请输入数字！");
                return false;
            }
            return true;
        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            DetectNumber(textBox_setM);
            
        }
        private void SendChassisData(TextBox one,TextBox two,byte way)
        {
            byte[] dataSend = new byte[m_length_dataSend];
            ScaleDataSend(ref dataSend);
            if (this.radioButton_direction.Checked)
            {
                MakeOrder(ref dataSend, 6, 0X86);
            }
            else if (this.radioButton_special.Checked)
            {
                MakeOrder(ref dataSend, 6, 0X68);
            }
            else if (this.radioButton_findLine.Checked)
            {
                MakeOrder(ref dataSend, 6, 0XA0);
            }
            MakeOrder(ref dataSend, 7, way);
            if (!DetectNumber(one) || !DetectNumber(two))
            {
                MessageBox.Show("请检查输入的数据是否有误！");
            }
            else
            {
                MakeOrder(ref dataSend, 8, (byte)int.Parse(one.Text));
                MakeOrder(ref dataSend, 9, (byte)int.Parse(two.Text));
            }

            m_SPManager.发送数据(m_namePort, dataSend, m_length_dataSend);

            for (int i = 0; i < m_length_dataSend; i++)
            {
                m_dataSend[i] = dataSend[i];
            }

            ShowSendData(m_dataSend);
        }
        private void ShowSendData(byte[] dataSend)
        {
            string show = "";
            for (int i = 0; i < m_length_dataSend;i++ )
            {
                string data= dataSend[i].ToString("X2");
                if (i==0||i==4||i==9||i==11||i==13)
                {
                    data += "||";
                }
                else if (i==15)
                {
                    data += "";
                }
                else
                {
                    data += ",";
                }
                show += data;
            }           
            m_state_showSendData.Text = show;
        }
        private void button29_Click(object sender, EventArgs e)
        {
            SendChassisData(textBox_setM, textBox_setCM, 0X00);
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            DetectNumber(textBox_setCM);
        }

        private void button28_Click(object sender, EventArgs e)
        {
            SendChassisData(textBox_setM, textBox_setCM, 0X01);
        }

        private void button30_Click(object sender, EventArgs e)
        {
            SendChassisData(textBox_setTurn, textBox_setAngle, 0X02);
        }

        private void button32_Click(object sender, EventArgs e)
        {
            SendChassisData(textBox_setTurn, textBox_setAngle, 0X03);
        }

        private void button17_Click(object sender, EventArgs e)
        {

        }

        private void button37_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(10, 49);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button38_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(10, 50);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button39_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(10, 51);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        private void button40_Click(object sender, EventArgs e)
        {
            MakeSingleOrder(10, 52);
            m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
        }

        protected override void WndProc(ref System.Windows.Forms.Message msg)
        {
            switch (msg.Msg)
            {
                case Form1.WM_RECEIVEWRONGDATA:
                    m_SPManager.发送数据(m_namePort, m_dataSend, m_length_dataSend);
                    break;
                default:
                    base.WndProc(ref msg);//调用基类函数处理非自定义消息。
                    break;
            }

        }

        private void button20_Click(object sender, EventArgs e)
        {

        }
    }
}
