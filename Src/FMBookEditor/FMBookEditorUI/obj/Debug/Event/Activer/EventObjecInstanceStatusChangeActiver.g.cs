﻿#pragma checksum "..\..\..\..\Event\Activer\EventObjecInstanceStatusChangeActiver.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "45D0C594A5752134815E4F7C977AA0E3"
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


namespace FMBookEditorUI.Event.Activer {
    
    
    /// <summary>
    /// EventObjecInstanceStatusChangeActiver
    /// </summary>
    public partial class EventObjecInstanceStatusChangeActiver : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_EventObjectInstanceName Name Field
        /// </summary>
        
        #line 17 "..\..\..\..\Event\Activer\EventObjecInstanceStatusChangeActiver.xaml"
        public System.Windows.Controls.TextBox m_EventObjectInstanceName;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_StatusName Name Field
        /// </summary>
        
        #line 19 "..\..\..\..\Event\Activer\EventObjecInstanceStatusChangeActiver.xaml"
        public System.Windows.Controls.TextBox m_StatusName;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/event/activer/eventobjecinstancestatuschangeactiver.xam" +
                    "l", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\Event\Activer\EventObjecInstanceStatusChangeActiver.xaml"
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
            this.m_EventObjectInstanceName = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_StatusName = ((System.Windows.Controls.TextBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

