#pragma checksum "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "603848DE82C38152EE25448F9150CA67"
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
using FMBookEditorUI.Event.Trigger;
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
    /// EventMultiButtonTrigger
    /// </summary>
    public partial class EventMultiButtonTrigger : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_SeelctionType Name Field
        /// </summary>
        
        #line 24 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public System.Windows.Controls.ComboBox m_SeelctionType;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_PrifixButtonName Name Field
        /// </summary>
        
        #line 30 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public System.Windows.Controls.TextBox m_PrifixButtonName;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_ClickCount Name Field
        /// </summary>
        
        #line 32 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public CoreWPF.Common.NumericUpDown m_ClickCount;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_ButtonName Name Field
        /// </summary>
        
        #line 35 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public System.Windows.Controls.TextBox m_ButtonName;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_ButtonList Name Field
        /// </summary>
        
        #line 36 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public System.Windows.Controls.ListBox m_ButtonList;
        
        #line default
        #line hidden
        
        
        #line 37 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        internal System.Windows.Controls.Button AddButton;
        
        #line default
        #line hidden
        
        
        #line 38 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        internal System.Windows.Controls.Button DelButton;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Button Name Field
        /// </summary>
        
        #line 40 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
        public FMBookEditorUI.Event.Trigger.EventButton m_Button;
        
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
            System.Uri resourceLocater = new System.Uri("/FMBookEditorUI;component/event/trigger/eventmultibuttontrigger.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
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
            this.m_SeelctionType = ((System.Windows.Controls.ComboBox)(target));
            return;
            case 2:
            this.m_PrifixButtonName = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.m_ClickCount = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 4:
            this.m_ButtonName = ((System.Windows.Controls.TextBox)(target));
            return;
            case 5:
            this.m_ButtonList = ((System.Windows.Controls.ListBox)(target));
            
            #line 36 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
            this.m_ButtonList.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(this.Buttonist_MouseDoubleClick);
            
            #line default
            #line hidden
            return;
            case 6:
            this.AddButton = ((System.Windows.Controls.Button)(target));
            
            #line 37 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
            this.AddButton.Click += new System.Windows.RoutedEventHandler(this.AddButton_Click);
            
            #line default
            #line hidden
            return;
            case 7:
            this.DelButton = ((System.Windows.Controls.Button)(target));
            
            #line 38 "..\..\..\..\Event\Trigger\EventMultiButtonTrigger.xaml"
            this.DelButton.Click += new System.Windows.RoutedEventHandler(this.DelButton_Click);
            
            #line default
            #line hidden
            return;
            case 8:
            this.m_Button = ((FMBookEditorUI.Event.Trigger.EventButton)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

