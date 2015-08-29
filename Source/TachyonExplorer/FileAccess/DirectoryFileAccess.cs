using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace TachyonExplorer.FileAccess
{
    public class DirectoryFileAccess : IFileAccess
    {
        public string ConstructionParams { get { return dir; } }

        private string dir;
        private List<string> names; 
        public DirectoryFileAccess(string dir)
        {
            this.dir = dir;
            var files = Directory.GetFiles(dir);
            names = files.Select(f => Path.GetFileName(f)).ToList();
        }

        public List<string> GetFiles()
        {
            return names;
        }

        public byte[] GetContent(string fileName)
        {
            return File.ReadAllBytes(Path.Combine(dir, fileName));
        }
    }
}
