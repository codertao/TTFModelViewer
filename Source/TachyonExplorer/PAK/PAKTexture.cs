using System.Drawing;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKTexture : BaseToString
    {
        public string Name { get; set; }
        public ushort Unknown1 { get; set; }
        public int Rows { get; set; }
        public int Cols { get; set; }
        public int DataLength { get; set; }
        public ushort PaletteCount { get; set; }
        public Bitmap Image { get; set; }
        public byte[] RawData { get; set; }
        public byte[] RawPalette { get; set; }
    }
}
