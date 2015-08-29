using System.Collections.Generic;

namespace TachyonExplorer.FileAccess
{
    public class BlankFileAccess : IFileAccess
    {
        public string ConstructionParams { get { return "";  } }

        public List<string> GetFiles()
        {
            return new List<string>();
        }

        public byte[] GetContent(string fileName)
        {
            return new byte[0];
        }
    }
}
