using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections;
using System.Threading;


namespace HSRobot
{
    public partial class MainForm : Form
    {
        public static string m_formText = "家庭服务机器人";

        private DBManager m_DBManager;
        private const int WM_SR2HSRobot=0x1234;
        private const int WM_SETTEXT = 0x000C;
        private const int WM_COPYDATA = 0x004A;
        public const int WM_RECEIVEWRONGDATA=0x297A;
        

        private bool f_OpenCam=false;
        private FaceBase m_faceBase;
        private string trainPerson;
        private TrainSetDlg m_trainSetdlg;
        private TrainDlg m_trainDlg;
        private const int WM_CHAR = 0x0102;

        private 串口管理 m_SPManager;
        private string m_iniFileName;
        private string m_rootPath;
        public ArrayList m_SPList;
        string m_name_errorReturnPort;

        private string m_DBPath;
        private string m_ClassifierPath;
        private string m_GrammarPath;
        private string m_nameOtherWindow;
        //private string trainPerson;      
        public MainForm()
        {
            InitializeComponent();            
        }

        private void OpenCamera(object sender, EventArgs e)
        {
            OpenCamera();    
        }

        private void 加个朋友(object sender, EventArgs e)
        {
            if (f_OpenCam)
            {
                AddPerson();
            }
            else
            {
                MessageBox.Show("请先打开摄像头！");
            } 
        }

        private void 训练(object sender, EventArgs e)
        {            
            TrainPerson(trainPerson);
        }

        private void 识别(object sender, EventArgs e)
        {
            string stringResult=Recognize();
            if (stringResult!=null&&stringResult!="")
            {
                MessageBox.Show(stringResult);
            }              
        }

        private void 训练设置(object sender, EventArgs e)
        {
            TrainSetDlg dlg = new TrainSetDlg();
            dlg.m_faceBase=m_faceBase;
            dlg.ShowDialog(this);
            if (dlg.selectedPerson!=null&&dlg.selectedPerson!="")
            {
                trainPerson = dlg.selectedPerson;
            }
        }

        private void button_打开摄像头_Click(object sender, EventArgs e)
        {
            OpenCamera();
        }

        private void button_训练_Click(object sender, EventArgs e)
        {
            TrainPerson(trainPerson);         
        }

        private void button_添加好友_Click(object sender, EventArgs e)
        {       
            if (f_OpenCam)
            {
                AddPerson();
            }
            else
            {
                MessageBox.Show("请先打开摄像头！");
            }
            
        }
        
        private void button_识别_Click(object sender, EventArgs e)
        {            
            string stringResult = Recognize();
            if (stringResult != null&&stringResult!="")
            {
                //MessageBox.Show(stringResult);
                string str_Show = "朋友，你是" + stringResult;
                SRecoManager.Speak(str_Show);
                statusShow.Text = str_Show;
            }   
        }
        
        private void AddPerson()
        {
            AddPersonDlg dlg = new AddPersonDlg();
            dlg.ShowDialog(this);
            string personName = dlg.personName;
            if(personName!=null&&personName!="")
            {
                FRManager.AddPerson(personName);
                trainPerson = personName;
                ShowTrainDlg(personName);
            }                 
        }

        private void TrainPerson(string personName)
        {
            if (f_OpenCam)
            {
                
                if (personName!=null&&personName!="")
                {
                    FRManager.TrainPerson(personName);
                }
                else
                {
                    MessageBox.Show("请先选择要训练的好友！");
                }
            }
            else
                MessageBox.Show("请先打开摄像头！");
        }

        private string Recognize()
        {
            if (f_OpenCam)
            {
                string nameResult;
                nameResult = FRManager.Recognize();               
                return nameResult;               
            }
            else
            {
                MessageBox.Show("请先打开摄像头！");
                return null;
            }
        }

        private void OpenCamera()
        {
            if (!f_OpenCam)
            {
                if (FRManager.OpenCamera() != -1)
                {
                    f_OpenCam = true;
                }
                
            }  
        }

