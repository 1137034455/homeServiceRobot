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
using SpeechRecognition;
using System.IO;
using System.Collections;
using System.Threading;

namespace HSRobot
{
    public partial class Form1 : Form
    {
        private DBManager m_DBManager;
        private const int WM_SR2HSRobot=0x1234;
        private const int WM_SETTEXT = 0x000C;
        private const int WM_COPYDATA = 0x004A;
        public const int WM_RECEIVEWRONGDATA=0x297A;
        [DllImport("语音识别DLL.dll", EntryPoint = "InitSR")]
        public static extern void InitSR();

        [DllImport("kernel32", EntryPoint = "CopyMemory")]
        public static extern void CopyMemory(
                StringBuilder Destination,
                IntPtr Source,
                int Length
        );
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);  
//         [DllImport("kernel32.dll", EntryPoint = "GetPrivateProfileString")]
//         private static extern int GetPrivateProfileString(
//             string ApplicationName, string KeyName, string DefaultString,
//             [MarshalAs(UnmanagedType.LPArray)] byte[] lpBuffer, int nSize, string FileName);
        [DllImport("winmm.dll", EntryPoint = "PlaySound")]
        public static extern int PlaySound(
            string lpszName,
            int hModule,
            int dwFlags
        );

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
        public Form1()
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
        /************************************************************************/
        /* 初始化数据库                                                                     */
        /************************************************************************/        
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
        /************************************************************************/
        /* 窗口初始化                                                                     */
        /************************************************************************/
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
        //读取INI
        public string ReadIni(string Section, string Ident, string Default)
        {
//             Byte[] Buffer = new Byte[65535];
//             int bufLen = GetPrivateProfileString(Section, Ident, Default, Buffer, Buffer.GetUpperBound(0), m_iniFileName);
//             string s = Encoding.GetEncoding(0).GetString(Buffer);
//             s = s.Substring(0, bufLen);
//             return s.Trim();
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section,Ident, Default,temp, 255, m_iniFileName);
            return temp.ToString();
//             int size = 260;
//             byte[] buff = new byte[size];
//             int ret = GetPrivateProfileString(Section, Ident, Default, buff, size, m_iniFileName);
//             if (ret < 0)
//                 return null;
//             byte[] arry = new byte[ret];
//             for (int i = 0; i < ret; i++)
//             {
//                 arry[i] = buff[i];
//             }
//             return System.Text.Encoding.Default.GetString(arry);
        }

        private void button1_Click(object sender, EventArgs e)
        {
//             TrainDlg dlg = new TrainDlg("李智博");
//             dlg.ShowDialog();
            ShowTrainDlg("李智博");
        }

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
//                 case WM_SETTEXT:
//                         {
//                             string str = Marshal.PtrToStringAnsi(msg.LParam);
//                             //MessageBox.Show(str);
//                         }
//                         break;
                case WM_SR2HSRobot:
                        {
                            this.listBox_showSRData.Items.Add(GetSRResult());
                            this.statusShow.Text = GetSRResult();
                            //MessageBox.Show(GetSRResult());
                        }
                        break;
//                 case WM_COPYDATA:
//                         {
//                             COPYDATASTRUCT mystr = new COPYDATASTRUCT();
//                             Type mytype = mystr.GetType();
//                             mystr = (COPYDATASTRUCT)msg.GetLParam(mytype);
//                             
//                             MessageBox.Show(mystr.lpData);
//                         }
//                         break;
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
//             ImageAndPicturebox t=new ImageAndPicturebox();
//             ImageAndPicturebox tt=new ImageAndPicturebox();
//             foreach (ImageAndPicturebox imaAndPicbox in m_trainDlg.baseList)
//             {
//                 t=imaAndPicbox;
//             }
//             Point point=t.pictureBox.Location;
//             Size size=t.pictureBox.Size;
//             tt.pictureBox.Location=new Point();

//             PictureBoxLZ pictureBox = new PictureBoxLZ();
//             ImageList imageList = new ImageList();
//             string imagePath = m_faceBase.rootPath + m_selectedPersonName + "\\"+m_selectedPersonName+"10.bmp";
//             Image image = Image.FromFile(imagePath);
//             imageList.Images.Add(image);
//             pictureBox.Image = image;
//             m_trainDlg.m_imageList.Images.Add(image);
// 
//             ImageAndPicturebox imgAndPicbox = new ImageAndPicturebox();
//             imgAndPicbox.pictureBox = pictureBox;
//             imgAndPicbox.imageList = imageList;
//             m_trainDlg.baseList.Add(imgAndPicbox);
//             m_trainDlg.Controls.Add(pictureBox);
//             m_trainDlg.SetPicLayout();

