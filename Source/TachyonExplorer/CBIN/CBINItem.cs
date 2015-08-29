using System.Collections.Generic;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.CBIN
{
    public class CBINItem : BaseToString
    {
        //ItemID uses the TextData to tell you what text it uses.
        //TotalItemParameters lets you know how many Parameters a item has.
        public int ItemId { get; set; }
        public int TotalParameters { get; set; }

        public List<CBINParameter> Parameters { get; set; }
        public string Value { get; set; }
    }
}