        private void FormSizeChanged(object sender, EventArgs e)
        {
//             this.Size= new Size(this.Size.Width / 640 * 640,this.Size.Height/480*480);
//             float factor = this.Size.Width / 640;
//             FRManager.Size = new Size((int)(factor*640),(int)(factor*480));
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
            m_iniFileName =  System.Environment.CurrentDirectory+@"\Config.ini";
            string def="def";
            m_rootPath = ReadIni("BASE","RootPath",def);
            if (m_rootPath!=def)
            {
                m_faceBase = new FaceBase(m_rootPath,true);
            }
            

            InitDataBase();

            InitFRManager();

            InitSRManager();
                      
            InitSerialPort();

            ShowTrainSetDlg();

            //改变列表框的显示位置
            int width = listBox_showSRData.Size.Width;
            int height = this.ClientRectangle.Size.Height - listBox_showSRData.Location.Y - 25;
            listBox_showSRData.Size = new Size(width, height);
        }
        /************************************************************************/
        /* 初始化语音识别管理器                                                                     */
        /************************************************************************/
        private void InitSRManager()
        {   
            m_GrammarPath = ReadIni("SRMANAGER", "GrammarPath", "");
            if (m_GrammarPath!="")
            {
                try
                {
                    SRecoManager.InitSRManager(m_GrammarPath);  
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message + "。请重新在Config.ini文件中设置语法库的路径！");
                }                
            }
            else
            {
                MessageBox.Show("没有设置语法库路径。请在Config.ini文件中设置语法库的路径！");
            }
        }

