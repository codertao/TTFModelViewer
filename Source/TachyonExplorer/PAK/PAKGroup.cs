using System.Collections.Generic;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKGroup : BaseToString
    {
        public int Unknown1 { get; set; }
        public int EntryCount { get; set; }
        public int Unknown2 { get; set; }
        public List<PAKGroupEntry> Entries { get; set; }  
    }
}
