using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Threading;
using System.Timers;
using System.Collections;
using System.Drawing.Drawing2D;

namespace HSRobot
{
    public class PictureBoxLZ:PictureBox
    {
        public bool flag_Clicked = false;
        public bool flag_MouseMove = false;
        public bool flag_Name = false;
        public string m_name;
        public ArrayList nameList=new ArrayList();
        private ImageList m_imageList = new ImageList();      
        
        private void showRect()
        {
            Graphics graphics = this.CreateGraphics();
            Pen myPen;
            Size size = this.Size;

            if (flag_Clicked)
            {
                myPen = new Pen(Color.Blue, 5);
            }
            else if (flag_MouseMove)
            {
                myPen = new Pen(Color.Red, 5);
            }
            else
            {
                myPen = new Pen(Color.Transparent, 50);
            }
            //myPen = new Pen(Color.Blue, 5);
            graphics.DrawRectangle(myPen, 0, 0, size.Width, size.Height);
            /*myPen.Dispose();*/
        }
        public PictureBoxLZ()
        {
            this.Paint +=new PaintEventHandler(this.Redraw); 
        }

        private void Redraw(object sender, PaintEventArgs e)
        {
            //showRect();
             Graphics g = e.Graphics;
//             Graphics newGraphics = this.CreateGraphics();
//             newGraphics.SetClip(new Rectangle(0, 0, 100, 100));
// 
// 
//             e.Graphics.SetClip(newGraphics, CombineMode.Replace);
// 
//             // Fill rectangle to demonstrate clip region.
//             e.Graphics.FillRectangle(new SolidBrush(Color.Black), 0, 0, 500, 300);
// 
//             // Release new graphics.
//             newGraphics.Dispose();

//             int i = 0;
//             foreach (string name in nameList)
//             {
//                 Font drawFont = new Font("Arial", 16);
//                 SolidBrush drawBrush = new SolidBrush(Color.Black);
// 
//                 // Create point for upper-left corner of drawing.
//                 PointF drawPoint = new PointF(100 + 130 * i, 125.0F);
// 
//                 // Draw string to screen.
//                 g.SmoothingMode = SmoothingMode.HighQuality;
//                 g.DrawString(name,new Font("Arial ", 10, FontStyle.Bold), SystemBrushes.ActiveCaptionText, drawPoint);
// 
//                 g.DrawString(name, drawFont, drawBrush, drawPoint);
//                 i++;
//             }
            
        }
        public void Resize()
        {
            m_imageList.Images.Add(this.Image);
            m_imageList.ImageSize = new Size(120,120);
        }

        public void showSelected()
        {
//             showThread = new Thread(new ThreadStart(this.showRect));
//             showThread.Start();
//             timer = new System.Timers.Timer(100000);
//             timer.Elapsed += new ElapsedEventHandler(this.OnTimedEvent);
        }

        private void OnTimedEvent(object source, ElapsedEventArgs e)
        {
//             showThread.Abort();
//             timer.Stop();
        }

    }
}
