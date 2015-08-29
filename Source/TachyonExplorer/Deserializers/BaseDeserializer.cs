using System.IO;

namespace TachyonExplorer.Deserializers
{
    public class BaseDeserializer : BaseToString //done because I'm lazy
    {
        protected byte[] data;

        protected BaseDeserializer(byte[] data)
        {
            this.data = data;
        }

        protected int ReadInt32(int idx)
        {
            return OpenReader(idx).ReadInt32();
        }

        protected uint ReadUInt32(int idx)
        {
            return OpenReader(idx).ReadUInt32();
        }

        protected BinaryReader OpenReader(int idx)
        {
            MemoryStream ms = new MemoryStream(data);
            ms.Position = idx;
            return new BinaryReader(ms);
        }

        protected byte[] ReadBytes(int idx, int len)
        {
            MemoryStream ms = new MemoryStream(data);
            ms.Position = idx;
            byte[] b = new byte[len];
            ms.Read(b, 0, len);
            return b;
        }
    }
}
