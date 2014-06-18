using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace HSRobot
{
    public partial class AddPersonDlg : Form
    {
        public string personName;
        public AddPersonDlg()
        {
            InitializeComponent();            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            personName = this.textBox_personName.Text;
            if(personName=="")
            {
                MessageBox.Show("请输入要添加好友的名字！");
                return;
            }
            this.Close();
        }

        private void button_Cancle_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
