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
            camera = new Camera(new Point3D(-10,0,0), Vector3D.UnitZ, Vector3D.UnitX);
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

            var models = Models.Loaders.PAKLoader.LoadModels(fileAccess, fileAccess.GetFiles().First(f => f.ToLower().EndsWith(".pak") && f.ToLower().StartsWith("scara")));
            renderer = new ModelRenderer(models[0].Item2);
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
            float amt = (float)(4*(ms/1000f));
            var state = Keyboard.GetState();
            if (state.IsKeyDown(Key.A)) camera.MoveRight(-amt);
            if (state.IsKeyDown(Key.D)) camera.MoveRight(amt);
            if (state.IsKeyDown(Key.W)) camera.MoveForward(amt);
            if (state.IsKeyDown(Key.S)) camera.MoveForward(-amt);
            if (state.IsKeyDown(Key.Q)) camera.RollRight(amt/5);
            if (state.IsKeyDown(Key.E)) camera.RollRight(-amt/5);
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
            TryLoadPFF(AppSettings.GetString("PFF_PATH"));
            while (fileAccess == null)
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.CheckFileExists = true;
                ofd.Multiselect = false;
                ofd.Title = "Choose a PFF File";
                ofd.Filter = "PFF File (*.pff)|*.pff";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    TryLoadPFF(ofd.FileName);
                }
                else
                {
                    Environment.Exit(0);
                }
            }
        }

        private void SaveSettings()
        {
            if (fileAccess != null)
                AppSettings.SetString("PFF_PATH", fileAccess.ConstructionParams);
            AppSettings.Save();
        }

        private bool TryLoadPFF(string path)
        {
            try
            {
                if (Directory.Exists(path))
                    fileAccess = new DirectoryFileAccess(path);
                else if (File.Exists(path))
                    fileAccess = new PFFFile(path);
                else
                    return false;
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }
        }
    }
}
