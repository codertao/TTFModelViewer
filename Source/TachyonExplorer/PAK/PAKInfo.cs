using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKInfo : BaseToString
    {
        public short Filler1 { get; set; }
        public short Type { get; set; }
        public int Value { get; set; } 
    }
}
