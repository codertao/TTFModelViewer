using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using OpenTK.Graphics.OpenGL;
using TachyonExplorer.Models;

namespace TachyonExplorer.UI.Rendering
{
    public class ModelRenderer
    {
        private Model model;
        private bool loaded;

        private int[] textures;
        private Dictionary<string, int> textureLookup; 

        public ModelRenderer(Model model)
        {
            this.model = model;
            loaded = false;
        }

        public void Load()
        {
            if (loaded)
                return;

            textureLookup = new Dictionary<string, int>();
            textures = new int[model.Textures.Count];
            GL.GenTextures(textures.Length, textures);

            int i = 0;
            foreach (var kvp in model.Textures)
            {
                GL.BindTexture(TextureTarget.Texture2D, textures[i]);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                BitmapData data = kvp.Value.LockBits(new System.Drawing.Rectangle(0, 0, kvp.Value.Width, kvp.Value.Height),
                    ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, data.Width, data.Height, 0,
                    OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);

                kvp.Value.UnlockBits(data);
                textureLookup[kvp.Key] = textures[i];

                i++;
            }

            loaded = true;
        }

        public void Unload()
        {
            if (!loaded)
                return;

            GL.DeleteTextures(textures.Length, textures);
            loaded = false;
        }


        public void Render()
        {
            GL.PushAttrib(AttribMask.AllAttribBits);
            SetGLState();
            if (!loaded)
                Load();
            GL.MatrixMode(MatrixMode.Modelview);
            foreach (var inst in model.Instances)
            {
                GL.PushMatrix();
                GL.MultMatrix(inst.Transform.ToArray());
                Mesh mesh = model.Meshes[inst.MeshIndex];
                RenderMesh(mesh);
                GL.PopMatrix();
            }
            GL.PopAttrib();
        }

        private void RenderMesh(Mesh mesh)
        {
            BindTexture(mesh);
            GL.Begin(PrimitiveType.Triangles);
            foreach (var face in mesh.Faces)
                foreach (var vertex in face.Vertices)
                    RenderVertex(vertex);
            GL.End();
        }

        private void RenderVertex(Vertex v)
        {
            GL.TexCoord2(v.Texture.X, v.Texture.Y);
            GL.Vertex3(v.Position.X / 100000, v.Position.Y / 100000, v.Position.Z / 100000);
        }

        private void BindTexture(Mesh mesh)
        {
            if(mesh.Material.Transparent)
                GL.Enable(EnableCap.Blend);
            else
                GL.Disable(EnableCap.Blend);

            GL.BindTexture(TextureTarget.Texture2D, textureLookup[mesh.Material.TextureName]);
        }

        private void SetGLState()
        {
            GL.BlendEquation(BlendEquationMode.FuncAdd);
            GL.BlendFunc(BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcColor);
        }

        //private void DrawCube(Vector3d pt, double scale)
        //{
        //    Vector3d actual = pt;
        //    double size = scale;
        //    Vector3d top = actual + size * Vector3d.UnitZ;
        //    Vector3d bottom = actual - size * Vector3d.UnitZ;
        //    Vector3d left = actual + size * Vector3d.UnitY;
        //    Vector3d right = actual - size * Vector3d.UnitY;
        //    Vector3d front = actual + size * Vector3d.UnitX;
        //    Vector3d back = actual - size * Vector3d.UnitX;
        //    Vector3d[] pts = { front, left, back, right };
        //    GL.Color3(1f, 1f, 0);
        //    GL.Begin(PrimitiveType.Triangles);
        //    for (int i = 0; i < pts.Length; i++)
        //    {
        //        int n = (i + 1) % pts.Length;
        //        GL.Vertex3(top);
        //        GL.Vertex3(pts[i]);
        //        GL.Vertex3(pts[n]);
        //        GL.Vertex3(bottom);
        //        GL.Vertex3(pts[n]);
        //        GL.Vertex3(pts[i]);
        //    }
        //    GL.End();
        //}

    }
}
