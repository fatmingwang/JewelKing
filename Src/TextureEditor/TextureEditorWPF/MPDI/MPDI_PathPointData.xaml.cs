using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using TimeLineTool;
using System.Collections.ObjectModel;
namespace TextureEditorWPF
{
    /// <summary>
    /// Interaction logic for MPDI_PathPointData.xaml
    /// </summary>
    public partial class MPDI_PathPointData : UserControl
    {
        public MPDI_PathPointData()
        {
            InitializeComponent();
            //var tmp1 = new TempDataType() { StartTime = 10, EndTime = 25, Name = "Temp 1" };
            //var tmp2 = new TempDataType() { StartTime = 3, EndTime = 18, Name = "Temp 2" };
            //var tmp3 = new TempDataType() { StartTime = 44, EndTime = 60, Name = "Temp 3" };
            //ObservableCollection<ITimeLineDataItem> t1Data = new ObservableCollection<ITimeLineDataItem>();
            //ObservableCollection<ITimeLineDataItem> t2Data = new ObservableCollection<ITimeLineDataItem>();
            //ObservableCollection<ITimeLineDataItem> t3Data = new ObservableCollection<ITimeLineDataItem>();
            //t1Data.Add(tmp1);
            //t2Data.Add(tmp2);
            //t3Data.Add(tmp3);
            //TimeLine2.Items = t2Data;
            //TimeLine3.Items = t3Data;
            //TimeLine2.StartDate = 0;
            //TimeLine3.StartDate = 0;
            //TempDataType l_TempDataType1 = CreateTempDataType("tes1", 0, 1000);
            //TempDataType l_TempDataType2 = CreateTempDataType("test2", 0, 1200);
            //TempDataType l_TempDataType3 = CreateTempDataType("test3", 0, 100);
            //AddTimeLineControl(l_TempDataType1);
            //AddTimeLineControl(l_TempDataType2);
            //AddTimeLineControl(l_TempDataType3);
        }
        private void Slider_Scale_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            m_ZoomSliderText.Text = "Zoom Slider:" + Slider_Scale.Value.ToString();
            foreach (TimeLineControl item in m_ItemControl.Items)
            {
                item.UnitSize = Slider_Scale.Value;
            }
        }
        #region CreateTempDataType
        public TempDataType CreateTempDataType(string e_strName, int e_iStartTime, int e_iEndTime)
        {
            var temp = new TempDataType()
            {
                StartTime = e_iStartTime,
                EndTime = e_iEndTime,
                Name = e_strName
            };
            return temp;
        }
        #endregion
        #region CreateDataTemplate
        DataTemplate CreateDataTemplate()
        {
            //http://stackoverflow.com/questions/248362/how-do-i-build-a-datatemplate-in-c-sharp-code
            //create the data template
            DataTemplate cardLayout = new DataTemplate();
            //cardLayout.DataType = typeof(CreditCardPayment);

            //set up the stack panel
            FrameworkElementFactory spFactory = new FrameworkElementFactory(typeof(StackPanel));
            spFactory.Name = "myComboFactory";
            spFactory.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);

            //set up the card holder textblock
            FrameworkElementFactory cardHolder = new FrameworkElementFactory(typeof(TextBlock));
            cardHolder.SetBinding(TextBlock.TextProperty, new Binding("BillToName"));
            cardHolder.SetValue(TextBlock.ToolTipProperty, "Card Holder Name");
            spFactory.AppendChild(cardHolder);

            //set up the card number textblock
            FrameworkElementFactory cardNumber = new FrameworkElementFactory(typeof(TextBlock));
            cardNumber.SetBinding(TextBlock.TextProperty, new Binding("SafeNumber"));
            cardNumber.SetValue(TextBlock.ToolTipProperty, "Credit Card Number");
            spFactory.AppendChild(cardNumber);

            //set up the notes textblock
            FrameworkElementFactory notes = new FrameworkElementFactory(typeof(TextBlock));
            notes.SetBinding(TextBlock.TextProperty, new Binding("Notes"));
            notes.SetValue(TextBlock.ToolTipProperty, "Notes");
            spFactory.AppendChild(notes);

            //set the visual tree of the data template
            cardLayout.VisualTree = spFactory;

            //set the item template to be our shiny new data template
            //drpCreditCardNumberWpf.ItemTemplate = cardLayout;
            return cardLayout;
        }
        #endregion
        #region AddTimeLineControl
        public void AddTimeLineControl(TempDataType e_TempDataType)
        {
            TimeLineTool.TimeLineControl l_NewToolLineControl = new TimeLineControl();
            ObservableCollection<TempDataType> l_NewData = new ObservableCollection<TempDataType>();
            var converter = new System.Windows.Media.BrushConverter();

            //TextBox b = (TextBox)Resources["teste01"];
            l_NewData.Add(e_TempDataType);
            l_NewToolLineControl.Items = l_NewData;

            l_NewToolLineControl.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
            l_NewToolLineControl.UnitSize = Slider_Scale.Value;
            l_NewToolLineControl.MinimumUnitWidth = 1;
            l_NewToolLineControl.Background = (Brush)converter.ConvertFromString("WhiteSmoke"); ;
            l_NewToolLineControl.DrawTimeGrid = true;
            l_NewToolLineControl.MinWidth = 50;
            l_NewToolLineControl.SynchedWithSiblings = true;
            l_NewToolLineControl.Height = 80;
            l_NewToolLineControl.ViewLevel = TimeLineViewLevel.Minutes;
            l_NewToolLineControl.ItemTemplate = (DataTemplate)Resources["UsedTemplateProperty"];

            l_NewToolLineControl.StartDate = 0;
            //m_StackPanel.Children.Add(l_NewToolLineControl);
            l_NewToolLineControl.DrawBackGround(true);
            
            m_ItemControl.Items.Add(l_NewToolLineControl);
        }
        #endregion

        private void m_ViewLevelCombox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TimeLineViewLevel   l_TimeLineViewLevel;
            switch (m_ViewLevelCombox.SelectedIndex)
            { 
                case 0:
                    l_TimeLineViewLevel = TimeLineViewLevel.Seconds;
                break;
                case 1:
                    l_TimeLineViewLevel = TimeLineViewLevel.Minutes;
                break;
                case 2:
                    l_TimeLineViewLevel = TimeLineViewLevel.Hours;
                break;
                default:
                l_TimeLineViewLevel = TimeLineViewLevel.Seconds;
                break;
            }
            foreach (TimeLineControl item in m_ItemControl.Items)
            {
                item.ViewLevel = l_TimeLineViewLevel;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            TempDataType l_TempDataType1 = CreateTempDataType("Name:" + m_ItemControl.Items.Count.ToString(),0, 1000);
            AddTimeLineControl(l_TempDataType1);
        }
    }
}
