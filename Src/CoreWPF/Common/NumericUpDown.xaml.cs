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
    /// Interaction logic for NumericUpDown.xaml
    /// </summary>
    /// 
    public partial class NumericUpDown : UserControl
    {
        public event EventHandler f_ValueChanged;
        unsafe public NumericUpDown()
        {
            InitializeComponent();
            m_Value_TextBox.Text = m_fValue.ToString();
            m_pfValue = null;
        }
        public unsafe void SetValue(float* e_pfValue)
        {
            if (e_pfValue != null)
            {
                m_pfValue = e_pfValue;
                Value = *m_pfValue;
            }
            else 
            {
                m_pfValue = null;
            }
        }
        unsafe public float* m_pfValue;
        private float m_fMaximumValue = 100;
        public float Maximum{get { return m_fMaximumValue; }set { m_fMaximumValue = value; }}
        private float m_fIncrement = 1;
        public float Increment { get { return m_fIncrement; } set { m_fIncrement = value; } }
        private float m_fMinimumValue = 0;
        public float Minimum { get { return m_fMinimumValue; } set { m_fMinimumValue = value; } }
        private float m_fValue = 0;
        public float Value
        {
            get 
            {
                return m_fValue; 
            }
            set
            {
                RoutedEventArgs e = new RoutedEventArgs();
                ValueChangeVaild(m_fValue, value, m_Increase_Button, e);
            }
        }

        unsafe private void ValueChangeVaild(float e_fOld, float e_fNew, object sender, RoutedEventArgs e)
        {
            if (e_fOld != e_fNew)
            {
                if (e_fNew <= m_fMaximumValue && e_fNew >= m_fMinimumValue)
                {
                    bool l_bValueChanged = m_fValue != e_fNew ? true : false;
                    m_fValue = e_fNew;
                    if (m_pfValue != null)
                        *m_pfValue = m_fValue;
                    if (l_bValueChanged&&f_ValueChanged != null)
                    {
                        f_ValueChanged(sender, e);
                    }
                    m_Value_TextBox.Text = m_fValue.ToString();
                }
                else
                    m_fValue = e_fOld;
            }
        }

        private void ValueAdd_Click(object sender, RoutedEventArgs e)
        {
            Value += m_fIncrement;
        }

        private void ValueMinusd_Click(object sender, RoutedEventArgs e)
        {
            Value -= m_fIncrement;
        }

        private void txtNum_TextChanged(object sender, TextChangedEventArgs e)
        {
            float l_fStartValue = m_fValue;
            if (float.TryParse(m_Value_TextBox.Text, out m_fValue))
            {
                if (l_fStartValue != m_fValue)
                    ValueChangeVaild(l_fStartValue, m_fValue, sender, e);
            }
        }
        private void ValueWithMouseWhell(Object sender,MouseWheelEventArgs e)
        {
            if (this.m_Value_TextBox.IsFocused)
            {
                if (e.Delta > 0)
                {
                    Value += m_fIncrement;
                }
                else
                    Value -= m_fIncrement;
            }
        }
        private void m_Value_TextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Up)
            {
                ValueAdd_Click(sender, e);
            }
            else
            if (e.Key == Key.Down)
            {
                ValueMinusd_Click(sender, e);
            }
        }
    }
}
