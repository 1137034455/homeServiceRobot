using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;

namespace HSRobot
{
    /// <summary>
    /// 用于管理各种Win32 API的类
    /// </summary>
    public class DllManager
    {
        [DllImport("语音识别DLL.dll", EntryPoint = "InitSR")]
        public static extern void InitSR();

        [DllImport("kernel32", EntryPoint = "CopyMemory")]
        public static extern void CopyMemory(
                StringBuilder Destination,
                IntPtr Source,
                int Length
        );

        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        [DllImport("winmm.dll", EntryPoint = "PlaySound")]
        public static extern int PlaySound(
            string lpszName,
            int hModule,
            int dwFlags
        );

        #region 窗口相关
        [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", EntryPoint = "FindWindowEx", SetLastError = true)]
        public static extern IntPtr FindWindowEx(IntPtr hwndParent, uint hwndChildAfter, string lpszClass, string lpszWindow);


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
        public static extern bool ShowWindow(IntPtr hWnd, uint nCmdShow);

        [DllImport("user32.dll", EntryPoint = "SendMessage", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern int SendMessage(IntPtr hwnd, uint wMsg, int wParam, int lParam);

        [DllImport("user32.dll", EntryPoint = "SetForegroundWindow", SetLastError = true)]
        public static extern void SetForegroundWindow(IntPtr hwnd);

        #endregion 窗口相关
    }
}