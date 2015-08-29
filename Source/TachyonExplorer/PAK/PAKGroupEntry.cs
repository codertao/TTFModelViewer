using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKGroupEntry : BaseToString
    {
        public int StartIndex { get; set; }
        public int Unknown1 { get; set; }
        public int Unknown2 { get; set; }
        public int Unknown3 { get; set; }
        public int Unknown4 { get; set; }
        public int Unknown5 { get; set; }
        public int Unknown6 { get; set; }
        public int Unknown7 { get; set; }
        public int Unknown8 { get; set; }
        public int Unknown9 { get; set; }
        public int Unknown10 { get; set; }

        [IgnoreForPrinting] public int RawDataLength { get { return RawData.Length; } }
        [IgnoreForPrinting] public byte[] RawData { get; set; }
        [IgnoreForPrinting] public PAKOBJ OBJ { get; set; }
    }
}
