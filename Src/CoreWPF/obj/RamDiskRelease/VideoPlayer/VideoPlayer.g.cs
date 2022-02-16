﻿#pragma checksum "..\..\..\VideoPlayer\VideoPlayer.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "66EC5F3F73B5A69E172D73E21F644BF9"
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
using Microsoft.Windows.Themes;
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


namespace CoreWPF.VideoPlayer {
    
    
    /// <summary>
    /// VideoPlayer
    /// </summary>
    public partial class VideoPlayer : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_FilmNameList Name Field
        /// </summary>
        
        #line 17 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.ListBox m_FilmNameList;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_StartH Name Field
        /// </summary>
        
        #line 20 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_StartH;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_StartM Name Field
        /// </summary>
        
        #line 21 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_StartM;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_StartS Name Field
        /// </summary>
        
        #line 22 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_StartS;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EndH Name Field
        /// </summary>
        
        #line 26 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_EndH;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EndM Name Field
        /// </summary>
        
        #line 27 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_EndM;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_EndS Name Field
        /// </summary>
        
        #line 28 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.Common.NumericUpDown m_EndS;
        
        #line default
        #line hidden
        
        
        #line 31 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Label lblSave;
        
        #line default
        #line hidden
        
        
        #line 32 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Label lblLoad;
        
        #line default
        #line hidden
        
        
        #line 33 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Label lblClear;
        
        #line default
        #line hidden
        
        
        #line 34 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Label lbl3D;
        
        #line default
        #line hidden
        
        
        #line 35 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Label lblNormal;
        
        #line default
        #line hidden
        
        
        #line 39 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnPlay;
        
        #line default
        #line hidden
        
        
        #line 40 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnPause;
        
        #line default
        #line hidden
        
        
        #line 41 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnStop;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_SliderTime Name Field
        /// </summary>
        
        #line 45 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.Slider m_SliderTime;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_TimeText Name Field
        /// </summary>
        
        #line 46 "..\..\..\VideoPlayer\VideoPlayer.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.Label m_TimeText;
        
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
            System.Uri resourceLocater = new System.Uri("/CoreWPF;component/videoplayer/videoplayer.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal System.Delegate _CreateDelegate(System.Type delegateType, string handler) {
            return System.Delegate.CreateDelegate(delegateType, this, handler);
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
            
            #line 16 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            ((System.Windows.Controls.Label)(target)).DragEnter += new System.Windows.DragEventHandler(this.Label_DragEnter);
            
            #line default
            #line hidden
            
            #line 16 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            ((System.Windows.Controls.Label)(target)).Drop += new System.Windows.DragEventHandler(this.Label_Drop);
            
            #line default
            #line hidden
            return;
            case 2:
            this.m_FilmNameList = ((System.Windows.Controls.ListBox)(target));
            return;
            case 3:
            this.m_StartH = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 4:
            this.m_StartM = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 5:
            this.m_StartS = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 6:
            this.m_EndH = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 7:
            this.m_EndM = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 8:
            this.m_EndS = ((CoreWPF.Common.NumericUpDown)(target));
            return;
            case 9:
            this.lblSave = ((System.Windows.Controls.Label)(target));
            
            #line 31 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.lblSave.MouseUp += new System.Windows.Input.MouseButtonEventHandler(this.lblSave_MouseUp);
            
            #line default
            #line hidden
            return;
            case 10:
            this.lblLoad = ((System.Windows.Controls.Label)(target));
            return;
            case 11:
            this.lblClear = ((System.Windows.Controls.Label)(target));
            return;
            case 12:
            this.lbl3D = ((System.Windows.Controls.Label)(target));
            return;
            case 13:
            this.lblNormal = ((System.Windows.Controls.Label)(target));
            return;
            case 14:
            this.btnPlay = ((System.Windows.Controls.Button)(target));
            
            #line 39 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.btnPlay.Click += new System.Windows.RoutedEventHandler(this.btnPlay_Click);
            
            #line default
            #line hidden
            return;
            case 15:
            this.btnPause = ((System.Windows.Controls.Button)(target));
            
            #line 40 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.btnPause.Click += new System.Windows.RoutedEventHandler(this.btnPause_Click);
            
            #line default
            #line hidden
            return;
            case 16:
            this.btnStop = ((System.Windows.Controls.Button)(target));
            
            #line 41 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.btnStop.Click += new System.Windows.RoutedEventHandler(this.btnStop_Click);
            
            #line default
            #line hidden
            return;
            case 17:
            this.m_SliderTime = ((System.Windows.Controls.Slider)(target));
            
            #line 45 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.m_SliderTime.ValueChanged += new System.Windows.RoutedPropertyChangedEventHandler<double>(this.sliderTime_ValueChanged);
            
            #line default
            #line hidden
            
            #line 45 "..\..\..\VideoPlayer\VideoPlayer.xaml"
            this.m_SliderTime.MouseUp += new System.Windows.Input.MouseButtonEventHandler(this.m_SliderTime_MouseUp);
            
            #line default
            #line hidden
            return;
            case 18:
            this.m_TimeText = ((System.Windows.Controls.Label)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

