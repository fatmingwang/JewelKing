﻿#pragma checksum "..\..\..\..\Event\Trigger\EventObjectStatusCheckerTrigger.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "84575E2811F064DE23B80DAA9F39CF0D"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.5466
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


namespace FMBookEditorUI.Event.Trigger {
    
    
    /// <summary>
    /// EventObjectStatusCheckerTrigger
    /// </summary>
    public partial class EventObjectStatusCheckerTrigger : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_ObjectStatusName_Textbox Name Field
        /// </summary>
        
        #line 25 "..\..\..\..\Event\Trigger\EventObjectStatusCheckerTrigger.xaml"
        public System.Windows.Controls.TextBox m_ObjectStatusName_Textbox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_ObjectStatusValue_Textbox Name Field
        /// </summary>
        
        #line 26 "..\..\..\..\Event\Trigger\EventObjectStatusCheckerTrigger.xaml"
        public System.Windows.Controls.TextBox m_ObjectStatusValue_Textbox;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/event/trigger/eventobjectstatuscheckertrigger.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\Event\Trigger\EventObjectStatusCheckerTrigger.xaml"
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
            this.m_ObjectStatusName_Textbox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_ObjectStatusValue_Textbox = ((System.Windows.Controls.TextBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

