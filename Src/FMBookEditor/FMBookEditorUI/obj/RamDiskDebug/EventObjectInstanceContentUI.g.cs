﻿#pragma checksum "..\..\EventObjectInstanceContentUI.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "9450DC88009893D1232F072082304759"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.6400
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


namespace FMBookEditorUI {
    
    
    /// <summary>
    /// EventObjectInstanceContentForListBoxUI
    /// </summary>
    public partial class EventObjectInstanceContentForListBoxUI : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_EventObjecInstanceName Name Field
        /// </summary>
        
        #line 11 "..\..\EventObjectInstanceContentUI.xaml"
        public System.Windows.Controls.TextBox m_EventObjecInstanceName;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventObjecStatus Name Field
        /// </summary>
        
        #line 21 "..\..\EventObjectInstanceContentUI.xaml"
        public System.Windows.Controls.TextBox m_EventObjecStatus;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventInstance Name Field
        /// </summary>
        
        #line 33 "..\..\EventObjectInstanceContentUI.xaml"
        public System.Windows.Controls.TextBox m_EventInstance;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_StatusEventVariable Name Field
        /// </summary>
        
        #line 45 "..\..\EventObjectInstanceContentUI.xaml"
        public System.Windows.Controls.TextBox m_StatusEventVariable;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/eventobjectinstancecontentui.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\EventObjectInstanceContentUI.xaml"
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
            this.m_EventObjecInstanceName = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_EventObjecStatus = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.m_EventInstance = ((System.Windows.Controls.TextBox)(target));
            return;
            case 4:
            this.m_StatusEventVariable = ((System.Windows.Controls.TextBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}
