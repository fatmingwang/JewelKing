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

namespace CoreWPF.Common
{
    /// <summary>
    /// EditableListbox.xaml
    /// 
    /// listbox will add a textbox as its item
    /// </summary>
    public partial class EditableListbox : UserControl
    {
        string m_TextStartEditText;
        public bool m_bSameItemAddAlow = false;
        public event EventHandler f_DeleteSelectedItem;
        public event EventHandler f_AddItem;
        public EditableListbox()
        {
            InitializeComponent();
            this.m_ListBox.KeyUp += new System.Windows.Input.KeyEventHandler(EditableListbox_KeyUp);
            this.m_ListBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            this.m_ListBox.SelectionMode = System.Windows.Controls.SelectionMode.Extended;
        }

        public int IndexOf(string e_strName)
        {
            int l_iIndex = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return l_iIndex;
                ++l_iIndex;
            }
            return -1;
        }
        public bool IsContain(string e_strName)
        {
            foreach(TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return true;
            }
            return false;
        }

        public int NumSame(string e_strName)
        {
            int l_iValue = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    ++l_iValue;
            }
            return l_iValue;
        }

        public bool AddListBoxItem(string e_strName)
        {
            if (!m_bSameItemAddAlow)
            {
                if (IsContain(e_strName))
                    return false;
            }
            TextBox l_TextBox = new TextBox();
            l_TextBox.Text = e_strName;
            l_TextBox.IsEnabled = false;
            //l_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
            //l_TextBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            m_ListBox.Items.Add(l_TextBox);
            if (f_AddItem != null)
            { 
                System.EventArgs e = new EventArgs();
                f_AddItem(l_TextBox, e);
            }
            return true;
        }

        void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox l_TextBox = sender as TextBox;
            if (NumSame(l_TextBox.Text) != 1 && m_bSameItemAddAlow != true)
            {
                MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                l_TextBox.Text = m_TextStartEditText;
            }
            l_TextBox.IsEnabled = false;
        }

        public void DeleteSelectedItems()
        {
            while (this.m_ListBox.SelectedItems.Count>0)
            {
                if (null != f_DeleteSelectedItem)
                {
                    System.EventArgs e = new EventArgs();
                    f_DeleteSelectedItem(this.m_ListBox.SelectedItems[0], e);
                }
                this.m_ListBox.Items.RemoveAt(this.m_ListBox.Items.IndexOf(this.m_ListBox.SelectedItems[0]));
            }
        }

        public bool IsInEditMode()
        {
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.IsEnabled == true)
                    return true;
            }
            return false;
        }

        public string SelectedName()
        {
            if (m_ListBox.SelectedItem != null)
            {
                TextBox l_TextBox = m_ListBox.SelectedItem as TextBox;
                return l_TextBox.Text;
            }
            return null;
        }
        void EditableListbox_KeyUp(System.Object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                TextBox l_TextBox = m_ListBox.SelectedItem as TextBox;
                if (e.Key == Key.Enter)
                {
                    if (NumSame(l_TextBox.Text) == 1 || m_bSameItemAddAlow == true)
                    {
                        l_TextBox.IsEnabled = false;
                        l_TextBox.LostFocus -= new RoutedEventHandler(TextBox_LostFocus);
                    }
                    else
                    {
                        MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                    }
                }
                else
                    if (e.Key == Key.F2)
                    {
                        if (l_TextBox.IsEnabled == false)
                        {
                            l_TextBox.IsEnabled = true;
                            l_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
                            m_TextStartEditText = l_TextBox.Text;
                            l_TextBox.SelectAll();
                            l_TextBox.Focus();
                        }
                    }
            }
        }

        void EditableListbox_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                TextBox l_TextBox = this.m_ListBox.SelectedItem as TextBox;
                if (l_TextBox.IsEnabled == false)
                {
                    l_TextBox.IsEnabled = true;
                    l_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
                }
            }
        }
    }
}
