﻿#pragma checksum "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "D1D8C0D53692AC5ACEA66124938AA14F"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.5466
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


namespace FMBookEditorUI.Event.Activer {
    
    
    /// <summary>
    /// EventValueChangeActiver
    /// </summary>
    public partial class EventValueChangeActiver : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_EventVariable Name Field
        /// </summary>
        
        #line 14 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
        public System.Windows.Controls.TextBox m_EventVariable;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_TabControl Name Field
        /// </summary>
        
        #line 17 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
        public System.Windows.Controls.TabControl m_TabControl;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Value_0 Name Field
        /// </summary>
        
        #line 26 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
        public System.Windows.Controls.TextBox m_Value_0;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_TargetVariable Name Field
        /// </summary>
        
        #line 33 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
        public System.Windows.Controls.TextBox m_TargetVariable;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_RestrictValue1 Name Field
        /// </summary>
        
        #line 46 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
        public CoreWPF.Common.RestrictValue m_RestrictValue1;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/event/activer/eventvaluechangeactiver.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.m_EventVariable = ((System.Windows.Controls.TextBox)(target));
            
            #line 14 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
            this.m_EventVariable.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.m_EventVariable_0_TextChanged);
            
            #line default
            #line hidden
            return;
            case 2:
            this.m_TabControl = ((System.Windows.Controls.TabControl)(target));
            return;
            case 3:
            this.m_Value_0 = ((System.Windows.Controls.TextBox)(target));
            
            #line 26 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
            this.m_Value_0.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.m_EventVariable_0_TextChanged);
            
            #line default
            #line hidden
            return;
            case 4:
            this.m_TargetVariable = ((System.Windows.Controls.TextBox)(target));
            
            #line 33 "..\..\..\..\Event\Activer\EventValueChangeActiver.xaml"
            this.m_TargetVariable.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.m_EventVariable_0_TextChanged);
            
            #line default
            #line hidden
            return;
            case 5:
            this.m_RestrictValue1 = ((CoreWPF.Common.RestrictValue)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

