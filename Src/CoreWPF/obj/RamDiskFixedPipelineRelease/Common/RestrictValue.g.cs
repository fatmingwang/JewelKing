﻿#pragma checksum "..\..\..\Common\RestrictValue.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "023FC290421BC57C5D5EF984AA212B75"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.6400
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using CoreWPF.Common;
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
    /// RestrictValue
    /// </summary>
    public partial class RestrictValue : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_Type Name Field
        /// </summary>
        
        #line 17 "..\..\..\Common\RestrictValue.xaml"
        public System.Windows.Controls.ComboBox m_Type;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Max Name Field
        /// </summary>
        
        #line 26 "..\..\..\Common\RestrictValue.xaml"
        public CoreWPF.Common.NumericUpDown m_Max;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Min Name Field
        /// </summary>
        
        #line 30 "..\..\..\Common\RestrictValue.xaml"
        public CoreWPF.Common.NumericUpDown m_Min;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_IncreaseValue Name Field
        /// </summary>
        
        #line 34 "..\..\..\Common\RestrictValue.xaml"
        public CoreWPF.Common.NumericUpDown m_IncreaseValue;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/CoreWPF;component/common/restrictvalue.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\Common\RestrictValue.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal System.Delegate _CreateDelegate(System.Type delegateType, string handler) {
            return System.Delegate.CreateDelegate(delegateType, this, handler);
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.m_Type = ((System.Windows.Controls.ComboBox)(target));
            
            #line 17 "..\..\..\Common\RestrictValue.xaml"
            this.m_Type.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler(this.m_Type_SelectionChanged);
            
            #line default
            #line hidden
            return;
            case 2:
            this.m_Max = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 3:
            this.m_Min = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 4:
            this.m_IncreaseValue = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

