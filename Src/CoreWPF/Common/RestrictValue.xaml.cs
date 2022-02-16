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
    /// Interaction logic for RestrictValue.xaml
    /// </summary>
    public partial class RestrictValue : UserControl
    {
        public event EventHandler f_Valdiate;
        public RestrictValue()
        {
            InitializeComponent();
            m_Type.SelectedIndex = 0;
            m_Max.m_Value_TextBox.TextChanged += new TextChangedEventHandler(m_Value_TextBox_TextChanged);
            m_Min.m_Value_TextBox.TextChanged += new TextChangedEventHandler(m_Value_TextBox_TextChanged);
            m_IncreaseValue.m_Value_TextBox.TextChanged += new TextChangedEventHandler(m_Value_TextBox_TextChanged);
        }

        void m_Value_TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (f_Valdiate != null)
            {
                f_Valdiate(sender, e);
            }
        }

        private void m_Type_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_Valdiate != null)
            {
                f_Valdiate(sender, e);
            }
        }

    }
}
