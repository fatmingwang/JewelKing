#pragma checksum "..\..\..\FMC\FatmingUtilities.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "FF25207F3D43AE8DD69EE52FBC9457C0"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.18010
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using CoreWPF.FMC;
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


namespace CoreWPF.FMC {
    
    
    /// <summary>
    /// FatmingUtilities
    /// </summary>
    public partial class FatmingUtilities : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        /// <summary>
        /// m_TabControl Name Field
        /// </summary>
        
        #line 14 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public System.Windows.Controls.TabControl m_TabControl;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_PI Name Field
        /// </summary>
        
        #line 16 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.PI m_PI;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_MPDI Name Field
        /// </summary>
        
        #line 19 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.MPDI m_MPDI;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Image Name Field
        /// </summary>
        
        #line 22 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.Image m_Image;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Sound Name Field
        /// </summary>
        
        #line 25 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.Sound m_Sound;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Particle Name Field
        /// </summary>
        
        #line 28 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.Particle m_Particle;
        
        #line default
        #line hidden
        
        /// <summary>
        /// m_Text Name Field
        /// </summary>
        
        #line 31 "..\..\..\FMC\FatmingUtilities.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public CoreWPF.FMC.Text m_Text;
        
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
            System.Uri resourceLocater = new System.Uri("/CoreWPF;component/fmc/fatmingutilities.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\FMC\FatmingUtilities.xaml"
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
            
            #line 14 "..\..\..\FMC\FatmingUtilities.xaml"
            this.m_TabControl.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler(this.TabControl_SelectionChanged);
            
            #line default
            #line hidden
            return;
            case 2:
            this.m_PI = ((CoreWPF.FMC.PI)(target));
            return;
            case 3:
            this.m_MPDI = ((CoreWPF.FMC.MPDI)(target));
            return;
            case 4:
            this.m_Image = ((CoreWPF.FMC.Image)(target));
            return;
            case 5:
            this.m_Sound = ((CoreWPF.FMC.Sound)(target));
            return;
            case 6:
            this.m_Particle = ((CoreWPF.FMC.Particle)(target));
            return;
            case 7:
            this.m_Text = ((CoreWPF.FMC.Text)(target));
            return;
            }
            this._contentLoaded = true;
        }
    }
}

