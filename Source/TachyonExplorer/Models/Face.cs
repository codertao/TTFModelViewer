namespace TachyonExplorer.Models
{
    public class Face
    {
        public Vertex[] Vertices { get; set; }

        public Face()
        {
            Vertices = new Vertex[3];
        }
    }
}
