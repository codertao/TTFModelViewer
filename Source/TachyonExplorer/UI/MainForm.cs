using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Nexus;
using OpenTK.Graphics.OpenGL;
using OpenTK.Input;
using TachyonExplorer.FileAccess;
using TachyonExplorer.FileAccess.PFF;
using TachyonExplorer.Models;
using TachyonExplorer.UI.Rendering;
using ButtonState = OpenTK.Input.ButtonState;

namespace TachyonExplorer.UI
{
    public partial class MainForm : Form
    {
        private string BaseTitle = "Tachyon Explorer";
        private ModelRenderer renderer;
        private Stopwatch sw;
        private IFileAccess fileAccess;
        private bool loaded;
        private Camera camera;

        public MainForm()
        {
            InitializeComponent();
            sw = Stopwatch.StartNew();
            camera = new Camera(new Point3D(0,100,0), Vector3D.UnitZ, -1*Vector3D.UnitY);
            LoadSettings();
            Closing += (sender, args) => SaveSettings();
        }

        private void glControl_Load(object sender, System.EventArgs e)
        {
            loaded = true;
            Initialize();
            SetViewPort();
            Application.Idle += Application_Idle;
            glControl.Focus();
        }

        private void Application_Idle(object sender, System.EventArgs e)
        {
            double ms = sw.Elapsed.TotalMilliseconds;
            sw.Restart();
            Accumulate(ms);

            UpdateState(ms);
            Render();
            glControl.Invalidate();
        }

        private void glControl_Paint(object sender, PaintEventArgs e)
        {
            Render();
        }

        double accumulator;
        int idleCounter;
        private void Accumulate(double milliseconds)
        {
            idleCounter++;
            accumulator += milliseconds;
            if (accumulator > 1000)
            {
                Text = BaseTitle + "- " + (idleCounter * 1000.0 / accumulator).ToString("F0") + "FPS";
                accumulator = 0;
                idleCounter = 0;
            }
        }


        public void Initialize()
        {
            GL.ClearColor(Color.SkyBlue);
            GL.Enable(EnableCap.Texture2D);
            GL.Enable(EnableCap.DepthTest);
            GL.Hint(HintTarget.PerspectiveCorrectionHint, HintMode.Nicest);
        }

        public void SetViewPort()
        {
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(camera.GetProjectionMatrix(glControl.Width/(float)glControl.Height));
            GL.Viewport(0, 0, glControl.Width, glControl.Height); // Use all of the glControl painting area
        }

