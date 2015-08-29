using TachyonExplorer.Deserializers;

namespace TachyonExplorer.FileAccess.PFF
{
    public class PFFFileEntry : BaseToString
    {
        public int Deleted { get; set; }
        public int FileLocation { get; set; }
        public int FileSize { get; set; }
        public int PackedDate { get; set; }
        public string FileName { get; set; }
        public int ModifiedDate { get; set; }
        public int CompressionLevel { get; set; }
    }
}