//             m_faceBase = new FaceBase(m_selectedPersonName);
//             m_trainDlg.m_faceBase.GetTrainPersonImagesInfo(out m_personImagesInfo);
//             FillImageList();
//             GetRectImageList();
//             SetPicLayout();
            /*ShowTrainDlg(m_selectedPersonName);*/
            //m_trainDlg.ReLoad(m_selectedPersonName);
            m_trainDlg.AddNewPicbox();
        }
        private void AddPersonPictureToTrainSetDlg()
        {
            m_trainSetdlg.AddNewPicbox();
        }
        private void button2_Click(object sender, EventArgs e)
        {
            ShowTrainSetDlg();
            OpenText();
            ControlCalculator();
        }
#region OtherStudy

        [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
        private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", EntryPoint = "FindWindowEx", SetLastError = true)]
        private static extern IntPtr FindWindowEx(IntPtr hwndParent, uint hwndChildAfter, string lpszClass, string lpszWindow);


        [DllImport("user32.dll", EntryPoint = "SetWindowPos")]
        public static extern int SetWindowPos(
            IntPtr hwnd,
            int hWndInsertAfter,
            int x,
            int y,
            int cx,
            int cy,
            int wFlags
        );

        [DllImport("user32.dll", EntryPoint = "GetWindowRect")]
        public static extern int GetWindowRect(
            IntPtr hwnd,
            ref Rectangle lpRect
        );
       

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool ShowWindow(IntPtr hWnd, uint nCmdShow);  

        [DllImport("user32.dll", EntryPoint = "SendMessage", SetLastError = true, CharSet = CharSet.Auto)]
        private static extern int SendMessage(IntPtr hwnd, uint wMsg, int wParam, int lParam);

        [DllImport("user32.dll", EntryPoint = "SetForegroundWindow", SetLastError = true)]
        private static extern void SetForegroundWindow(IntPtr hwnd);

        private void ControlCalculator()
        {
            const uint BM_CLICK = 0xF5; //鼠标点击的消息，对于各种消息的数值，大家还是得去API手册

            IntPtr hwndCalc = FindWindow(null, "计算器"); //查找计算器的句柄

            if (hwndCalc != IntPtr.Zero)
            {
                IntPtr hwndThree = FindWindowEx(hwndCalc, 0, null, "3"); //获取按钮3 的句柄

                IntPtr hwndPlus = FindWindowEx(hwndCalc, 0, null, "+");  //获取按钮 + 的句柄
                IntPtr hwndTwo = FindWindowEx(hwndCalc, 0, null, "2");  //获取按钮2 的句柄
                IntPtr hwndEqual = FindWindowEx(hwndCalc, 0, null, "="); //获取按钮= 的句柄
                SetForegroundWindow(hwndCalc);    //将计算器设为当前活动窗口
                System.Threading.Thread.Sleep(2000);   //暂停2秒让你看到效果
                SendMessage(hwndThree, BM_CLICK, 0, 0);
                System.Threading.Thread.Sleep(2000);   //暂停2秒让你看到效果
                SendMessage(hwndPlus, BM_CLICK, 0, 0);
                System.Threading.Thread.Sleep(2000);   //暂停2秒让你看到效果
                SendMessage(hwndTwo, BM_CLICK, 0, 0);
                System.Threading.Thread.Sleep(2000);   //暂停2秒让你看到效果
                SendMessage(hwndEqual, BM_CLICK, 0, 0);

                System.Threading.Thread.Sleep(2000);
                MessageBox.Show("你看到结果了吗？");
            }
            else
            {
                MessageBox.Show("没有启动 [计算器]");
            }
        }
        
        private void ShowOtherWindow(string name_otherWindow,int flag_fullScreen=1)
        {
            IntPtr hWnd=FindWindow(null, name_otherWindow);
            if (hWnd != IntPtr.Zero)
            {
                
//                 Rectangle rect=new Rectangle();
//                 GetWindowRect(hWnd, ref rect);
                Rectangle rect=new Rectangle(new Point(0,0),new Size(0,0));
                if (flag_fullScreen==1)
                {
                    rect = System.Windows.Forms.Screen.PrimaryScreen.WorkingArea;
                }
                else
                {
                    GetWindowRect(hWnd,ref rect);
                }
                SetWindowPos(hWnd, -1, 0, 0, rect.Width, rect.Height, 0x4000 | 0x0040);
                ShowWindow(hWnd, 1);
            }
        }

        private void HideOtherWindow(string name_otherWindow)
        {
            IntPtr hWnd = FindWindow(null, name_otherWindow);
            if (hWnd != IntPtr.Zero)
            {
                ShowWindow(hWnd, 0);
            }
        }

        private void OpenText()
        {
            //启动notepad.exe 记事本程序，并在d:\下创建 或 打开 text_test.txt文件
            System.Diagnostics.Process txt = System.Diagnostics.Process.Start(@"SRManager.exe");
            txt.StartInfo.WindowStyle = ProcessWindowStyle.Normal;

            //等待一秒，以便目标程序notepad.exe输入状态就绪
            txt.WaitForInputIdle(200);

            /*IntPtr hwndCalc = FindWindow("SRManager", null); //查找计算器的句柄*/
            IntPtr hwndCalc = txt.Handle;
            const uint BM_CLICK = 0xF5; //鼠标点击的消息，对于各种消息的数值，大家还是得去API手册
            if (hwndCalc != IntPtr.Zero)
            {
                IntPtr hwndThree = FindWindowEx(hwndCalc, 0, "CButton", null); //获取按钮3 的句柄

                IntPtr hwndPlus = FindWindowEx(hwndCalc, 0, null, "+");  //获取按钮 + 的句柄
//                 IntPtr hwndTwo = FindWindowEx(hwndCalc, 0, null, "2");  //获取按钮2 的句柄
//                 IntPtr hwndEqual = FindWindowEx(hwndCalc, 0, null, "="); //获取按钮= 的句柄
                SetForegroundWindow(hwndCalc);    //将计算器设为当前活动窗口
                /*System.Threading.Thread.Sleep(2000);   //暂停2秒让你看到效果*/
                /*SendMessage(hwndThree, BM_CLICK, 0, 0);*/
            }
            //如果目标程序 notepad.exe 没有停止响应，则继续
            if (txt.Responding)
            {
                //开始写入内容
                SendKeys.SendWait("-----下面的内容是外部程序自动写入-----\r\n");

                //SendKeys.SendWait(this.textBox1.Text);     //将文本框内的内容写入

                SendKeys.SendWait("{Enter}{Enter}");     //写入2个回车
                SendKeys.SendWait("文档创建时间：");
                //SendKeys.SendWait("{F5}");          //发送F5按键
                SendKeys.SendWait("{Enter}");       //发送回车键
                SendKeys.SendWait("^s");       //发送 Ctrl + s 键
                //SendKeys.SendWait("%{F4}");      // 发送 Alt + F4 键

               // MessageBox.Show("文件已经保存成功！");
            }
        }
#endregion
        private void ShowTrainSetDlg()
        {
            //panel2.SendToBack();
            panel2.Visible = false;
            panel1.Visible = true;
            panel1.Controls.Clear();
//             if (m_trainSetdlg==null)
//             {
//                 m_trainSetdlg = new TrainSetDlg();
//             }
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

        private void 开启语音识别ToolStripMenuItem_Click(object sender, EventArgs e)
        {
//             InitSR();
//             string[] nameList=new string[]{"李智博","孙旭让","刘尊","王雪彬","李亚楠","潘世豪","黄中意"};
//             SRecognition SR = new SRecognition(nameList);
//             //SR.SpeechRecoInit(nameList);
//             SR.BeginRec(this.textBox1);
            OpenText();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            OpenText();
        }
        private Thread thread_song;
        private string name_song = "";
        private void PlaySound1()
        {
            PlaySound(name_song, 0, 0x0001);
            //PlaySound(null, 0, 0);
            MessageBox.Show("没有播放歌曲吧！");
        }
        private void 监听到数据(object sender, EventArgs e)
        {
            string SRecoResult=SRecoManager.GetSRecoResult();
            if (SRecoResult=="我是谁")
            {

            }
            if (this.listBox_showSRData.FindString(SRecoResult)==-1)
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
                    PlaySound(name_song, 0, 0x0001);
                }                                
            }
            else
            {
//                 if (thread_song!=null)
//                 {
                    //thread_song.Abort();  
                name_song = null;  
                PlaySound(name_song, 0, 0x0001);
               /* }   */             
            }
        }
        private void 唱首歌ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PlaySound(@"D:\人机交互\音乐\范玮琪-一个像夏天 一个像秋天.wav", 0, 1);
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

    }

    public struct COPYDATASTRUCT
    {
        public IntPtr dwData;
        public int cbData;
        [MarshalAs(UnmanagedType.LPStr)]
        public string lpData;
    }
}
