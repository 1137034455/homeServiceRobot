using System.Windows.Forms;
namespace HSRobot
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.加个朋友ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.加个朋友ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.训练ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.识别ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.训练设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.打开摄像头ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.其它命令ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.更新语法库ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.重新加载语法库ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.显示其他窗口ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.隐藏其他窗口ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.发送串口数据ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.唱首歌ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.button_打开摄像头 = new System.Windows.Forms.Button();
            this.button_训练 = new System.Windows.Forms.Button();
            this.button_添加好友 = new System.Windows.Forms.Button();
            this.button_识别 = new System.Windows.Forms.Button();
            this.listBox_showSRData = new System.Windows.Forms.ListBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.statusShow = new System.Windows.Forms.ToolStripStatusLabel();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.FRManager = new AxFRManagerLib.AxFRManager();
            this.SRecoManager = new AxSRecoManagerLib.AxSRecoManager();
            this.label1 = new System.Windows.Forms.Label();
            this.button_openSendDataDlg = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FRManager)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SRecoManager)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.加个朋友ToolStripMenuItem,
            this.打开摄像头ToolStripMenuItem,
            this.其它命令ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStrip1.Size = new System.Drawing.Size(920, 25);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 加个朋友ToolStripMenuItem
            // 
            this.加个朋友ToolStripMenuItem.AutoSize = false;
            this.加个朋友ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.加个朋友ToolStripMenuItem1,
            this.训练ToolStripMenuItem,
            this.识别ToolStripMenuItem,
            this.训练设置ToolStripMenuItem});
            this.加个朋友ToolStripMenuItem.Name = "加个朋友ToolStripMenuItem";
            this.加个朋友ToolStripMenuItem.Size = new System.Drawing.Size(68, 21);
            this.加个朋友ToolStripMenuItem.Text = "人脸识别";
            // 
            // 加个朋友ToolStripMenuItem1
            // 
            this.加个朋友ToolStripMenuItem1.Name = "加个朋友ToolStripMenuItem1";
            this.加个朋友ToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.加个朋友ToolStripMenuItem1.Text = "加个朋友";
            this.加个朋友ToolStripMenuItem1.Click += new System.EventHandler(this.加个朋友);
            // 
            // 训练ToolStripMenuItem
            // 
            this.训练ToolStripMenuItem.Name = "训练ToolStripMenuItem";
            this.训练ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.训练ToolStripMenuItem.Text = "训练";
            this.训练ToolStripMenuItem.Click += new System.EventHandler(this.训练);
            // 
            // 识别ToolStripMenuItem
            // 
            this.识别ToolStripMenuItem.Name = "识别ToolStripMenuItem";
            this.识别ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.识别ToolStripMenuItem.Text = "识别";
            this.识别ToolStripMenuItem.Click += new System.EventHandler(this.识别);
            // 
            // 训练设置ToolStripMenuItem
            // 
            this.训练设置ToolStripMenuItem.Name = "训练设置ToolStripMenuItem";
            this.训练设置ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.训练设置ToolStripMenuItem.Text = "训练设置";
            this.训练设置ToolStripMenuItem.Click += new System.EventHandler(this.训练设置);
            // 
            // 打开摄像头ToolStripMenuItem
            // 
            this.打开摄像头ToolStripMenuItem.Name = "打开摄像头ToolStripMenuItem";
            this.打开摄像头ToolStripMenuItem.Size = new System.Drawing.Size(80, 21);
            this.打开摄像头ToolStripMenuItem.Text = "打开摄像头";
            this.打开摄像头ToolStripMenuItem.Click += new System.EventHandler(this.OpenCamera);
            // 
            // 其它命令ToolStripMenuItem
            // 
            this.其它命令ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.更新语法库ToolStripMenuItem,
            this.重新加载语法库ToolStripMenuItem,
            this.显示其他窗口ToolStripMenuItem,
            this.隐藏其他窗口ToolStripMenuItem,
            this.发送串口数据ToolStripMenuItem,
            this.唱首歌ToolStripMenuItem});
            this.其它命令ToolStripMenuItem.Name = "其它命令ToolStripMenuItem";
            this.其它命令ToolStripMenuItem.Size = new System.Drawing.Size(68, 21);
            this.其它命令ToolStripMenuItem.Text = "其它操作";
            // 
            // 更新语法库ToolStripMenuItem
            // 
            this.更新语法库ToolStripMenuItem.Name = "更新语法库ToolStripMenuItem";
            this.更新语法库ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.更新语法库ToolStripMenuItem.Text = "更新语法库";
            this.更新语法库ToolStripMenuItem.Click += new System.EventHandler(this.更新语法库ToolStripMenuItem_Click);
            // 
            // 重新加载语法库ToolStripMenuItem
            // 
            this.重新加载语法库ToolStripMenuItem.Name = "重新加载语法库ToolStripMenuItem";
            this.重新加载语法库ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.重新加载语法库ToolStripMenuItem.Text = "重新加载语法库";
            this.重新加载语法库ToolStripMenuItem.Click += new System.EventHandler(this.重新加载语法库ToolStripMenuItem_Click);
            // 
            // 显示其他窗口ToolStripMenuItem
            // 
            this.显示其他窗口ToolStripMenuItem.Name = "显示其他窗口ToolStripMenuItem";
            this.显示其他窗口ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.显示其他窗口ToolStripMenuItem.Text = "显示其他窗口";
            this.显示其他窗口ToolStripMenuItem.Click += new System.EventHandler(this.显示其他窗口ToolStripMenuItem_Click);
            // 
            // 隐藏其他窗口ToolStripMenuItem
            // 
            this.隐藏其他窗口ToolStripMenuItem.Name = "隐藏其他窗口ToolStripMenuItem";
            this.隐藏其他窗口ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.隐藏其他窗口ToolStripMenuItem.Text = "隐藏其他窗口";
            this.隐藏其他窗口ToolStripMenuItem.Click += new System.EventHandler(this.隐藏其他窗口ToolStripMenuItem_Click);
            // 
            // 发送串口数据ToolStripMenuItem
            // 
            this.发送串口数据ToolStripMenuItem.Name = "发送串口数据ToolStripMenuItem";
            this.发送串口数据ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.发送串口数据ToolStripMenuItem.Text = "发送串口数据";
            this.发送串口数据ToolStripMenuItem.Click += new System.EventHandler(this.发送串口数据ToolStripMenuItem_Click);
            // 
            // 唱首歌ToolStripMenuItem
            // 
            this.唱首歌ToolStripMenuItem.Name = "唱首歌ToolStripMenuItem";
            this.唱首歌ToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.唱首歌ToolStripMenuItem.Text = "唱首歌";
            this.唱首歌ToolStripMenuItem.Click += new System.EventHandler(this.唱首歌ToolStripMenuItem_Click);
            // 
            // button_打开摄像头
            // 
            this.button_打开摄像头.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button_打开摄像头.Image = ((System.Drawing.Image)(resources.GetObject("button_打开摄像头.Image")));
            this.button_打开摄像头.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.button_打开摄像头.Location = new System.Drawing.Point(0, 28);
            this.button_打开摄像头.Name = "button_打开摄像头";
            this.button_打开摄像头.Size = new System.Drawing.Size(80, 80);
            this.button_打开摄像头.TabIndex = 1;
            this.button_打开摄像头.UseVisualStyleBackColor = true;
            this.button_打开摄像头.Click += new System.EventHandler(this.button_打开摄像头_Click);
            // 
            // button_训练
            // 
            this.button_训练.Image = ((System.Drawing.Image)(resources.GetObject("button_训练.Image")));
            this.button_训练.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.button_训练.Location = new System.Drawing.Point(0, 185);
            this.button_训练.Name = "button_训练";
            this.button_训练.Size = new System.Drawing.Size(80, 80);
            this.button_训练.TabIndex = 1;
            this.button_训练.UseVisualStyleBackColor = true;
            this.button_训练.Click += new System.EventHandler(this.button_训练_Click);
            // 
            // button_添加好友
            // 
            this.button_添加好友.Image = ((System.Drawing.Image)(resources.GetObject("button_添加好友.Image")));
            this.button_添加好友.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.button_添加好友.Location = new System.Drawing.Point(0, 105);
            this.button_添加好友.Name = "button_添加好友";
            this.button_添加好友.Size = new System.Drawing.Size(80, 80);
            this.button_添加好友.TabIndex = 1;
            this.button_添加好友.UseVisualStyleBackColor = true;
            this.button_添加好友.Click += new System.EventHandler(this.button_添加好友_Click);
            // 
            // button_识别
            // 
            this.button_识别.Image = ((System.Drawing.Image)(resources.GetObject("button_识别.Image")));
            this.button_识别.Location = new System.Drawing.Point(0, 265);
            this.button_识别.Name = "button_识别";
            this.button_识别.Size = new System.Drawing.Size(80, 80);
            this.button_识别.TabIndex = 1;
            this.button_识别.UseVisualStyleBackColor = true;
            this.button_识别.Click += new System.EventHandler(this.button_识别_Click);
            // 
            // listBox_showSRData
            // 
            this.listBox_showSRData.FormattingEnabled = true;
            this.listBox_showSRData.ItemHeight = 12;
            this.listBox_showSRData.Location = new System.Drawing.Point(12, 479);
            this.listBox_showSRData.Name = "listBox_showSRData";
            this.listBox_showSRData.Size = new System.Drawing.Size(177, 64);
            this.listBox_showSRData.TabIndex = 13;
            this.listBox_showSRData.SelectedIndexChanged += new System.EventHandler(this.语音选择发生变化);
            // 
            // panel1
            // 
            this.panel1.Location = new System.Drawing.Point(732, 28);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(188, 577);
            this.panel1.TabIndex = 15;
            // 
            // panel2
            // 
            this.panel2.Location = new System.Drawing.Point(572, 28);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(348, 577);
            this.panel2.TabIndex = 17;
            this.panel2.Visible = false;
            this.panel2.Paint += new System.Windows.Forms.PaintEventHandler(this.panel2_Paint);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusShow});
            this.statusStrip.Location = new System.Drawing.Point(0, 580);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(920, 22);
            this.statusStrip.TabIndex = 27;
            this.statusStrip.Text = "statusStrip1";
            // 
            // statusShow
            // 
            this.statusShow.Name = "statusShow";
            this.statusShow.Size = new System.Drawing.Size(0, 17);
            // 
            // treeView1
            // 
            this.treeView1.Location = new System.Drawing.Point(417, 233);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(121, 97);
            this.treeView1.TabIndex = 31;
            // 
            // serialPort
            // 
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.接收到数据);
            // 
            // FRManager
            // 
            this.FRManager.Enabled = true;
            this.FRManager.Location = new System.Drawing.Point(86, 28);
            this.FRManager.Name = "FRManager";
            this.FRManager.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("FRManager.OcxState")));
            this.FRManager.Size = new System.Drawing.Size(480, 320);
            this.FRManager.TabIndex = 37;
            this.FRManager.Enter += new System.EventHandler(this.FRManager_Enter);
            // 
            // SRecoManager
            // 
            this.SRecoManager.Enabled = true;
            this.SRecoManager.Location = new System.Drawing.Point(309, 233);
            this.SRecoManager.Name = "SRecoManager";
            this.SRecoManager.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("SRecoManager.OcxState")));
            this.SRecoManager.Size = new System.Drawing.Size(124, 63);
            this.SRecoManager.TabIndex = 29;
            this.SRecoManager.SRecognizeEvent += new System.EventHandler(this.监听到数据);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 464);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 39;
            this.label1.Text = "监听到的数据：";
            // 
            // button_openSendDataDlg
            // 
            this.button_openSendDataDlg.Image = ((System.Drawing.Image)(resources.GetObject("button_openSendDataDlg.Image")));
            this.button_openSendDataDlg.Location = new System.Drawing.Point(0, 342);
            this.button_openSendDataDlg.Name = "button_openSendDataDlg";
            this.button_openSendDataDlg.Size = new System.Drawing.Size(80, 80);
            this.button_openSendDataDlg.TabIndex = 1;
            this.button_openSendDataDlg.UseVisualStyleBackColor = true;
            this.button_openSendDataDlg.Click += new System.EventHandler(this.button_打开发送串口对话框_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(920, 602);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.FRManager);
            this.Controls.Add(this.treeView1);
            this.Controls.Add(this.SRecoManager);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.listBox_showSRData);
            this.Controls.Add(this.button_添加好友);
            this.Controls.Add(this.button_openSendDataDlg);
            this.Controls.Add(this.button_识别);
            this.Controls.Add(this.button_训练);
            this.Controls.Add(this.button_打开摄像头);
            this.Controls.Add(this.menuStrip1);
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximumSize = new System.Drawing.Size(2000, 1000);
            this.Name = "Form1";
            this.Text = "家庭服务机器人";
            this.TransparencyKey = System.Drawing.Color.Black;
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.SizeChanged += new System.EventHandler(this.FormSizeChanged);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.窗口重绘);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FRManager)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SRecoManager)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 加个朋友ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 加个朋友ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem 训练ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 识别ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 打开摄像头ToolStripMenuItem;
        private System.Windows.Forms.Button button_打开摄像头;
        private System.Windows.Forms.Button button_训练;
        private System.Windows.Forms.Button button_添加好友;
        private System.Windows.Forms.Button button_识别;
        private System.Windows.Forms.ToolStripMenuItem 训练设置ToolStripMenuItem;
        private System.Windows.Forms.ListBox listBox_showSRData;
        private Panel panel1;
        private Panel panel2;
        private ToolStripMenuItem 其它命令ToolStripMenuItem;
        private ToolStripMenuItem 更新语法库ToolStripMenuItem;
        private StatusStrip statusStrip;
        private ToolStripStatusLabel statusShow;
        private AxSRecoManagerLib.AxSRecoManager SRecoManager;
        private ToolStripMenuItem 重新加载语法库ToolStripMenuItem;
        private TreeView treeView1;
        private ToolStripMenuItem 显示其他窗口ToolStripMenuItem;
        private ToolStripMenuItem 隐藏其他窗口ToolStripMenuItem;
        private System.IO.Ports.SerialPort serialPort;
        private ToolStripMenuItem 发送串口数据ToolStripMenuItem;
        private ToolStripMenuItem 唱首歌ToolStripMenuItem;
        private AxFRManagerLib.AxFRManager FRManager;
        private Label label1;
        private Button button_openSendDataDlg;
    }
}

