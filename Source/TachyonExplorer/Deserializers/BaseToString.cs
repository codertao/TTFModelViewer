using System;
using System.Collections;
using System.Reflection;
using System.Text;

namespace TachyonExplorer.Deserializers
{
    public class IgnoreForPrintingAttribute : Attribute
    {
        
    }

    public class BaseToString
    {
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            foreach (PropertyInfo property in GetType().GetProperties())
            {
                if (property.GetCustomAttributes(typeof (IgnoreForPrintingAttribute), false).Length>0)
                    continue;
                sb.Append(Pad(property.Name + ": ", 20));
                object value = property.GetValue(this, null);
                AppendValue(sb, property.PropertyType, value);
                sb.AppendLine();
            }
            return sb.ToString();
        }

        private void AppendValue(StringBuilder sb, Type type, object value)
        {
            if (value == null)
                sb.Append("<null>");
            else if (value is Int32)
                sb.Append("0x" + ((Int32)value).ToString("X8"));
            else if (value is Int16)
                sb.Append("0x" + ((Int16)value).ToString("X4"));
            else if (value is UInt32)
                sb.Append("0x" + ((UInt32)value).ToString("X8"));
            else if (value is UInt16)
                sb.Append("0x" + ((UInt16)value).ToString("X4"));
            else if (type.Name.Contains("PAK"))
                    sb.Append("\r\n"+value);
            else if (value is IEnumerable && !((value is string)||(value is byte[])))
            {
                int i = 0;
                var enu = value as IEnumerable;
                foreach (var obj in enu)
                {
                    sb.AppendLine(Pad("\r\n[" + i + "]: ", 20));
                    AppendValue(sb, obj.GetType(), obj);
                    i++;
                }
            }
            else
                sb.Append(value);
        }

        private static string Pad(string bas, int len)
        {
            while (bas.Length < len)
                bas = bas + " ";
            return bas;
        }
    }
}
