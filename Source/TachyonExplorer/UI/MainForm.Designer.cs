namespace TachyonExplorer.UI
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.glControl = new OpenTK.GLControl();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.loadPFFButton = new System.Windows.Forms.Button();
            this.loadDirectoryButton = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // glControl
            // 
            this.glControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.glControl.BackColor = System.Drawing.Color.Black;
            this.glControl.Location = new System.Drawing.Point(201, 13);
            this.glControl.Name = "glControl";
            this.glControl.Size = new System.Drawing.Size(997, 746);
            this.glControl.TabIndex = 1;
            this.glControl.VSync = false;
            this.glControl.Load += new System.EventHandler(this.glControl_Load);
            this.glControl.Paint += new System.Windows.Forms.PaintEventHandler(this.glControl_Paint);
            this.glControl.Resize += new System.EventHandler(this.glControl_Resize);
            // 
            // treeView1
            // 
            this.treeView1.Location = new System.Drawing.Point(13, 102);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(182, 657);
            this.treeView1.TabIndex = 2;
            this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            // 
            // loadPFFButton
            // 
            this.loadPFFButton.Location = new System.Drawing.Point(13, 13);
            this.loadPFFButton.Name = "loadPFFButton";
            this.loadPFFButton.Size = new System.Drawing.Size(182, 23);
            this.loadPFFButton.TabIndex = 3;
            this.loadPFFButton.Text = "Load PFF...";
            this.loadPFFButton.UseVisualStyleBackColor = true;
            this.loadPFFButton.Click += new System.EventHandler(this.loadPFFButton_Click);
            // 
            // loadDirectoryButton
            // 
            this.loadDirectoryButton.Location = new System.Drawing.Point(13, 43);
            this.loadDirectoryButton.Name = "loadDirectoryButton";
            this.loadDirectoryButton.Size = new System.Drawing.Size(182, 23);
            this.loadDirectoryButton.TabIndex = 4;
            this.loadDirectoryButton.Text = "Load From Directory...";
            this.loadDirectoryButton.UseVisualStyleBackColor = true;
            this.loadDirectoryButton.Click += new System.EventHandler(this.loadDirectoryButton_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(13, 73);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(182, 23);
            this.button3.TabIndex = 5;
            this.button3.Text = "Reset Camera";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1210, 771);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.loadDirectoryButton);
            this.Controls.Add(this.loadPFFButton);
            this.Controls.Add(this.treeView1);
            this.Controls.Add(this.glControl);
            this.Cursor = System.Windows.Forms.Cursors.Default;
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.ResumeLayout(false);

        }

        #endregion

        private OpenTK.GLControl glControl;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.Button loadPFFButton;
        private System.Windows.Forms.Button loadDirectoryButton;
        private System.Windows.Forms.Button button3;
    }
}