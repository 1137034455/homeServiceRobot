using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Messaging;

namespace HSRobot
{
    /// <summary>
    /// 用于向指定的窗口发送信号
    /// </summary>
    public class SendMsg
    {
        //声明 API 函数 
        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessage(int hWnd, int msg, IntPtr wParam, IntPtr lParam);

        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        private static extern int FindWindow(string lpClassName, string lpWindowName);

        //定义消息常数 
        //public const int CUSTOM_MESSAGE = 0X400 + 2;//自定义消息

        private string m_windowName;
        private int m_Msg;        
        public SendMsg(int Msg,string windowName)
        {
            m_Msg = Msg;
            m_windowName = windowName;
        }
        //向窗体发送消息的函数 
        public void SendMsgToOtherForm()
        {
            int WINDOW_HANDLER = FindWindow(null, m_windowName);
            if (WINDOW_HANDLER == 0)
            {
                throw new Exception("Could not find Main window!");
            }

            SendMessage(WINDOW_HANDLER, m_Msg, IntPtr.Zero, IntPtr.Zero);

        }
        //向指定窗体发送数据
        public void SendStringToOtherForm(string info)
        {
            int WINDOW_HANDLER = FindWindow(null, m_windowName);
            if (WINDOW_HANDLER == 0)
            {
                throw new Exception("Could not find Main window!");
            }

            SendMessage(WINDOW_HANDLER, m_Msg, Marshal.StringToHGlobalAnsi(info), IntPtr.Zero);
        }
    } 
}