        private void InitFRManager()
        {
            string def = "";
            m_ClassifierPath = ReadIni("FRMANAGER", "ClassifierPath", def);
            string recordPath = ReadIni("BASE","RecordPath",def);
            if (m_ClassifierPath != ""&&recordPath!=def)
            {
                try
                {
                    FRManager.InitFRManager2(m_ClassifierPath,recordPath);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message + "。请重新在Config.ini文件中设置分类器的路径！");
                }
            }
            else
            {
                MessageBox.Show("没有设置分类器路径。请在Config.ini文件中设置分类器的路径！");
            }
        }
        /// <summary>
        /// 初始化数据库文件
        /// </summary>
        private void InitDataBase()
        {
            m_DBManager = new DBManager();
            m_DBPath = ReadIni("DATABASE", "DatabasePath", "");
            if (m_DBPath != "")
            {
                try
                {
                    m_DBManager.InitDBManager(m_DBPath);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message + "。请重新在Config.ini文件中设置数据库的路径！");
                }

            }
            else
            {
                MessageBox.Show("没有设置数据库路径。请在Config.ini文件中设置数据库的路径！");
            }
        }

        /// <summary>
        /// 初始化串口管理
        /// </summary>
        private void InitSerialPort()
        {
            m_SPManager = new 串口管理();
            m_SPList = new ArrayList();
            
            
            int nPorts=0;
            try
            {
                nPorts = int.Parse(ReadIni("COMSET", "nPorts", ""));
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message + "，请在Config.ini文件中设置串口的个数为数字格式！");
            }
            for (int i=1;i<=nPorts;i++)
            {
                string name_port="ComPortName"+i.ToString();
                string name_comPort = ReadIni("COMSET", name_port, "");
                if (name_comPort == "")
                {
                    return;
                }
                串口名及其事件 SPNameAndEvent = new 串口名及其事件();
                SPNameAndEvent.串口名 = name_comPort;
                SPNameAndEvent.串口事件 = 接收到数据;
                m_SPList.Add(SPNameAndEvent);    
                m_SPManager.增加串口(SPNameAndEvent);
                if (!m_SPManager.打开串口(name_comPort))
                {
                    string show = name_comPort + "未能打开！";
                    MessageBox.Show(show);
                }               
            }
            string def = "def";
            m_name_errorReturnPort = ReadIni("COMSET", "ErrorReturnPort", def);        
        }

        /// <summary>
        /// 读取配置文件
        /// </summary>
        /// <param name="Section">某个字段</param>
        /// <param name="Ident">属性名称</param>
        /// <param name="Default">如果没有找到相应属性的数据则返回Default</param>
        /// <returns></returns>
        public string ReadIni(string Section, string Ident, string Default)
        {
            return FileUtil.ReadIni(m_iniFileName, Section, Ident, Default);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ShowTrainDlg("李智博");
        }
        /// <summary>
        /// 主要的窗口回调函数
        /// </summary>
        /// <param name="msg"></param>
        protected override void WndProc(ref System.Windows.Forms.Message msg)
        {
            switch (msg.Msg)
            {
                case TrainDlg.WM_CLOSETRAINDLG: //处理消息
                    {
                        trainPerson = "";
                        ShowTrainSetDlg();
                    }

                    break;
                case TrainSetDlg.WM_CLOSETRAINSETDLG:
                    {
                        trainPerson = Marshal.PtrToStringAnsi(msg.WParam);
                        ShowTrainDlg(trainPerson);
                    }
                    break;
                case WM_CHAR:
                    {
                        //MessageBox.Show("已经收到训练完成的信号！");
                        AddPersonPictureToTrainDlg();
                        AddPersonPictureToTrainSetDlg();
                    }
                    break;

                case WM_SR2HSRobot:
                    {
                        this.listBox_showSRData.Items.Add(GetSRResult());
                        this.statusShow.Text = GetSRResult();
                        //MessageBox.Show(GetSRResult());
                    }
                    break;

                default:
                    base.WndProc(ref msg);//调用基类函数处理非自定义消息。
                    break;
            }

        }
        private string GetSRResult()
        {
            string strFile =/* m_faceBase.rootPath + */@"temporary.dat";
            string str = "" ;
            try
            {
                if (File.Exists(strFile))
                {
                    using (StreamReader sr = new StreamReader(strFile, Encoding.GetEncoding("gb2312")))
                    {
                        ArrayList str_list = new ArrayList();
                        while (sr.Peek() >= 0)
                        {
                            str = sr.ReadLine();
                        }
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            return str;
        }

        private void AddPersonPictureToTrainDlg()
        {
            m_trainDlg.AddNewPicbox();
        }

        private void AddPersonPictureToTrainSetDlg()
        {
            m_trainSetdlg.AddNewPicbox();
        }

        private void ShowOtherWindow(string name_otherWindow, int flag_fullScreen = 1)
        {
            IntPtr hWnd = DllManager.FindWindow(null, name_otherWindow);
            if (hWnd != IntPtr.Zero)
            {

                //                 Rectangle rect=new Rectangle();
                //                 GetWindowRect(hWnd, ref rect);
                Rectangle rect = new Rectangle(new Point(0, 0), new Size(0, 0));
                if (flag_fullScreen == 1)
                {
                    rect = System.Windows.Forms.Screen.PrimaryScreen.WorkingArea;
                }
                else
                {
                    DllManager.GetWindowRect(hWnd, ref rect);
                }
                DllManager.SetWindowPos(hWnd, -1, 0, 0, rect.Width, rect.Height, 0x4000 | 0x0040);
                DllManager.ShowWindow(hWnd, 1);
            }
        }

        private void HideOtherWindow(string name_otherWindow)
        {
            IntPtr hWnd = DllManager.FindWindow(null, name_otherWindow);
            if (hWnd != IntPtr.Zero)
            {
                DllManager.ShowWindow(hWnd, 0);
            }
        }

        private void ShowTrainSetDlg()
        {
            //panel2.SendToBack();
            panel2.Visible = false;
            panel1.Visible = true;
            panel1.Controls.Clear();

            m_trainSetdlg = new TrainSetDlg();
                       
            panel1.Location = new Point(this.FRManager.Location.X + this.FRManager.Width, 28);
            Size size = new Size(this.Size.Width - panel1.Location.X - 20, this.Size.Height);
            panel1.Size = size;

            m_trainSetdlg.Size = size;
            m_trainSetdlg.ControlBox = false;
            m_trainSetdlg.Text = "";
            m_trainSetdlg.m_faceBase = m_faceBase;
            m_trainSetdlg.MdiParent = this;
            m_trainSetdlg.Parent = panel1;
            m_trainSetdlg.m_faceBase = m_faceBase;
            m_trainSetdlg.MaximumSize = size;
            m_trainSetdlg.MinimumSize = size;
            m_trainSetdlg.Show();
            
        }

        private void ShowTrainDlg(string trainPersonName)
        {
            panel1.Visible=false;
            panel2.Visible = true;
            panel2.Controls.Clear();
            m_trainDlg = new TrainDlg(trainPersonName,m_rootPath);
            
            panel2.Location = new Point(this.FRManager.Location.X+this.FRManager.Width,28);
            Size size = new Size(this.Size.Width - panel2.Location.X - 20, this.Size.Height);
            panel2.Size = size;

            m_trainDlg.Size = size;
            m_trainDlg.ControlBox = false;
            m_trainDlg.Text = "";
            FaceBase faceBase = new FaceBase(trainPersonName,m_rootPath);
            m_trainDlg.m_faceBase = faceBase;
            m_trainDlg.MdiParent = this;
            m_trainDlg.Parent = panel2;
            m_trainDlg.MaximumSize = size;
            m_trainDlg.MinimumSize = size;
            m_trainDlg.Show();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            TrainDlg dlg = new TrainDlg("李智博",m_rootPath);
            dlg.m_faceBase = m_faceBase;
            dlg.Show();
        }

        private void 窗口重绘(object sender, PaintEventArgs e)
        {
//             System.Drawing.Drawing2D.GraphicsPath shape = new System.Drawing.Drawing2D.GraphicsPath();
//             shape.AddEllipse(0, 0, this.Height, this.Width);
//             this.Region = new Region(shape);
        }
        private bool flag_LButtonDown = false;
        private void LButtonDown(object sender, MouseEventArgs e)
        {
            flag_LButtonDown = true;
        }

        private void LButtonUp(object sender, MouseEventArgs e)
        {
            if (flag_LButtonDown)
            {
//                 this.panel.Location = PointToScreen(e.Location);
//                 this.FRManager.Location=new Point(this.panel.Location.X+this.panel.Size.Width,this.panel.Location.Y);
//                 flag_LButtonDown = false;
            }
        }


        private Thread thread_song = null;
        private string name_song = "";
        private void PlaySound1()
        {
            DllManager.PlaySound(name_song, 0, 0x0001);
            //PlaySound(null, 0, 0);
            MessageBox.Show("没有播放歌曲吧！");
        }
        private void 监听到数据(object sender, EventArgs e)
        {
            string SRecoResult = SRecoManager.GetSRecoResult();
            if (SRecoResult == "我是谁")
            {

            }
            if (this.listBox_showSRData.FindString(SRecoResult) == -1)
            {
                this.listBox_showSRData.Items.Insert(0, SRecoResult);
            }

            ExecuteSpeechOrder(SRecoResult);
            //             string def="def";
            //             string answer = m_DBManager.GetAnswer(SRecoResult, def);
            //             if (answer!=def)
            //             {
            //                 SRecoManager.Speak(answer);
            //             }  
            //             //唱歌
            //             answer =m_DBManager.GetAnswer("NAME_SONG",SRecoResult,def);
            //             if (answer!=def)
            //             {
            //                 if (thread_song!=null)
            //                 {
            //                     thread_song.Abort();
            //                 }
            //                 else
            //                 {
            //                     //thread_song = new Thread(new ThreadStart(PlaySound1));
            //                     name_song = answer;
            //                     //thread_song.Start();
            //                     PlaySound(name_song, 0, 0x0001);
            //                 }                                
            //             }
            //             else
            //             {
            // //                 if (thread_song!=null)
            // //                 {
            //                     //thread_song.Abort();  
            //                 name_song = null;  
            //                 PlaySound(name_song, 0, 0x0001);
            //                /* }   */             
            //             }
        }

        private void 更新语法库ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ArrayList array_Data = m_DBManager.GetDatas("CONTEXT");
            foreach (string data in array_Data)
            {
                SRecoManager.WriteOrderToXML(m_GrammarPath, data);
            }
            MessageBox.Show("更新语法库成功！");
            //SRecoManager.Test();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 重新加载语法库ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SRecoManager.LoadSRGrammar();
            MessageBox.Show("重新加载语法库成功！");
        }

        private void 显示其他窗口ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_nameOtherWindow =ReadIni("WINDOWNAME", "NameOtherWindow", "");
            if (m_nameOtherWindow!="")
            {
                ShowOtherWindow(m_nameOtherWindow);
            }
            else
            {
                MessageBox.Show("未设置要显示的串口的标题。请在Config.ini文件中进行设置！");
            }
        }

        private void 隐藏其他窗口ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_nameOtherWindow =ReadIni("WINDOWNAME", "NameOtherWindow", "");
            if (m_nameOtherWindow!="")
            {
                HideOtherWindow(m_nameOtherWindow);
            }
            else
            {
                MessageBox.Show("未设置要隐藏的串口的标题。请在Config.ini文件中进行设置！");
            }
        }
        

        private void 接收到数据(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
           
            string data=m_SPManager.接收数据(m_name_errorReturnPort);
            if (data=="wrong")
            {
                MessageBox.Show(data);
                SendMsg msg = new SendMsg(WM_RECEIVEWRONGDATA, "发送串口消息");
                msg.SendMsgToOtherForm();
            }
            //m_SPManager.发送数据("COM3","1/r/n");
        }

        private void 发送串口数据ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SendDataDlg dlg = new SendDataDlg(m_SPManager);
            dlg.ShowDialog();
            //ShowOtherWindow("SendDataDlg",0);
        }

        private void 语音选择发生变化(object sender, EventArgs e)
        {
            string answer = (string)listBox_showSRData.SelectedItem;
            ExecuteSpeechOrder(answer);
        }

        private void ExecuteSpeechOrder(string SRecoResult)
        {
            string def = "def";
            string answer = m_DBManager.GetAnswer(SRecoResult, def);
            if (answer!=def)
            {
                SRecoManager.Speak(answer);
            }  
            //唱歌
            answer =m_DBManager.GetAnswer("NAME_SONG",SRecoResult,def);
            if (answer!=def)
            {
                if (thread_song!=null)
                {
                    thread_song.Abort();
                }
                else
                {
                    //thread_song = new Thread(new ThreadStart(PlaySound1));
                    name_song = answer;
                    //thread_song.Start();
                    DllManager.PlaySound(name_song, 0, 0x0001);
                }                                
            }
            else
            {
//                 if (thread_song!=null)
//                 {
                    //thread_song.Abort();  
                name_song = null;
                DllManager.PlaySound(name_song, 0, 0x0001);
               /* }   */             
            }
        }
        private void 唱首歌ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DllManager.PlaySound(@"D:\人机交互\音乐\范玮琪-一个像夏天 一个像秋天.wav", 0, 1);
        }

        private void FRManager_Enter(object sender, EventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void button_打开发送串口对话框_Click(object sender, EventArgs e)
        {
            SendDataDlg dlg = new SendDataDlg(m_SPManager);
            dlg.ShowDialog();
        }

        private void SRecoManager_SREvent(object sender, EventArgs e)
        {

        }
    }

    public struct COPYDATASTRUCT
    {
        public IntPtr dwData;
        public int cbData;
        [MarshalAs(UnmanagedType.LPStr)]
        public string lpData;
    }
}
