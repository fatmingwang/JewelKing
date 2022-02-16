using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
namespace CoreWPF.Common
{
    /// <summary>
    /// Interaction logic for FolderExplorerTree.xaml
    /// </summary>
    public partial class FolderExplorerTree : UserControl
    {
        public event EventHandler f_Drop;
        public event EventHandler f_MouseDoubleClick;
        public event EventHandler f_KeyUp;
        private object dummyNode = null;
        public string[] m_strExtensionName;//string[] split = e_strDirectory.Split(new Char[] { '\\'});
                                           //SetWorkingProject("C:\\Users\\Fatming\\Desktop\\FM79979\\Media\\MR\\Tutorial");
        public FolderExplorerTree()
        {
            InitializeComponent();
            RoutedEventArgs e = new RoutedEventArgs();
            Window_Loaded(this, e);
        }
        public string SelectedImagePath { get; set; }


        //public string GetFullPath(TreeViewItem e_pTreeViewItem)
        //{
        //    TreeViewItem l_Parent = e_pTreeViewItem;
        //    Array l_FullString;
        //    l_FullString.SetValue(l_Parent.Header, l_iIndex);
        //    while (l_Parent.Parent)
        //    {
        //        l_FullString.SetValue(l_Parent.Header, l_iIndex);
        //        l_Parent = l_Parent.Parent;
        //    }
        //    int l_iIndex = 0;
        //    for (int i = l_FullString.Length - 1; i > -1; --i)
        //    {
        //        if (l_iIndex == 0)
        //        {
        //            ++l_iIndex;
        //        }
        //        else 
        //        {

        //        }
        //    }
        //}
        public void SetWorkingProject(string e_strDirectory)
        {
            this.m_SelectedFileNameTextBlock.Text = e_strDirectory;
            string[] split = e_strDirectory.Split(new Char[] { '\\'});
            TreeViewItem l_CurrentNode = foldersItem.Items[0] as TreeViewItem;
            RoutedEventArgs e = new RoutedEventArgs();
            foreach(TreeViewItem l_TreeViewItem in foldersItem.Items)
            {
                if (l_TreeViewItem.Header.ToString().ToLower().CompareTo(split[0].ToLower()) == 0 )
                {
                    l_CurrentNode = l_TreeViewItem;
                    folder_Expanded(l_CurrentNode, e);
                    break;
                }
                
            }
            //int l_iIndex = 0;             
            foreach (string s in split)
            {
                foreach (TreeViewItem l_pData in l_CurrentNode.Items)
                {
                    if (l_pData != null && l_pData.Header.ToString().CompareTo(s) == 0)
                    {
                        folder_Expanded(l_pData, e);
                        l_CurrentNode = l_pData;
                        break;
                    }
                }
            }
            l_CurrentNode.IsExpanded = true;
            l_CurrentNode.BringIntoView();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (string s in Directory.GetLogicalDrives())
            {
                TreeViewItem item = new TreeViewItem();
                int l_iIndex = s.LastIndexOf("\\");
                item.Header = s.Substring(0,l_iIndex);
                item.Tag = s;
                item.FontWeight = FontWeights.Normal;
                item.Items.Add(dummyNode);
                item.Expanded += new RoutedEventHandler(folder_Expanded);
                foldersItem.Items.Add(item);
            }
        }

        void folder_Expanded(object sender, RoutedEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            if (item.Items.Count == 1 && item.Items[0] == dummyNode)
            {
                item.Items.Clear();
                try
                {
                    foreach (string s in Directory.GetDirectories(item.Tag.ToString()))
                    {
                        TreeViewItem subitem = new TreeViewItem();
                        subitem.Header = s.Substring(s.LastIndexOf("\\") + 1);
                        subitem.Tag = s;
                        subitem.FontWeight = FontWeights.Normal;
                        subitem.Items.Add(dummyNode);
                        subitem.Expanded += new RoutedEventHandler(folder_Expanded);
                        item.Items.Add(subitem);
                    }
                    foreach (string s in Directory.GetFiles(item.Tag.ToString()))
                    {
                        TreeViewItem subitem = new TreeViewItem();
                        subitem.Header = s.Substring(s.LastIndexOf("\\") + 1);
                        if ( m_strExtensionName != null )
                        {
                            bool    l_bSame = false;
                            foreach( string l_strExtensionName in m_strExtensionName )
                            {
                                string l_ExtensionName = System.IO.Path.GetExtension(subitem.Header.ToString());
                                if (l_strExtensionName.Equals(l_ExtensionName))
                                {
                                    l_bSame = true;
                                    break;
                                }
                            }
                            if( !l_bSame )
                                continue;
                        }
                        subitem.Tag = s;
                        subitem.FontWeight = FontWeights.Normal;
                        item.Items.Add(subitem);                    
                    }
                }
                catch (Exception) { }
            }
        }

        private void foldersItem_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeView tree = (TreeView)sender;
            TreeViewItem temp = ((TreeViewItem)tree.SelectedItem);

            if (temp == null)
                return;
            SelectedImagePath = "";
            string temp1 = "";
            string temp2 = "";
            while (true)
            {
                temp1 = temp.Header.ToString();
                if (temp1.Contains(@"\"))
                {
                    temp2 = "";
                }
                SelectedImagePath = temp1 + temp2 + SelectedImagePath;
                if (temp.Parent.GetType().Equals(typeof(TreeView)))
                {
                    break;
                }
                temp = ((TreeViewItem)temp.Parent);
                temp2 = @"\";
            }
            //show user selected path
            this.m_SelectedFileNameTextBlock.Text = SelectedImagePath;
        }

        private void foldersItem_DragEnter(object sender, DragEventArgs e)
        {
            CoreWPF.Common.CommonFunction.DragEnter(sender, e);
        }

        private void foldersItem_Drop(object sender, DragEventArgs e)
        {
            if (f_Drop != null)
            {
                f_Drop(sender, e);
            }
        }

        private void foldersItem_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_MouseDoubleClick != null)
            {
                f_MouseDoubleClick(sender, e);
            }
        }

        private void foldersItem_KeyUp(object sender, KeyEventArgs e)
        {
            if (f_KeyUp != null)
            {
                f_KeyUp(sender, e);
            }
        }
    }
}
