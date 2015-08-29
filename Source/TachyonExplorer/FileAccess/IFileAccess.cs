using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TachyonExplorer.FileAccess
{
    public interface IFileAccess
    {
        List<string> GetFiles();
        byte[] GetContent(string fileName);
    }
}
