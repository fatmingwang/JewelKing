   M S B A M L   `   `   `  ���� U  QGongSolutions.Wpf.DragDrop, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullkLclr-namespace:GongSolutions.Wpf.DragDrop;assembly=GongSolutions.Wpf.DragDropGongSolutions.Wpf.DragDrop  G CTimeLineTool, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullA0clr-namespace:TimeLineTool;assembly=TimeLineToolTimeLineTool  TextureEditorWPF)   #TextureEditorWPF.MPDI_PathPointData   \ XPresentationFramework, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=3.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     8x,http://schemas.microsoft.com/winfx/2006/xaml    _TimeLineLclr-namespace:GongSolutions.Wpf.DragDrop;assembly=GongSolutions.Wpf.DragDrop    =tt0clr-namespace:TimeLineTool;assembly=TimeLineTool      � 	Resources   �� R �� G �� B �� A   BgColor �� Color  BgBrush&   TimeLineTool.TimeLineItemControl �� 
TargetType �� Property �� Value	 �� 
TargetType
 J� Opacity ��Name �� DataType  TimeLineTool.TempDataType �� CornerRadius �� Path  UsedTemplateProperty �� Orientation��%�  &	        &	 -     7   '  7     (�� h    *, +)&	 �    7  �� $	 255��$	 255��$	 255��$	 180���� 8   �� "  .���  � �� �� "	  .R�� .+��� 
����  �$��Stretch=�$��Stretch=�$
 0Q��� $&  PART_ContentHostPART_ContentHost��$��Stretch=�$��Stretch=��� $ PART_LeftIndicator��$
��Left=�$��Stretch=����� �� $ PART_RightIndicator��$��Right=�$��Stretch=����� �� "  .R��� $��1q�
����  �$ 3��$��10��._��� ���� $
 Name��� "  .R��� $��1q�
��� � �$ 3��$��0,0,0,0q�$��80��8��  ._��� $ Vertical=�.+��� ���� $
 Name��� ���� $ 	StartTime��� ���� $ EndTime��� ���� $ Duration�.���  � ColumnDefinitions �� �� �� .+�� $��0�� � RowDefinitions �� $	��250 ��� �� .+��� -   $��m_PathNode_listBox��$��0���� $��1��$ 
Horizontal=�.+��� -   $��SelectAll/全選���� $��StartEnd/頭尾���� $��2��$ 
Horizontal=�.+��� $��DelPoint/刪除���� $��InsertPoint/插入��� $��1�� �� �� $��14 ��� $��20 ��� $��20 �.+��� -    ��Name$ FoundMe��$��0��$��Stretch=�$
��Auto=�$
��Auto=�$��Stretch=�$	��100��.���� -   $��m_StackPanel��$ Vertical=�.+��� -   $��m_ItemControl���� -   $��m_ZoomSliderText��$��Zoom Slider:��$��1���� -    ��Name$ Slider_Scale��$��2�� � Minimum$
 0.01Q� � Maximum$	 100Q� � Value$ 1Q��� -   $��m_ViewLevelCombox��$��3�� �� SelectedIndex$ 0��.@��� $��Seconds���� $��Minutes���� $��Hours��� $��2��.+��� .@��� $
��Move���� $
��Size���� $��Behavior��