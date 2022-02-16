﻿#pragma checksum "..\..\EventObjectStatusList.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "B5056EFB6D84BA3D29B8550A9F492B0C"
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
    /// EventObjectStatusList
    /// </summary>
    public partial class EventObjectStatusList : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_EventObjectStatusName_textBox Name Field
        /// </summary>
        
        #line 22 "..\..\EventObjectStatusList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_EventObjectStatusName_textBox;
        
        #line default
        #line hidden
        
        
        #line 30 "..\..\EventObjectStatusList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_AddEvent_button;
        
        #line default
        #line hidden
        
        
        #line 31 "..\..\EventObjectStatusList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_DelEvent_button;
        
        #line default
        #line hidden
        
        
        #line 32 "..\..\EventObjectStatusList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_RevertEvent_button;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventObjectStatusList_listBox Name Field
        /// </summary>
        
        #line 34 "..\..\EventObjectStatusList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ListBox m_EventObjectStatusList_listBox;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/eventobjectstatuslist.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\EventObjectStatusList.xaml"
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
            this.m_EventObjectStatusName_textBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_AddEvent_button = ((System.Windows.Controls.Button)(target));
            
            #line 30 "..\..\EventObjectStatusList.xaml"
            this.m_AddEvent_button.Click += new System.Windows.RoutedEventHandler(this.Add);
            
            #line default
            #line hidden
            return;
            case 3:
            this.m_DelEvent_button = ((System.Windows.Controls.Button)(target));
            
            #line 31 "..\..\EventObjectStatusList.xaml"
            this.m_DelEvent_button.Click += new System.Windows.RoutedEventHandler(this.Del);
            
            #line default
            #line hidden
            return;
            case 4:
            this.m_RevertEvent_button = ((System.Windows.Controls.Button)(target));
            
            #line 32 "..\..\EventObjectStatusList.xaml"
            this.m_RevertEvent_button.Click += new System.Windows.RoutedEventHandler(this.Revert);
            
            #line default
            #line hidden
            return;
            case 5:
            this.m_EventObjectStatusList_listBox = ((System.Windows.Controls.ListBox)(target));
            
            #line 34 "..\..\EventObjectStatusList.xaml"
            this.m_EventObjectStatusList_listBox.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler(this.m_EventObjectStatusList_listBox_SelectionChanged);
            
            #line default
            #line hidden
            
            #line 34 "..\..\EventObjectStatusList.xaml"
            this.m_EventObjectStatusList_listBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(this.listBox_MouseDoubleClick);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}
