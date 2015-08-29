using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.FileAccess.PFF
{
    public class PFFFooter : BaseToString
    {
        public byte[] SystemIP { get; set; }
        public int Reserved { get; set; }
        public string KINGTag { get; set; }
    }
}
