﻿#pragma checksum "..\..\EventInstance.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "4E54B55A3B987183292404B73D596E6E"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18010
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
    /// EventInstance
    /// </summary>
    public partial class EventInstance : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        
        #line 17 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TabControl m_TabControl;
        
        #line default
        #line hidden
        
        
        #line 19 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.ListBox m_TriggerListBox;
        
        #line default
        #line hidden
        
        
        #line 35 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.ListBox m_ActiverListBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// WrapPaneTabControl Name Field
        /// </summary>
        
        #line 54 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TabControl WrapPaneTabControl;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventTriggerListWrapPanel Name Field
        /// </summary>
        
        #line 57 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.WrapPanel m_EventTriggerListWrapPanel;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventActiverListWrapPanel Name Field
        /// </summary>
        
        #line 62 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.WrapPanel m_EventActiverListWrapPanel;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventList_Listbox Name Field
        /// </summary>
        
        #line 73 "..\..\EventInstance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ListBox m_EventList_Listbox;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/eventinstance.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\EventInstance.xaml"
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
            this.m_TabControl = ((System.Windows.Controls.TabControl)(target));
            return;
            case 2:
            this.m_TriggerListBox = ((System.Windows.Controls.ListBox)(target));
            
            #line 19 "..\..\EventInstance.xaml"
            this.m_TriggerListBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(this.TriggerListBox_MouseDoubleClick);
            
            #line default
            #line hidden
            return;
            case 3:
            this.m_ActiverListBox = ((System.Windows.Controls.ListBox)(target));
            
            #line 35 "..\..\EventInstance.xaml"
            this.m_ActiverListBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(this.ActiverListBox_MouseDoubleClick);
            
            #line default
            #line hidden
            return;
            case 4:
            this.WrapPaneTabControl = ((System.Windows.Controls.TabControl)(target));
            return;
            case 5:
            this.m_EventTriggerListWrapPanel = ((System.Windows.Controls.WrapPanel)(target));
            return;
            case 6:
            this.m_EventActiverListWrapPanel = ((System.Windows.Controls.WrapPanel)(target));
            return;
            case 7:
            
            #line 71 "..\..\EventInstance.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.AddEvent_Button_Click);
            
            #line default
            #line hidden
            return;
            case 8:
            
            #line 72 "..\..\EventInstance.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.DelEvent_Button_Click);
            
            #line default
            #line hidden
            return;
            case 9:
            this.m_EventList_Listbox = ((System.Windows.Controls.ListBox)(target));
            return;
            case 10:
            
            #line 74 "..\..\EventInstance.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.EventInitButton_Click);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