        public void Render()
        {
            if (!loaded) return;
            try
            {
                GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                
                GL.MatrixMode(MatrixMode.Modelview);
                GL.LoadMatrix(camera.GetWorldMatrix());
                if(renderer != null)
                    renderer.Render();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            glControl.SwapBuffers();
        }

        private void glControl_Resize(object sender, System.EventArgs e)
        {
            if (!loaded) return;
            SetViewPort();
            glControl.Invalidate();
        }

        private MouseState prevState;
        private void UpdateState(double ms)
        {
            float amt = (float)(50 * (ms / 1000f));
            float rotAmt = (float)(5 * (ms / 1000f));
            var state = Keyboard.GetState();
            if (state.IsKeyDown(Key.A)) camera.MoveRight(-amt);
            if (state.IsKeyDown(Key.D)) camera.MoveRight(amt);
            if (state.IsKeyDown(Key.W)) camera.MoveForward(amt);
            if (state.IsKeyDown(Key.S)) camera.MoveForward(-amt);
            if (state.IsKeyDown(Key.Q)) camera.RollRight(rotAmt / 5);
            if (state.IsKeyDown(Key.E)) camera.RollRight(-rotAmt / 5);
            if (state.IsKeyDown(Key.Z)) camera.MoveUp(amt);
            if (state.IsKeyDown(Key.X)) camera.MoveUp(-amt);

            MouseState newState = Mouse.GetState();
            Vector2D diff = new Vector2D(newState.X - prevState.X, newState.Y - prevState.Y);
            if (prevState.RightButton == ButtonState.Pressed && newState.RightButton == ButtonState.Pressed)
            {
                camera.YawRight(diff.X / 200);
                camera.PitchUp(diff.Y / 200);
                Point p = glControl.PointToScreen(new Point(glControl.Width/2, glControl.Height/2));
                Mouse.SetPosition(p.X, p.Y);
            }
            prevState = newState;
        }

        private void LoadSettings()
        {
            TryLoadArchive(AppSettings.GetString("PFF_PATH"));
            camera.Position = AppSettings.GetPoint3D("CAM_POS", camera.Position);
            camera.Up = AppSettings.GetVector3D("CAM_UP", camera.Up);
            camera.Forward = AppSettings.GetVector3D("CAM_FORWARD", camera.Forward);
        }

        private void SaveSettings()
        {
            if (fileAccess != null)
                AppSettings.SetString("PFF_PATH", fileAccess.ConstructionParams);
            AppSettings.SetPoint3D("CAM_POS", camera.Position);
            AppSettings.SetVector3D("CAM_UP", camera.Up);
            AppSettings.SetVector3D("CAM_FORWARD", camera.Forward);
            AppSettings.Save();
        }

        private bool TryLoadArchive(string path)
        {
            try
            {
                if (Directory.Exists(path))
                    fileAccess = new DirectoryFileAccess(path);
                else if (File.Exists(path))
                    fileAccess = new PFFFile(path);
                else
                    return false;

                treeView1.Nodes.Clear();

                AddPAKNodes();
                AddModelNodes("OCF");

                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }
        }

        private void AddPAKNodes()
        {
            TreeNode node = treeView1.Nodes.Add("PAKs");
            node.Expand();
            var models = fileAccess.GetFiles().Where(f => f.EndsWith(".pak", StringComparison.InvariantCultureIgnoreCase)).ToList();
            foreach (var model in models.OrderBy(m => m.ToLower()))
            {
                node.Nodes.Add(new PAKTreeNode(fileAccess, model));
            }
        }

        private void AddModelNodes(string ext)
        {
            TreeNode node = treeView1.Nodes.Add(ext + "s");
            node.Expand();
            var models = fileAccess.GetFiles().Where(f => f.ToLower().EndsWith("." + ext.ToLower())).ToList();
            foreach (var model in models.OrderBy(m => m.ToLower()))
            {
                var mNode = node.Nodes.Add(model, model);
                mNode.Tag = model;
            }
        }

        private void loadPFFButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.CheckFileExists = true;
            ofd.Multiselect = false;
            ofd.Title = "Choose a PFF File";
            ofd.Filter = "PFF File (*.pff)|*.pff";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                TryLoadArchive(ofd.FileName);
            }
        }

        private void loadDirectoryButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                TryLoadArchive(fbd.SelectedPath);
            }
        }

        private void resetCameraButton_Click(object sender, EventArgs e)
        {
            camera = new Camera(new Point3D(0, 100, 0), Vector3D.UnitZ, -1 * Vector3D.UnitY);
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var node = e.Node;
            if (node is ModelTreeNode)
            {
                LoadModel(((ModelTreeNode)node).GetModel());
            } 
            else if (node is FileTreeNode)
            {
                ((FileTreeNode)node).LoadChildren();
                var modelNode = node.Nodes.OfType<ModelTreeNode>().FirstOrDefault();
                if(modelNode != null)
                    LoadModel(modelNode.GetModel());
            }
        }

        private void LoadModel(Model model)
        {
            if (renderer != null)
            {
                renderer.Unload();
                renderer = null;
            }
            if (model != null)
            {
                renderer = new ModelRenderer(model);
            }
        }

        private void treeView1_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            var node = e.Node;
            if(node is FileTreeNode)
                ((FileTreeNode)node).LoadChildren();
        }
    }
}
