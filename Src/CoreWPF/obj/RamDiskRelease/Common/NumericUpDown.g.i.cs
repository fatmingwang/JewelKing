﻿#pragma checksum "..\..\..\Common\NumericUpDown.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "14435F21568D16F7C9646726D1563A22"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.17929
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace CoreWPF.Common {
    
    
    /// <summary>
    /// NumericUpDown
    /// </summary>
    public partial class NumericUpDown : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_Value_TextBox Name Field
        /// </summary>
        
        #line 11 "..\..\..\Common\NumericUpDown.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_Value_TextBox;
        
        #line default
        #line hidden
        
        
        #line 13 "..\..\..\Common\NumericUpDown.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_Increase_Button;
        
        #line default
        #line hidden
        
        
        #line 14 "..\..\..\Common\NumericUpDown.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_Decrease_Button;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/CoreWPF;component/common/numericupdown.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\Common\NumericUpDown.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.m_Value_TextBox = ((System.Windows.Controls.TextBox)(target));
            
            #line 11 "..\..\..\Common\NumericUpDown.xaml"
            this.m_Value_TextBox.MouseWheel += new System.Windows.Input.MouseWheelEventHandler(this.ValueWithMouseWhell);
            
            #line default
            #line hidden
            
            #line 11 "..\..\..\Common\NumericUpDown.xaml"
            this.m_Value_TextBox.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.txtNum_TextChanged);
            
            #line default
            #line hidden
            
            #line 11 "..\..\..\Common\NumericUpDown.xaml"
            this.m_Value_TextBox.KeyUp += new System.Windows.Input.KeyEventHandler(this.m_Value_TextBox_KeyUp);
            
            #line default
            #line hidden
            return;
            case 2:
            this.m_Increase_Button = ((System.Windows.Controls.Button)(target));
            
            #line 13 "..\..\..\Common\NumericUpDown.xaml"
            this.m_Increase_Button.MouseUp += new System.Windows.Input.MouseButtonEventHandler(this.ValueAdd_Click);
            
            #line default
            #line hidden
            return;
            case 3:
            this.m_Decrease_Button = ((System.Windows.Controls.Button)(target));
            
            #line 14 "..\..\..\Common\NumericUpDown.xaml"
            this.m_Decrease_Button.MouseUp += new System.Windows.Input.MouseButtonEventHandler(this.ValueMinusd_Click);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

