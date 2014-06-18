using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace HSRobot
{
    public class ArrayListLZ: ArrayList
    {
        public object GetObject(int index)
        {
            object t_obj = new object();
            int i = 0;
            foreach (object obj in this)
            {
                t_obj = obj;
                if (i==index)
                {
                    break;
                }
                i++;
            }
            return t_obj;
        }
    }
}
