using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HSRobot
{
    public class FileUtil
    {
        //读取INI
        public static string ReadIni(string iniFileName, string Section, string Ident, string Default)
        {
            StringBuilder temp = new StringBuilder(255);
            int i = DllManager.GetPrivateProfileString(Section, Ident, Default, temp, 255, iniFileName);
            return temp.ToString();
        }
    }
}
