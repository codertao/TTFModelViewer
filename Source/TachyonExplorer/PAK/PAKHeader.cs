using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKHeader : BaseToString
    {
        public string Identifier { get; set; }
        public int Version { get; set; }
        public string Name { get; set; }
        public int Unknown1 { get; set; }
        public int GroupCount { get; set; }
        public int Unknown2 { get; set; }
        public int Filler1 { get; set; }
        public int InfoListIndex { get; set; }
        public int TextureStart { get; set; }
        public int TextureEnd { get; set; }
        public int Filler2 { get; set; }
        public int Unknown3 { get; set; }
        public int Filler3 { get; set; }
    }
}
