using System;
using System.Collections.Generic;
using System.Linq;
using Nexus;
using Nexus.Graphics.Transforms;
using TachyonExplorer.FileAccess;
using TachyonExplorer.PAK;

namespace TachyonExplorer.Models.Loaders
{
    public static class PAKLoader
    {
        public static List<Tuple<String, Model>> LoadModels(IFileAccess access, string name)
        {
            var pak = new PAKFile(access.GetContent(name));
            return LoadModels(pak);
        }

        public static List<Tuple<String, Model>> LoadModels(PAKFile file)
        {
            var ret = new List<Tuple<String, Model>>();
            foreach (var gi in file.FileGroups.Select((g, i) => new {g, i}))
            {
                foreach (var entry in gi.g.Entries)
                {
                    string name = gi.i + "_" + entry.OBJ.Name;
                    Model m = LoadModel(file, entry.OBJ);
                    ret.Add(new Tuple<string, Model>(name, m));
                }
            }
            return ret;
        }

        public static Model LoadModel(PAKFile file, PAKOBJ obj)
        {
            Model mdl = new Model();

            Func<FaceData, int> offConv = f =>
            {
                int idx = f.ImageOffset/ImageData.RecordSize;
                return 2*idx + (obj.Images[idx].MaybeTransparent ? 1 : 0);
            };

            var faceGroups = obj.Faces.GroupBy(offConv).ToList();
            foreach (var faceGroup in faceGroups)
            {
                var imageIdx = faceGroup.First().ImageOffset/ImageData.RecordSize;
                var image = obj.Images[imageIdx];
                var texture = file.Textures.Single(t => t.Name.EqualsCI(image.ImageName));
                Mesh m = new Mesh();
                m.Material = new Material
                {
                    Transparent = image.MaybeTransparent,
                    TextureName = texture.Name,
                };
                mdl.Textures[m.Material.TextureName] = texture.Image;
                foreach (var face in faceGroup)
                {
                    Face f = new Face();
                    f.Vertices[0] = GetVertex(obj, face.Vertex1Offset, face.Normal1Offset);
                    f.Vertices[1] = GetVertex(obj, face.Vertex2Offset, face.Normal2Offset);
                    f.Vertices[2] = GetVertex(obj, face.Vertex3Offset, face.Normal3Offset);
                    m.Faces.Add(f);
                }
                mdl.Meshes.Add(m);
                mdl.Instances.Add(new MeshInstance
                {
                    MeshIndex = mdl.Meshes.Count - 1,
                    Transform = Matrix3D.Identity
                });
            }
            return mdl;
        }

        private static Vertex GetVertex(PAKOBJ obj, int v0Offset, int n0Offset)
        {
            var v0 = obj.Verticies[v0Offset / VertexData.RecordSize];
            var n0 = obj.Normals[n0Offset / NormalData.RecordSize];
            Vertex v = new Vertex();
            v.Position = new Point3D(v0.RawX, v0.RawY, v0.RawZ);
            v.Normal = new Vector3D(n0.RawX, n0.RawY, n0.RawZ);
            v.Texture = new Point2D(v0.RawU, v0.RawV) * (1 / 65536.0f);
            v.Normal.Normalize();

            return v;
        }
    }
}
