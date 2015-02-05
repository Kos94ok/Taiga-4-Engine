﻿using System;
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
        // Initialization
        public cDatabase database = new cDatabase();
        public Form1()
        {
            InitializeComponent();

            database.parent = this;
            database.addUnit("missingno");
            database.addItem("missingno");
            database.addUIElement("missingno");
        }

        // Tree view object selection
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            tabControl1.TabPages.Remove(tab_unit);
            tabControl1.TabPages.Remove(tab_item);
            tabControl1.TabPages.Remove(tab_ui);
            if (treeView.SelectedNode.Parent != null)
            {
                tabControl1.Show();
                if (treeView.SelectedNode.Parent.Name == "node_units")
                {
                    tabControl1.TabPages.Add(tab_unit);
                    text_unit_name.Text = "missingno";
                }
                else if (treeView.SelectedNode.Parent.Name == "node_items")
                {
                    tabControl1.TabPages.Add(tab_item);
                }
                else if (treeView.SelectedNode.Parent.Name == "node_ui")
                {
                    tabControl1.TabPages.Add(tab_ui);
                }
            }
            else
            {
                tabControl1.Hide();
            }
        }

        // Add new element button
        private void btn_addElement_Click(object sender, EventArgs e)
        {
            treeView.BeginUpdate();
            // New unit
            if (treeView.SelectedNode.Name == "node_units")
            {
                database.addUnit("missingno");
            }
            // New item
            else if (treeView.SelectedNode.Name == "node_items")
            {
                database.addItem("missingno");
            }
            // New UI element
            else if (treeView.SelectedNode.Name == "node_ui")
            {
                database.addUIElement("missingno");
            }
            treeView.EndUpdate();
        }
    }
}
