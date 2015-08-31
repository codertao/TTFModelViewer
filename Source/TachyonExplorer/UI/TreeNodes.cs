using System;
using System.Linq;
using System.Windows.Forms;
using TachyonExplorer.FileAccess;
using TachyonExplorer.Models;
using TachyonExplorer.Models.Loaders;
using TachyonExplorer.PAK;

namespace TachyonExplorer.UI
{
    public abstract class FileTreeNode : TreeNode
    {
        protected IFileAccess access;
        protected string fileName;
        protected byte[] Content { get { return access.GetContent(fileName); } }

        public FileTreeNode(IFileAccess access, string fileName, string nodeName) : base(nodeName)
        {
            this.access = access;
            this.fileName = fileName;
            Nodes.Clear();
            Nodes.Add("dummy");
        }

        public abstract void LoadChildren();
    }

    public class PAKTreeNode : FileTreeNode
    {
        public PAKTreeNode(IFileAccess access, string fileName) : base(access, fileName, fileName) { }

        public override void LoadChildren()
        {
            Nodes.Clear();
            try
            {
                PAKFile f = new PAKFile(Content);
                var result = PAKLoader.LoadModels(f);
                foreach (var tuple in result.Models)
                {
                    string subgroupName = tuple.Item1;
                    Nodes.Add(new ModelTreeNode(subgroupName, ModelFetcher(subgroupName)));
                }
                foreach (var msg in result.Messages)
                {
                    Nodes.Add(msg);
                    if (!this.Text.EndsWith("!"))
                        this.Text = this.Text + " !";
                }
            }
            catch (Exception e)
            {
                Nodes.Add("!!! " + e.Message);
            }
        }

        private Func<Model> ModelFetcher(string sub)
        {
            //I feel like I've done too much javascript when this is a solution...
            return () => {
                PAKFile f = new PAKFile(Content);
                var result = PAKLoader.LoadModels(f);
                return result.Models.FirstOrDefault(t=>t.Item1 == sub).Item2;
            };
        }
    }

    public class ModelTreeNode : TreeNode
    {
        private Func<Model> modelLoader;
        public ModelTreeNode(string name, Func<Model> modelLoader) : base(name)
        {
            this.modelLoader = modelLoader;
        }

        public Model GetModel()
        {
            return modelLoader();
        }
    }
}
