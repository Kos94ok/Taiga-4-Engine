namespace Taiga_Editor
{
    partial class Form1
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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Units");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Items");
            this.treeView = new System.Windows.Forms.TreeView();
            this.btn_addElement = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // treeView
            // 
            this.treeView.Location = new System.Drawing.Point(12, 12);
            this.treeView.MaximumSize = new System.Drawing.Size(300, 400);
            this.treeView.MinimumSize = new System.Drawing.Size(100, 400);
            this.treeView.Name = "treeView";
            treeNode1.Name = "node_units";
            treeNode1.Text = "Units";
            treeNode2.Name = "node_items";
            treeNode2.Text = "Items";
            this.treeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
            this.treeView.Size = new System.Drawing.Size(143, 400);
            this.treeView.TabIndex = 0;
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            // 
            // btn_addElement
            // 
            this.btn_addElement.Location = new System.Drawing.Point(12, 415);
            this.btn_addElement.Name = "btn_addElement";
            this.btn_addElement.Size = new System.Drawing.Size(100, 23);
            this.btn_addElement.TabIndex = 1;
            this.btn_addElement.Text = "New Element";
            this.btn_addElement.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(615, 450);
            this.Controls.Add(this.btn_addElement);
            this.Controls.Add(this.treeView);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.Button btn_addElement;
    }
}

