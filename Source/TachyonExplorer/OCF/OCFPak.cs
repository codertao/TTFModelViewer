using TachyonExplorer.Deserializers;

namespace TachyonExplorer.OCF
{
    public class OCFPak : BaseToString
    {
        public string Name { get; set; }
        public string PAKFileName { get; set; }
        public string PALFileName { get; set; }
        public int Unknown { get; set; }
        public int Type { get; set; }
    }
}
