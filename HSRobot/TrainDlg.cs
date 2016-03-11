using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace HSRobot
{
    public partial class TrainDlg : Form
    {
        public  const int WM_CLOSETRAINDLG=0x400+2;

        private string m_name;
        public FaceBase m_faceBase;
        private ArrayList m_personImagesInfo;
        private ArrayList m_pictureBoxList = new ArrayList();
        public ArrayList baseList = new ArrayList();
        public ArrayList old_baseList = new ArrayList();
        private const int g_pictureSize_S = 100;
        private const int g_pictureSize_L = 120; 
        private Size g_pictureSize = new Size(g_pictureSize_S, g_pictureSize_S);
        private ArrayList rectList = new ArrayList(2);
        private Label h_label = new Label();
        private Label name_label = new Label();
        private Point m_lastPicboxLocation = new Point();
        private int m_delta=0;
        public string m_rootPath="";
        public TrainDlg()
        {
            InitializeComponent();
        }

        public TrainDlg(string trainPersonName,string rootPath)
        {
            InitializeComponent();
            m_name = trainPersonName;
            m_rootPath = rootPath;
        }
        private void TrainDlg_Load(object sender, EventArgs e)
        {
            m_faceBase =new FaceBase(m_name,m_rootPath);
            m_faceBase.GetTrainPersonImagesInfo(out m_personImagesInfo);
            FillImageList();
            GetRectImageList();
            SetPicLayout();
        }

        public void ReLoad(string trainPersonName)
        {
            m_name = trainPersonName;
            m_faceBase = new FaceBase(m_name);
            m_faceBase.GetTrainPersonImagesInfo(out m_personImagesInfo);
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

            foreach (string trainPersonImageInfo in m_personImagesInfo)
            {

                ImageAndPicturebox imaAndPicbox = new ImageAndPicturebox();

                string path =trainPersonImageInfo;

                
                Image image = Image.FromFile(path);

//                 ImageList imageList = new ImageList();
//                 imageList.ImageSize = g_pictureSize;
//                 imageList.Images.Add(image);
                m_imageList.Images.Add(image);

                PictureBoxLZ pictureBox = new PictureBoxLZ();
                pictureBox.AutoSize = false;
                pictureBox.SizeMode = PictureBoxSizeMode.Zoom;
                pictureBox.Image = image;
                pictureBox.m_name = trainPersonImageInfo.Substring(trainPersonImageInfo.LastIndexOf("\\")+1);
                
                pictureBox.Cursor = Cursors.Hand;
                pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.鼠标滑过图片);
                pictureBox.MouseLeave += new EventHandler(this.鼠标离开图片);
                pictureBox.Click += new System.EventHandler(this.鼠标点击图片);
                pictureBox.DoubleClick += new System.EventHandler(this.鼠标双击图像);
                m_pictureBoxList.Add(pictureBox);

                //imaAndPicbox.imageList = imageList;
                imaAndPicbox.pictureBox = pictureBox;
                baseList.Add(imaAndPicbox);
                old_baseList.Add(imaAndPicbox);
                i++;
            }
            //old_baseList = baseList;
            name_label.AutoSize = true;
            name_label.Font = new System.Drawing.Font("微软雅黑", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            name_label.Location = new System.Drawing.Point(240, 61);
            name_label.Size = new System.Drawing.Size(0, 28);
            this.label_name.Text = m_name;
        }
        public void SetPicLayout()
        {
            Size windowSize = this.Size;
            int width = windowSize.Width;
            int height = windowSize.Height;
            int i = -1;
            int j = 0;
            int t_height = 0;
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox = imaAndPicbox.pictureBox;
                pictureBox.Size = g_pictureSize;
                if (25 + 115 * (i + 1) > width - 120)
                {
                    i = 0;
                    j++;
                    t_height += 150;
                }
                else
                {
                    i++;
                }
                pictureBox.Location = new Point(25 + 115 * i, g_pictureSize_S +/*t_height*/j * 150);
                t_height = pictureBox.Location.Y;
                m_lastPicboxLocation = pictureBox.Location;
                if (pictureBox.flag_Clicked)
                {
                    AddSelectFlag(pictureBox, true);
                }
                this.Controls.Add(pictureBox);
            }

            h_label.Location = new Point(25, g_pictureSize_S + j * 150 + 150);
            this.Controls.Add(h_label);
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

        private void AddSelectFlag(PictureBoxLZ pictureBox, bool formChanged = false)
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

        private void 鼠标滑过图片(object sender, MouseEventArgs e)
        {
            int index = 0;
            foreach (ImageAndPicturebox imaAndPicbox in baseList)
            {
                PictureBoxLZ pictureBox/* = new PictureBoxLZ()*/;
                pictureBox = imaAndPicbox.pictureBox;
                ImageList imageList/* = new ImageList()*/;
                imageList = imaAndPicbox.imageList;
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
                                rectPic.Location = new Point(point.X - 5, point.Y + 120);
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

        private void 鼠标点击图片(object sender, EventArgs e)
        {

        }

        private void 鼠标双击图像(object sender, EventArgs e)
        {

        }

        private void button_back_Click(object sender, EventArgs e)
        {
            //SendMessage
            SendMsg msg = new SendMsg(WM_CLOSETRAINDLG,MainForm.m_formText);
            msg.SendMsgToOtherForm();
        }

        private void 窗口大小变化(object sender, EventArgs e)
        {
            SetPicLayout();
        }

        private void AddOnePicbox(string newPicPath)
        {
            if (newPicPath==null&&newPicPath=="")
            {
                return;
            }
            Image image = Image.FromFile(newPicPath);
            m_imageList.Images.Add(image);
            PictureBoxLZ pictureBox = new PictureBoxLZ();
            pictureBox.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox.Size = new Size(100, 100);
            pictureBox.m_name = newPicPath.Substring(newPicPath.LastIndexOf("\\") + 1);
            Size windowSize = this.Size;
            int width = windowSize.Width;
            int height = windowSize.Height;
            pictureBox.Image = image;
            if (m_lastPicboxLocation.Y < 100)
            {
                m_lastPicboxLocation = new Point(-90,100);
            }
            if (m_lastPicboxLocation.X > width - 235)
            {
                pictureBox.Location = new Point(25, m_lastPicboxLocation.Y + 150-m_delta);
            }
            else
            {
                pictureBox.Location = new Point(m_lastPicboxLocation.X + 115, m_lastPicboxLocation.Y-m_delta);
            }
            m_lastPicboxLocation = new Point(pictureBox.Location.X,pictureBox.Location.Y+m_delta);
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
            string newPicName=m_faceBase.GetNewPicName();
            string newPicPath =newPicName;
            AddOnePicbox(newPicPath);
        }

       

        private void 滚动条被拖动时(object sender, ScrollEventArgs e)
        {                        
            m_delta=e.NewValue;           
        }

    }
}
