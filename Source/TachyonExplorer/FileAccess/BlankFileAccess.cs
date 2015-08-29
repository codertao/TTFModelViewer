using System.Collections.Generic;

namespace TachyonExplorer.FileAccess
{
    public class BlankFileAccess : IFileAccess
    {
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
