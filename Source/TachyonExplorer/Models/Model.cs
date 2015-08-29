using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TachyonExplorer.Models
{
    public class Model
    {
        public List<Mesh> Meshes { get; set; }
        public List<MeshInstance> Instances { get; set; }
        public Dictionary<string, Bitmap> Textures { get; set; } 

        public Model()
        {
            Meshes = new List<Mesh>();
            Instances = new List<MeshInstance>();
            Textures = new Dictionary<string, Bitmap>();
        }
    }
}
