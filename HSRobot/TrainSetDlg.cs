using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Threading;
using System.Drawing.Drawing2D;

namespace HSRobot
{
    public partial class TrainSetDlg : Form
    {
        public const int WM_CLOSETRAINSETDLG = 0x400 + 10;
        private const int g_pictureSize_S = 100;
        private const int g_pictureSize_L = 120; 
        public string personName;
        public FaceBase m_faceBase;
        private  ArrayList name_list;
        private ArrayList m_pictureBoxList=new ArrayList();
        private Size g_pictureSize = new Size(g_pictureSize_S,g_pictureSize_S);
        private ArrayListLZ baseList = new ArrayListLZ();
        private ArrayList rectList = new ArrayList(2);
        private Label name_label = new Label();
        private Label h_label = new Label();
        public string selectedPerson;
        private Point m_lastPicboxLocation = new Point();
        private int m_delta = 0;
        public TrainSetDlg()
        {
            InitializeComponent();
        }

        private void TrainSetDlg_Load(object sender, EventArgs e)
        {
            m_faceBase.GetNameList(out name_list);
            FillImageList();
            GetRectImageList();
            SetPicLayout();  
        }

        private void FillImageList()
        {           
            try
            {
                m_imageList.ImageSize = g_pictureSize;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            int i = 0;
            foreach (string name in name_list)
            {
                
                ImageAndPicturebox imaAndPicbox = new ImageAndPicturebox();
                
                string path = m_faceBase.m_rootPath + name;

                path += "\\" + name + "0.bmp";
                Image image = Image.FromFile(path, true);

//                 ImageList imageList = new ImageList();
//                 imageList.ImageSize = g_pictureSize;
//                 imageList.Images.Add(image);             
                m_imageList.Images.Add(image);

                PictureBoxLZ pictureBox = new PictureBoxLZ();
                pictureBox.AutoSize = false;
                pictureBox.Location = new Point(100 + i * 115, 100);
                pictureBox.Size = new Size(100,100);
                pictureBox.Size = g_pictureSize;  
                pictureBox.SizeMode=PictureBoxSizeMode.Zoom;           
                pictureBox.Image=image;
                pictureBox.m_name = name;
                this.Controls.Add(pictureBox);
                pictureBox.Cursor = Cursors.Hand; 
                pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.鼠标滑过图片);
                pictureBox.MouseLeave += new System.EventHandler(this.鼠标离开图片);
                pictureBox.Click += new System.EventHandler(this.鼠标点击图片);
                pictureBox.DoubleClick += new System.EventHandler(this.鼠标双击图像);
                m_pictureBoxList.Add(pictureBox);

                //imaAndPicbox.imageList = imageList;
                imaAndPicbox.pictureBox = pictureBox;
                baseList.Add(imaAndPicbox);
                i++;
            }
            name_label.AutoSize = true;
            name_label.Font = new System.Drawing.Font("微软雅黑", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            name_label.Location = new System.Drawing.Point(240, 61);
            name_label.Size = new System.Drawing.Size(0, 28);
        }
        private void SetPicLayout()
        {
            Size windowSize = this.Size;
            int width = windowSize.Width;
            int height = windowSize.Height;
            int i = -1;
            int j = 0;
            int t_height=0;
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox = imaAndPicbox.pictureBox;
                if (25 + 115 * (i + 1) > width - 120)
                {
                    i = 0;
                    j++;
                    t_height+=150;
                }
                else
                {
                    i++;
                }
                pictureBox.Location = new Point(25+115*i,g_pictureSize_S+/*t_height*/j*150);
                t_height = pictureBox.Location.Y;
                if (pictureBox.flag_Clicked)
                {
                    AddSelectFlag(pictureBox,true);
                }
            }
            
            h_label.Location = new Point(25 , g_pictureSize_S + j*150+150);
            this.Controls.Add(h_label);
        }
        //在每张图片下面添加姓名
        private void SetNameForPic()
        {
            int i = 0;
            int personCount = baseList.Count;
            PictureBoxLZ namePicbox = new PictureBoxLZ();
            namePicbox.Location = new Point(100, 225);
            namePicbox.Size = new Size(115 * personCount, 30);       
            Graphics g = namePicbox.CreateGraphics();
            namePicbox.flag_Name = true;
            namePicbox.nameList = name_list;

            foreach (string name in name_list)
            {
                Font drawFont = new Font("Arial", 16);
                SolidBrush drawBrush = new SolidBrush(Color.Black);

                PointF drawPoint = new PointF(100 + 130 * i, 125.0F);

                g.SmoothingMode = SmoothingMode.HighQuality;
                g.DrawString(name, drawFont, SystemBrushes.ActiveCaptionText, drawPoint);
                i++;
            }
            this.Controls.Add(namePicbox);
        }
        private void GetRectImageList()
        {
            string rectPath110B = m_faceBase.m_rootPath + "方框图\\110B.jpg";
            string rectPath130B = m_faceBase.m_rootPath + "方框图\\130B.jpg";
            string rectPath130R = m_faceBase.m_rootPath + "方框图\\130R.jpg";
            Image rectImage110B = Image.FromFile(rectPath110B);
            Image rectImage130B = Image.FromFile(rectPath130B);
            Image rectImage130R = Image.FromFile(rectPath130R);
            ImageList rectImageList110B = new ImageList();
            ImageList rectImageList130B = new ImageList();
            ImageList rectImageList130R = new ImageList();

            PictureBoxLZ rectPicBox110B = new PictureBoxLZ();
            PictureBoxLZ rectPicBox130B = new PictureBoxLZ();
            PictureBoxLZ rectPicBox130R = new PictureBoxLZ();

            rectImageList110B.ImageSize = rectImage110B.Size;
            rectImageList110B.Images.Add(rectImage110B);
            rectPicBox110B.Image = rectImageList110B.Images[0];
            rectPicBox110B.Size = new Size(110, 30);
            rectList.Add(rectPicBox110B);

            rectImageList130B.ImageSize = rectImage130B.Size;
            rectImageList130B.Images.Add(rectImage130B);
            rectPicBox130B.Image = rectImageList130B.Images[0];
            rectPicBox130B.Size = new Size(130, 30);
            rectList.Add(rectPicBox130B);

            rectImageList130R.ImageSize = rectImage130R.Size;
            rectImageList130R.Images.Add(rectImage130R);
            rectPicBox130R.Image = rectImageList130R.Images[0];
            rectList.Add(rectPicBox130R);
        }
        private void 窗口大小变化(object sender, EventArgs e)
        {                       
            int minWidth=this.label1.Location.X + this.label1.Width+20;
            if (this.Size.Width < minWidth)
            {
                this.Size = new Size((int)minWidth, this.Size.Height);
            }
            
            SetPicLayout();
        }

