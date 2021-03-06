#pragma checksum "..\..\EventObjectInstanceList.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "A5D554177AE5D86A500F4B63170EF956"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18010
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using CoreWPF.Common;
using FMBookEditorUI;
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
    /// EventObjectInstanceList
    /// </summary>
    public partial class EventObjectInstanceList : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_EventObjectInstanceName_textBox Name Field
        /// </summary>
        
        #line 27 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_EventObjectInstanceName_textBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventInstance_textBox Name Field
        /// </summary>
        
        #line 31 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_EventInstance_textBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventStatus_textBox Name Field
        /// </summary>
        
        #line 35 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_EventStatus_textBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventVariable_textBox Name Field
        /// </summary>
        
        #line 39 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TextBox m_EventVariable_textBox;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_AllStatusWorkingAtSameTime_CheckBox Name Field
        /// </summary>
        
        #line 42 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.CheckBox m_AllStatusWorkingAtSameTime_CheckBox;
        
        #line default
        #line hidden
        
        
        #line 50 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_Add_button;
        
        #line default
        #line hidden
        
        
        #line 51 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button m_Del_button;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EventObjectInstance_listBox Name Field
        /// </summary>
        
        #line 56 "..\..\EventObjectInstanceList.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ListBox m_EventObjectInstance_listBox;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/eventobjectinstancelist.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\EventObjectInstanceList.xaml"
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
            this.m_EventObjectInstanceName_textBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 2:
            this.m_EventInstance_textBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.m_EventStatus_textBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 4:
            this.m_EventVariable_textBox = ((System.Windows.Controls.TextBox)(target));
            return;
            case 5:
            this.m_AllStatusWorkingAtSameTime_CheckBox = ((System.Windows.Controls.CheckBox)(target));
            return;
            case 6:
            this.m_Add_button = ((System.Windows.Controls.Button)(target));
            
            #line 50 "..\..\EventObjectInstanceList.xaml"
            this.m_Add_button.Click += new System.Windows.RoutedEventHandler(this.Add);
            
            #line default
            #line hidden
            return;
            case 7:
            this.m_Del_button = ((System.Windows.Controls.Button)(target));
            
            #line 51 "..\..\EventObjectInstanceList.xaml"
            this.m_Del_button.Click += new System.Windows.RoutedEventHandler(this.Del);
            
            #line default
            #line hidden
            return;
            case 8:
            this.m_EventObjectInstance_listBox = ((System.Windows.Controls.ListBox)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

