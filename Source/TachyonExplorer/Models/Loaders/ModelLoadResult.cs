using System;
using System.Collections.Generic;

namespace TachyonExplorer.Models.Loaders
{
    public class ModelLoadResult
    {
        public List<Tuple<String, Model>> Models { get; set; }
        public List<string> Messages { get; set; }

        public ModelLoadResult()
        {
            Models = new List<Tuple<string, Model>>();
            Messages = new List<string>();
        }
    }
}
