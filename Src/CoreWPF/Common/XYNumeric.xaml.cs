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
    /// Interaction logic for XYNumeric.xaml
    /// </summary>
    public partial class XYNumeric : UserControl
    {
        public XYNumeric()
        {
            InitializeComponent();
        }
        public unsafe void SetValue(float* e_pfValueX, float* e_pfValueY)
        {
            this.m_X_numericUpDown.SetValue(e_pfValueX);
            this.m_Y_numericUpDown.SetValue(e_pfValueY);
        }
        public unsafe void SetValue(float* e_pfValue)
        {
            if (e_pfValue != null)
            {
                this.m_X_numericUpDown.SetValue(&e_pfValue[0]);
                this.m_Y_numericUpDown.SetValue(&e_pfValue[1]);
            }
            else 
            {
                this.m_X_numericUpDown.SetValue(null);
                this.m_Y_numericUpDown.SetValue(null);
            }
        }

        public void SetMaximim(float e_fMaximim)
        {
            this.m_X_numericUpDown.Maximum = e_fMaximim;
            this.m_Y_numericUpDown.Maximum = e_fMaximim;
        }

        public void SetMinimim(float e_fMinimim)
        {
            this.m_X_numericUpDown.Minimum = e_fMinimim;
            this.m_Y_numericUpDown.Minimum = e_fMinimim;
        }
    }
}
