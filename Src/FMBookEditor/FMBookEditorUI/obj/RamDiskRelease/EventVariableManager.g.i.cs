﻿#pragma checksum "..\..\EventVariableManager.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "901FBE8080DA5729EF80903D84F6828C"
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


namespace FMBookEditorUI {
    
    
    /// <summary>
    /// EventVariableManager
    /// </summary>
    public partial class EventVariableManager : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_VariableName Name Field
        /// </summary>
        
        #line 22 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_VariableName;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_VariableValue Name Field
        /// </summary>
        
        #line 23 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_VariableValue;
        
        #line default
        #line hidden
        
        
        #line 24 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button button1;
        
        #line default
        #line hidden
        
        
        #line 25 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button button2;
        
        #line default
        #line hidden
        
        
        #line 26 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button button3;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventVariable_listBox Name Field
        /// </summary>
        
        #line 28 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ListBox m_EventVariable_listBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_TypeComboBox Name Field
        /// </summary>
        
        #line 29 "..\..\EventVariableManager.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ComboBox m_TypeComboBox;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/eventvariablemanager.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\EventVariableManager.xaml"
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
            this.m_VariableName = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_VariableValue = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.button1 = ((System.Windows.Controls.Button)(target));
            
            #line 24 "..\..\EventVariableManager.xaml"
            this.button1.Click += new System.Windows.RoutedEventHandler(this.Add);
            
            #line default
            #line hidden
            return;
            case 4:
            this.button2 = ((System.Windows.Controls.Button)(target));
            
            #line 25 "..\..\EventVariableManager.xaml"
            this.button2.Click += new System.Windows.RoutedEventHandler(this.Del);
            
            #line default
            #line hidden
            return;
            case 5:
            this.button3 = ((System.Windows.Controls.Button)(target));
            
            #line 26 "..\..\EventVariableManager.xaml"
            this.button3.Click += new System.Windows.RoutedEventHandler(this.Revert);
            
            #line default
            #line hidden
            return;
            case 6:
            this.m_EventVariable_listBox = ((System.Windows.Controls.ListBox)(target));
            
            #line 28 "..\..\EventVariableManager.xaml"
            this.m_EventVariable_listBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(this.EventVariable_listBox_MouseDoubleClick);
            
            #line default
            #line hidden
            return;
            case 7:
            this.m_TypeComboBox = ((System.Windows.Controls.ComboBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}