        private void 子窗口2发生改变(object sender, EventArgs e)
        {
            
        }

        private void 鼠标滑过图片(object sender, MouseEventArgs e)
        {
            int index=0;
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox/* = new PictureBoxLZ()*/;
                pictureBox=imaAndPicbox.pictureBox;
                ImageList imageList/* = new ImageList()*/;
                imageList=imaAndPicbox.imageList;
                if (sender.Equals(pictureBox))
                {
                    Size pictureBoxSize = new Size(g_pictureSize_L, g_pictureSize_L);
                    pictureBox.Invoke(new EventHandler(delegate
                    {
                        
                        if (!pictureBox.flag_Clicked)                        
                        {                            
                            if (!pictureBox.flag_MouseMove)
                            {
                                pictureBox.Left -= 10;
                                pictureBox.Size = new Size(g_pictureSize_L, g_pictureSize_L);
                                pictureBoxSize = pictureBox.Size;
                                pictureBox.flag_MouseMove = true;
                                this.Controls.Add(pictureBox);

                                AddNameLable(pictureBox);

                                pictureBox.MouseMove -= new System.Windows.Forms.MouseEventHandler(this.鼠标滑过图片);
                            }                            
                        }                                               
                    }));                                                            
                }
                index++;
            }

        }

        private void AddNameLable(PictureBoxLZ pictureBox)
        {
            Graphics graphics = CreateGraphics();
            SizeF sizeF = graphics.MeasureString(pictureBox.m_name, name_label.Font);
            graphics.Dispose();
            Point point = pictureBox.Location;
            Size size = pictureBox.Size;
            name_label.Location = new Point((int)(point.X + 0.5 * (size.Width - (int)sizeF.Width)), point.Y + 123);
            this.name_label.Text = pictureBox.m_name;
            this.Controls.Add(name_label);
        }

