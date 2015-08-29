using System.Collections.Generic;

namespace TachyonExplorer.Models
{
    public class Mesh
    {
        public Material Material { get; set; }
        public List<Face> Faces { get; set; }

        public Mesh()
        {
            Faces = new List<Face>();
        }
    }
}
