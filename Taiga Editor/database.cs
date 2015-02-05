using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Taiga_Editor
{
    // Header
    public class cDBObject
    {
        public string name = new string('0', 1);
    }
    public class cUnit : cDBObject
    {

    }
    public class cItem : cDBObject
    {
    }

    public class cUIElement : cDBObject
    {

    }

    public class cDatabase
    {
        public Form1 parent;

        int unitCount = 0;
        cUnit[] unit = new cUnit[256];
        int itemCount = 0;
        cItem[] item = new cItem[256];
        int uiElementCount = 0;
        cUIElement[] uiElement = new cUIElement[256];

        public int addUnit(string name)
        {
            unit[unitCount].name = name;
            parent.treeView.Nodes.Find("node_units", true)[0].Nodes.Add(unitCount.ToString(), name);
            unitCount += 1;
            return unitCount - 1;
        }
        public int addItem(string name)
        {
            item[itemCount].name = name;
            parent.treeView.Nodes.Find("node_items", true)[0].Nodes.Add(itemCount.ToString(), name);
            itemCount += 1;
            return itemCount - 1;
        }
        public int addUIElement(string name)
        {
            uiElement[uiElementCount].name = name;
            parent.treeView.Nodes.Find("node_ui", true)[0].Nodes.Add(uiElementCount.ToString(), name);
            uiElementCount += 1;
            return itemCount - 1;
        }

        public cDatabase()
        {
            for (int i = 0; i < 256; i++)
            {
                unit[i] = new cUnit();
                item[i] = new cItem();
                uiElement[i] = new cUIElement();
            }
        }
    }
}
