using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;
using System.Drawing;

namespace HSRobot
{
    public class FaceBase
    {
        //public string personName;
        private ArrayList m_nameList=new ArrayList();
        private ArrayList m_trainPersonImages = new ArrayList();
        private ArrayList m_trainPersonImagesInfo = new ArrayList();
        public string m_rootPath;
        private string m_trainPersonName;
        public FaceBase()
        {
            InitFaceBase();
            GetNameList();            
        }

        public FaceBase(string rootPath,bool flag_rootPath=true)
        {
            if (flag_rootPath==true)
            {
                InitFaceBase(rootPath);
                GetNameList();
            }
        }

        public FaceBase(string trainPerson,string rootPath)
        {
            InitFaceBase(rootPath);
            m_trainPersonName = trainPerson;
            GetTrainPersonImages(m_trainPersonName);
        }
        public string GetNewPicName()
        {
            GetTrainPersonImages(m_trainPersonName);
            string newPicName=null;
            foreach (string picName in m_trainPersonImagesInfo)
            {
                newPicName=picName;
            }
            return newPicName;
        }
        public string GetNewPersonName()
        {
            GetNameList();
            string newPersonName = null;
            foreach (string personName in m_nameList)
            {
                newPersonName = personName;
            }
            return newPersonName;
        }
        private void InitFaceBase()
        {
            m_rootPath = AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
        }

        private void InitFaceBase(string rootPath)
        {
            if (rootPath[rootPath.Length-1]!='\\')
            {
                rootPath+='\\';
            }
            m_rootPath = rootPath;
        }
        private void GetNameList()
        {
            string strFile = m_rootPath + "Record.txt";
            try
            {
                if (File.Exists(strFile))
                {
                    using (StreamReader sr = new StreamReader(strFile,Encoding.GetEncoding("gb2312")))
                    {
                        ArrayList str_list = new ArrayList();
                        while (sr.Peek() >= 0)
                        {
                            string str = sr.ReadLine();
                            str_list.Add(str);                            
                        }
                        int count=str_list.Count;
                        m_nameList = str_list.GetRange(3, count - 3);
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void GetTrainPersonImages(string trainPersonName)
        {
            string strFile = m_rootPath + trainPersonName+"\\info.txt";
            try
            {
                if (File.Exists(strFile))
                {
                    using (StreamReader sr = new StreamReader(strFile, Encoding.GetEncoding("gb2312")))
                    {
                        ArrayList str_list = new ArrayList();
                        while (sr.Peek() >= 0)
                        {
                            string str = sr.ReadLine();
                            str = m_rootPath + trainPersonName + "\\"+str;
                            str_list.Add(str);
                        }
                        int count = str_list.Count;
                        m_trainPersonImagesInfo = str_list.GetRange(3, count - 3);
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public void GetNameList(out ArrayList nameList)
        {
            nameList = m_nameList;
        }

        public void GetTrainPersonImagesInfo(out ArrayList trainPersonImagesInfo)
        {
//             foreach (string trainPersonImageInfo in m_trainPersonImagesInfo)
//             {
//                 Image image = Image.FromFile(trainPersonImageInfo);
//                 ImageList imageList = new ImageList();
//                 imageList.Images.Add(image);
//                 trainPersonImages.Add(imageList);
//             }
            trainPersonImagesInfo = m_trainPersonImagesInfo;
            //trainPersonImages = m_trainPersonImages;
        }
    }
}
