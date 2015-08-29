using TachyonExplorer.Deserializers;

namespace TachyonExplorer.FileAccess.PFF
{
    public class PFFHeader : BaseToString
    {
        public int HeaderSize { get; set; }
        public string VersionString { get; set; }
        public int FileCount { get; set; }
        public int FileSegmentSize { get; set; }
        public int FileListOffset { get; set; }

        [IgnoreForPrinting]
        public PFFVersion Version { get; set; }

        public PFFHeader()
        {
            Version = PFFVersion.Unknown;
        }

        public static int GetSegmentSizeByVersion(PFFVersion version)
        {
            switch (version)
            {
                //case PFFVersion.PFF1: return 0;
                case PFFVersion.PFF2: return 0x20;
                case PFFVersion.PFF3: return 0x24;
                case PFFVersion.PFF4: return 0x28;
                default: return 0;
            }
        }
    }

    public enum PFFVersion
    {
        PFF1 = 1,
        PFF2 = 2,
        PFF3 = 3,
        PFF4 = 4,
        Unknown = 0
    }
}
