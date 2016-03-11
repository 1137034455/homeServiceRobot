using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace HSRobot
{
    /// <summary>
    /// 此类实现对串口功能的统一管理
    /// </summary>
    public class 串口管理
    {
        [DllImport("advapi32.dll", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
        private static extern int RegCloseKey(int hKey);

        [DllImport("advapi32.dll", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true, EntryPoint = "RegOpenKeyA")]
        private static extern int RegOpenKey(uint hKey, string lpSubKey, ref int phkResult);

        [DllImport("advapi32.dll", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true, EntryPoint = "RegEnumValueA")]
        private static extern int RegEnumValue(int hKey, int dwIndex, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpValueName, 
            ref int lpcbValueName, int lpReserved, int lpType, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpData, ref int lpcbData);

        private ArrayList m_listPorts=new ArrayList();

        /// <summary>
        /// 获取当前连接到计算机的所有串口
        /// </summary>
        /// <returns></returns>
        public static string[] GetSerialPortNames()
        {
            #region 方式一：调用系统API（DLL文件：advapi32.dll）读取注册表，并处理读取结果的“字符串结束符”
            string[] ports = null;
            List<string> portsList = new List<string>();
            uint HKEY_LOCAL_MACHINE = 0x80000002;
            int hKey = -1;
            int ret = RegOpenKey(HKEY_LOCAL_MACHINE, @"Hardware\DEVICEMAP\SERIALCOMM", ref hKey);
            try
            {
                if (ret == 0)
                {
                    int index = 0;
                    int BufferSize = 255;
                    int ERROR_NO_MORE_ITEMS = 259;
                    string valueName = "".PadRight(BufferSize, ' ');
                    int valueNameLength = BufferSize;
                    int valueLength = BufferSize;
                    string value = "".PadRight(BufferSize, ' ');
                    while (RegEnumValue(hKey, index, ref valueName, ref valueNameLength, 0, 0, ref value, ref valueLength) != ERROR_NO_MORE_ITEMS)
                    {
                        if (valueLength > 0)
                        {
                            if (value[valueLength - 1] == 0)
                                valueLength -= 1;
                            portsList.Add(value.Substring(0, valueLength));
                        }
                        index += 1;
                        valueName = "".PadRight(BufferSize, ' ');
                        valueNameLength = BufferSize;
                        valueLength = BufferSize;
                    }
                }
            }
            catch (Exception)
            {
            }
            finally
            {
                if (ret == 0)
                    RegCloseKey(hKey);
            }
            if (portsList.Count == 0)
                ports = new string[0];
            else
                ports = portsList.ToArray();
            return ports;
            #endregion

            #region 方式二：C#方式读取注册表后过滤非数字（此方式无法过滤数字，可能造成部分虚拟串口获取错误，例如 COM3 识别为 COM32）
            //HashSet<char> numbers = new HashSet<char>(new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
            //string[] ports = null;

            //RegistryKey localMachine = null;
            //RegistryKey subKey = null;
            //new RegistryPermission(RegistryPermissionAccess.Read, @"HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM").Assert();
            //try
            //{
            //    localMachine = Registry.LocalMachine;
            //    subKey = localMachine.OpenSubKey(@"HARDWARE\DEVICEMAP\SERIALCOMM", false);
            //    if (subKey != null)
            //    {
            //        string[] valueNames = subKey.GetValueNames();
            //        ports = new string[valueNames.Length];
            //        for (int j = 0; j < valueNames.Length; j++)
            //        {
            //            object obj = subKey.GetValue(valueNames[j]);
            //            if (obj == null)
            //                continue;
            //            string str = (string)obj;
            //            if (string.IsNullOrEmpty(str) || str.Length < 4)
            //                continue;

            //            string port = "COM";
            //            char[] cs = str.Substring(3).Replace("\n", "").ToCharArray();
            //            foreach (char c in cs)
            //            {
            //                if (numbers.Contains(c))
            //                    port += c.ToString();
            //                else
            //                    break;
            //            }
            //            ports[j] = port;
            //        }
            //    }
            //}
            //finally
            //{
            //    if (localMachine != null)
            //        localMachine.Close();
            //    if (subKey != null)
            //        subKey.Close();
            //    System.Security.CodeAccessPermission.RevertAssert();
            //}

            //if (ports == null)
            //    ports = new string[0];
            //return ports;
            #endregion
        }

        public bool 批量增加串口(ArrayList 串口列表)
        {
            foreach (串口名及其事件 串口名and事件 in 串口列表)
            {
                try
                {
                    SerialPort 串口 = new SerialPort(串口名and事件.串口名);
                    串口.DataReceived += 串口名and事件.串口事件;
                    m_listPorts.Add(串口);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return false;
                }                
            }
            return true;
        }
        public bool 增加串口(串口名及其事件 nameAndEvent)
        {
            try
            {
                SerialPort 串口 = new SerialPort(nameAndEvent.串口名);
                串口.DataReceived += nameAndEvent.串口事件;
                m_listPorts.Add(串口);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
                return false;
            }
            return true;
        }

        public SerialPort GetPortByName(string namePort)
        {            
            foreach (SerialPort port in m_listPorts)
            {
                if (port.PortName==namePort)
                {
                    return port;
                }
            }
            return null;
        }

        public bool 打开串口(string 串口名)
        {
            foreach (SerialPort 串口 in m_listPorts)
            {
                if (串口名==串口.PortName&&(!串口.IsOpen))
                {
                    try
                    {
                        串口.Open();
                        return true;
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                        return false;
                    }                   
                }
            }
            return false;
        }

        public void 关闭串口(string 串口名)
        {
            foreach (SerialPort 串口 in m_listPorts)
            {
                if (串口名 == 串口.PortName&&串口.IsOpen)
                {
                    try
                    {
                        串口.Close();
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
            }
        }

        public void 发送数据(string 串口名,string 数据)
        {
            foreach (SerialPort 串口 in m_listPorts)
            {
                if (串口名 == 串口.PortName)
                {
                    if (串口.IsOpen)
                    {                       
                        try
                        {
                           // 串口.
                            串口.Write(数据);
                        }
                        catch (System.Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                        }
                    }
                    else
                    {
                        string error = 串口名 + "口尚未打开！";
                        MessageBox.Show(error);
                    }
                }
            }
        }
        public void 发送数据(string 串口名, byte[] 数据,int len)
        {
            foreach (SerialPort 串口 in m_listPorts)
            {
                if (串口名 == 串口.PortName)
                {
                    if (串口.IsOpen)
                    {
                        try
                        {
                            // 串口.
                            串口.Write(数据,0,len);
                        }
                        catch (System.Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                        }
                    }
                    else
                    {
                        string error = 串口名 + "口尚未打开！";
                        MessageBox.Show(error);
                    }
                }
            }
        }
        public string 接收数据(string 串口名)
        {
            string readData="";
            foreach (SerialPort 串口 in m_listPorts)
            {
                if (串口名 == 串口.PortName)
                {
                    if (串口.IsOpen)
                    {
                        try
                        {
                            readData=串口.ReadLine();
                        }
                        catch (System.Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                        }
                    }
                    else
                    {
                        string error = 串口名 + "口尚未打开！";
                        MessageBox.Show(error);
                    }
                }
            }
            return readData;
        }
        public ArrayList GetOpendPorts()
        {
            ArrayList list_opendPorts = new ArrayList();
            foreach(SerialPort port in m_listPorts)
            {
                if (port.IsOpen)
                {
                    list_opendPorts.Add(port);
                }
            }
            return list_opendPorts;
        }
    }
    public class 串口名及其事件
    {
        public string 串口名;
        public SerialDataReceivedEventHandler 串口事件;
    }
}
