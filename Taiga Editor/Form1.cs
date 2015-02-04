using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Taiga_Editor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            treeView.Nodes.Find("node_units", true)[0].Nodes.Add("Default Unit [%unit name%] [%unit name%] [%unit name%]");

            treeView.Nodes.Find("node_items", true)[0].Nodes.Add("Default Item [%item name%]");
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }
    }
}