        private void 鼠标离开图片(object sender, EventArgs e)
        {
            int index = 0;
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox/* = new PictureBoxLZ()*/;
                pictureBox = imaAndPicbox.pictureBox;
                ImageList imageList /*= new ImageList()*/;
                imageList = imaAndPicbox.imageList;

                if (sender.Equals(pictureBox))
                {
                    pictureBox.Invoke(new EventHandler(delegate
                    {
                        pictureBox.flag_MouseMove = false;
                        
                        if (pictureBox.flag_Clicked)
                        {                           
                            if (!pictureBox.flag_MouseMove)
                            {
                                pictureBox.Left += 10;
                                pictureBox.Size = new Size(g_pictureSize_S, g_pictureSize_S);
                            }

                            pictureBox.Invoke(new EventHandler(delegate
                            {
                                Point point = pictureBox.Location;
                                PictureBoxLZ rectPic = (PictureBoxLZ)(rectList[0]);                                
                                rectPic.Location = new Point(point.X-5 , point.Y+120);
                                this.Controls.Remove((PictureBoxLZ)rectList[1]);
                                this.Controls.Add(rectPic);
                               
                            }));

                            pictureBox.MouseLeave -= new EventHandler(this.鼠标离开图片);


                        }
                        else
                        {
                            if (!pictureBox.flag_MouseMove)
                            {
                                pictureBox.Left += 10;
                                pictureBox.Size = new Size(g_pictureSize_S, g_pictureSize_S);
                                this.Controls.Add(pictureBox);
                            }

                            pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.鼠标滑过图片);
                        }

                    }));
                    this.Controls.Remove(name_label);
                    
                }
                index++;
            }
        }

        private void 鼠标点击图片(object sender, EventArgs e)
        {
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox/* = new PictureBoxLZ()*/;
                pictureBox = imaAndPicbox.pictureBox;
                ImageList imageList /*= new ImageList()*/;
                imageList = imaAndPicbox.imageList;
                
                if (pictureBox.flag_Clicked)
                {
                    pictureBox.Invalidate();
                    pictureBox.flag_Clicked = false;
                    pictureBox.MouseMove += new MouseEventHandler(this.鼠标滑过图片);
                    pictureBox.MouseLeave += new EventHandler(this.鼠标离开图片);
                    pictureBox.Click += new System.EventHandler(this.鼠标点击图片);
                    this.Controls.Remove((PictureBoxLZ)rectList[0]);
                }
                if (sender.Equals(pictureBox))
                {

                    this.label_selectName.Text = pictureBox.m_name;

                    pictureBox.Invoke(new EventHandler(delegate
                    {
                        AddSelectFlag(pictureBox);

                        pictureBox.MouseMove-= new MouseEventHandler(this.鼠标滑过图片);
                        pictureBox.Click -= new System.EventHandler(this.鼠标点击图片);
                    }));                                                                         
                }                
            }
        }

        private void AddSelectFlag(PictureBoxLZ pictureBox,bool formChanged=false)
        {
            Point point = pictureBox.Location;
            Size size = pictureBox.Size;

            PictureBoxLZ rectPic;
            if (!formChanged)
            {
                rectPic = (PictureBoxLZ)(rectList[1]);
            }
            else
            {
                rectPic = (PictureBoxLZ)(rectList[0]);
            }

            rectPic.Location = new Point(point.X - 5, point.Y + 120);
            this.Controls.Add(rectPic);

            pictureBox.flag_Clicked = true;
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button_Ok_Click(object sender, EventArgs e)
        {
            selectedPerson = this.label_selectName.Text;
            //MessageBox.Show("ok");
        }

        private void button_Cancle_Click(object sender, EventArgs e)
        {
            selectedPerson = "";
            //MessageBox.Show("cancle");
        }

        private void 鼠标双击图像(object sender, EventArgs e)
        {
            MessageBoxButtons buttons = MessageBoxButtons.YesNo;
            DialogResult result;
           
//             result = MessageBox.Show("您确定选择此好友进行训练吗？", "好友训练", buttons);
//             if(result == System.Windows.Forms.DialogResult.Yes)
//             {
                selectedPerson = this.label_selectName.Text;
                SendMsg msg = new SendMsg(WM_CLOSETRAINSETDLG, MainForm.m_formText);
                msg.SendStringToOtherForm(selectedPerson);
                this.Close();
            /*}*/
        }

        private void AddOnePicbox(string newPersonName)
        {
            string newPersonPicPath = m_faceBase.m_rootPath + newPersonName + "\\" + newPersonName + "0.bmp";
            Image image = Image.FromFile(newPersonPicPath);
            m_imageList.Images.Add(image);

            PictureBoxLZ pictureBox = new PictureBoxLZ();
            pictureBox.Size = new Size(100, 100);
            pictureBox.m_name = newPersonName;
            Size windowSize = this.Size;
            int width = windowSize.Width;
            int height = windowSize.Height;
            pictureBox.Image = image;
            if (m_lastPicboxLocation.X > width - 235)
            {
                pictureBox.Location = new Point(25, m_lastPicboxLocation.Y + 150 - m_delta);
            }
            else
            {
                pictureBox.Location = new Point(m_lastPicboxLocation.X + 115, m_lastPicboxLocation.Y - m_delta);
            }
            m_lastPicboxLocation = new Point(pictureBox.Location.X, pictureBox.Location.Y + m_delta);
            pictureBox.Cursor = Cursors.Hand;
            pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.鼠标滑过图片);
            pictureBox.MouseLeave += new EventHandler(this.鼠标离开图片);
            pictureBox.Click += new System.EventHandler(this.鼠标点击图片);
            pictureBox.DoubleClick += new System.EventHandler(this.鼠标双击图像);
            m_pictureBoxList.Add(pictureBox);

            h_label.Location = new Point(25, pictureBox.Location.Y + 150);

            this.Controls.Add(pictureBox);

            ImageAndPicturebox imaAndPicbox = new ImageAndPicturebox();
            //imaAndPicbox.imageList = imageList;
            imaAndPicbox.pictureBox = pictureBox;
            baseList.Add(imaAndPicbox);
        }

        public void AddNewPicbox()
        {
            string newPersonName = m_faceBase.GetNewPersonName();
            //string newPicPath = newPersonName;
            AddOnePicbox(newPersonName);
        }

        public void RemovePerson(string removePerson)
        {
            int index_remove=name_list.IndexOf(removePerson);
            //ImageAndPicturebox imaAndPicbox=(ImageAndPicturebox)baseList.GetObject(index_remove);
            this.Controls.RemoveAt(index_remove);
            baseList.RemoveAt(index_remove);           
        }
    }
}
