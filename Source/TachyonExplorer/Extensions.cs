using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Nexus;

namespace TachyonExplorer
{
    public static class Extensions
    {
        public static string ReadNullTermString(this BinaryReader reader, int length)
        {
            var res = reader.ReadBytes(length);
            return new string(res.TakeWhile(c => c != '\0').Select(c => (char)c).ToArray());
        }

        public static string ReadNullTermString(this BinaryReader reader)
        {
            var bytes = new List<byte>();
            byte b;
            while((b=reader.ReadByte())!=0)
                bytes.Add(b);
            return bytes.AsString();
        }

        public static string ReadString(this BinaryReader reader, int length)
        {
            return new string(reader.ReadChars(length));
        }

        public static float ConvertToFloat(this int i)
        {
            return BitConverter.ToSingle(BitConverter.GetBytes(i), 0);
        }

        public static BinaryReader ToReader(this byte[] data)
        {
            return new BinaryReader(new MemoryStream(data));
        }

        public static string AsString(this IEnumerable<byte> data)
        {
            return new string(data.Select(c=>(char)c).ToArray());
        }

        public static bool EqualsCI(this string s1, string s2)
        {
            return String.Equals(s1, s2, StringComparison.InvariantCultureIgnoreCase);
        }

        public static double[] ToArray(this Matrix3D mat)
        {
            double[] m = new double[16];
            for (int i = 0; i < 16; i++) 
                m[i] = mat[i];
            return m;
        }
    }
}
